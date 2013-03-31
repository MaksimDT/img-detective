#include "core/MySqlDbWrapper.h"
#include "utils/MemoryUtils.h"

using namespace std;

namespace ImgDetective {
namespace Core {

    CTOR MySqlDbWrapper::MySqlDbWrapper(const REF MySqlDbWrapper::ConnectionSettings& conSettings) {
        this->conSettings = conSettings;
    }

    void MySqlDbWrapper::ExecuteNonQuery(const REF std::string& sqlStatement, const REF DbWrapper::params_list_t& params) const {
        MYSQL* con = Connect();
        
        MYSQL_STMT* stmt = mysql_stmt_init(con);

        if (stmt == NULL) {
            throw std::exception("couldn't allocate MYSQL_STMT structure");
        }

        MYSQL_BIND* bind = NULL;

        try {
            if (mysql_stmt_prepare(stmt, sqlStatement.c_str(), sqlStatement.length())) {
                throw std::exception("mysql_stmt_prepare failed");
            }

            bind = new MYSQL_BIND[params.size()];
            memset(bind, 0, sizeof(MYSQL_BIND) * params.size());
        
            params_list_t::const_iterator it;
            size_t paramIndex = 0;
            for (it = params.cbegin(); it != params.cend(); ++it) {
                bind[paramIndex] = PrepareBindStruct(*it);
            }

            if (mysql_stmt_bind_param(stmt, bind)) {
                throw std::exception("mysql_stmt_bind_param failed");
            }

            if (mysql_stmt_execute(stmt)) {
                throw std::exception("mysql_stmt_execute failed");
            }
        }
        catch (...) {
            Disconnect(con);
            mysql_stmt_close(stmt);
            Utils::Memory::SafeDeleteArray(bind);
            throw;
        }

        Disconnect(con);
        mysql_stmt_close(stmt);
        Utils::Memory::SafeDeleteArray(bind);
    }

    MYSQL* MySqlDbWrapper::Connect() const {
        MYSQL* con = mysql_init(NULL);
        
        if (con == NULL) {
            throw std::exception("couldn't allocate MYSQL object");
        }

        if (mysql_real_connect(con, conSettings.host.c_str(), conSettings.login.c_str(), conSettings.password.c_str(), conSettings.dbName.c_str(), conSettings.port, NULL, 0) == NULL) {
            Utils::Memory::SafeDelete(con);
            throw std::exception("couldn't connect to mysql database");
        }

        return con;
    }

    void MySqlDbWrapper::Disconnect(MYSQL* con) const {
        if (con != NULL) {
            mysql_close(con);
        }
    }

    MYSQL_BIND MySqlDbWrapper::PrepareBindStruct(const REF ParamInfo& paramInfo) const {
        MYSQL_BIND bind;
        bind.buffer = paramInfo.data;
        bind.buffer_length = paramInfo.dataLength;

        enum_field_types mysqltype;
        switch (paramInfo.typeId) {
        case DbType::LONGLONG:
            mysqltype = MYSQL_TYPE_LONGLONG;
            break;
        case DbType::BLOB:
            mysqltype = MYSQL_TYPE_BLOB;
            break;
        default:
            //TODO: individual derived class for this exception
            throw std::exception("unknown parameter type");
        }

        bind.buffer_type = mysqltype;
        bind.is_unsigned = !paramInfo.isSigned;

        return bind;
    }
}
}