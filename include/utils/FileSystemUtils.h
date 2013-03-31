#pragma once

#include "common/CommonInternal.h"
#include <string>

namespace ImgDetective {
namespace Utils {

    class FileNotFoundException : public std::exception {
    public:
        FileNotFoundException(const std::string& message);
    };

    class FileSystem {
    public:
        static Core::blob_p_t ReadAllBytes(const std::string& path);
    };

}
}