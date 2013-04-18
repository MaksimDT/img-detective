#pragma once

namespace ImgDetective {
namespace Core {
namespace Db {

    struct DbType {
        enum Enum {
            UNKNOWN = 0,
            VARCHAR,
            LONGLONG,
            BLOB
        };
    };

}
}
}