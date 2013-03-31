#pragma once

#include "core/ImgInfo.h"
#include <string>

namespace ImgDetective {
namespace Core {

    INTERFACE class ImgContentStorage : NONCOPYABLE, HAS_VIRTUAL_DESCTUCTOR {
    public:
        virtual std::string Write(REF ImgInfo& imgInfo) = 0;
    };

}
}