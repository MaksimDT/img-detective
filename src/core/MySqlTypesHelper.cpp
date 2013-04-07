#include "core/MySqlTypesHelper.h"
#include <stdexcept>

namespace ImgDetective {
namespace Core {
namespace Db {

    unsigned int MySqlTypesHelper::GetMySqlTypeLength(enum_field_types mysqlType) {
        switch (mysqlType) {
        case MYSQL_TYPE_LONGLONG:
            return 8;
        case MYSQL_TYPE_BLOB:
            //variable length
            return 0;
        default:
            //TODO: new derived class for this exception
            throw std::exception("unknown db type");
        }
    }

    enum_field_types MySqlTypesHelper::GetMySqlType(DbType::Enum dbType) {
        switch (dbType) {
        case DbType::LONGLONG:
            return MYSQL_TYPE_LONGLONG;
        case DbType::BLOB:
            return MYSQL_TYPE_BLOB;
        default:
            //TODO: new derived class for this exception
            throw std::exception("unknown db type");
        }
    }

    DbType::Enum MySqlTypesHelper::GetDbTypeFromMySqlType(enum_field_types mysqlType) {
        switch (mysqlType)
        {
        case MYSQL_TYPE_LONGLONG:
            return DbType::LONGLONG;
        case MYSQL_TYPE_BLOB:
            return DbType::BLOB;
        default:
            throw std::exception("unknown db type");
        }
    }

}
}
}