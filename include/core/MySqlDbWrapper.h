#pragma once

#include "core/DbWrapper.h"
#include "core/MySqlConnectionSettings.h"
#include "core/mysqlheaders.h"

namespace ImgDetective {
namespace Core {
namespace Db {

    CONCRETE SEALED class MySqlDbWrapper : public DbWrapper {
    public:
        CTOR MySqlDbWrapper(const MySqlConnectionSettings& conSettings);

        //used for the execution of insert, update, delete statements
        virtual NonQueryExecResult ExecuteNonQuery(const std::string& sqlStatement, const REF params_list_t& params) const;
        //used for the execution of select statements
        virtual DbResultReader* ExecuteReader(const std::string& sqlStatement, const params_list_t& params) const;
    private:
        //connects to the mysql server
        MYSQL* Connect() const;
        //disconnects from the mysql server
        void Disconnect(MYSQL* con) const;
        //connects to the mysql server then prepares and executes specified statement. Returns the handles to db connection and executed statement
        void ExecuteStatement(const std::string& sqlStatement, const params_list_t& params, OUT MYSQL*& connection, OUT MYSQL_STMT*& stmt) const;

        //universal params format => mysql params format (for collection of params)
        static MYSQL_BIND* PrepareParams(const params_list_t& params);
        //mysql result metadata => mysql result set buffers + fields metadata
        static MYSQL_BIND* PrepareFieldBuffers(MYSQL_RES* resultMetadata, OUT MYSQL_FIELD*& fieldsMetadata);
        //universal param format => mysql param format
        static MYSQL_BIND PrepareParamInfo(const REF DbParamBuffer& param);

        MySqlConnectionSettings conSettings;
    };

}
}
}