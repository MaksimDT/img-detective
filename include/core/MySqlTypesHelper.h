#pragma once

#include "core/DbType.h"
#include "core/mysqlheaders.h"

namespace ImgDetective {
namespace Core {
namespace Db {

    class MySqlTypesHelper {
    public:
        //length in bytes of mysql data type
        static unsigned int GetMySqlTypeLength(enum_field_types mysqlType, MYSQL_FIELD* field = NULL);
        //universal db type => mysql db type
        static enum_field_types GetMySqlType(DbType::Enum dbType);
        //mysql db type => universal db type
        static DbType::Enum GetDbTypeFromMySqlType(enum_field_types mysqlType);
    };

}
}
}