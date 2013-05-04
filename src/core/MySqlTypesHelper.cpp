#include "core/MySqlTypesHelper.h"
#include <stdexcept>

namespace ImgDetective {
namespace Core {
namespace Db {

    unsigned int MySqlTypesHelper::GetMySqlTypeLength(enum_field_types mysqlType, MYSQL_FIELD* field) {
        switch (mysqlType) {
        case MYSQL_TYPE_LONGLONG:
            return 8;
        case MYSQL_TYPE_LONG:
            return 4;
        case MYSQL_TYPE_BLOB:
            //variable length
            return 0;
        case MYSQL_TYPE_VARCHAR:
            //variable length
            return 0;
        case MYSQL_TYPE_VAR_STRING:
            return field == NULL ? 0 : field->length;
        default:
            //TODO: new derived class for this exception
            throw std::exception("unknown db type");
        }
    }

    enum_field_types MySqlTypesHelper::GetMySqlType(DbType::Enum dbType) {
        switch (dbType) {
        case DbType::LONGLONG:
            return MYSQL_TYPE_LONGLONG;
        case DbType::LONG:
            return MYSQL_TYPE_LONG;
        case DbType::BLOB:
            return MYSQL_TYPE_BLOB;
        case DbType::VARCHAR:
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
        case MYSQL_TYPE_LONG:
            return DbType::LONG;
        case MYSQL_TYPE_BLOB:
            return DbType::BLOB;
        case MYSQL_TYPE_VARCHAR:
            return DbType::VARCHAR;
        case MYSQL_TYPE_VAR_STRING:
            return DbType::BLOB;
        default:
            throw std::exception("unknown db type");
        }
    }

}
}
}