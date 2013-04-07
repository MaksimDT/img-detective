#pragma once

#include "common/CommonInternal.h"

namespace ImgDetective {
namespace Core {
namespace Db {

    NESTED struct MySqlConnectionSettings {
    public:
        std::string host;
        int port;
        std::string login;
        std::string password;
        std::string dbName;
    };

}
}
}