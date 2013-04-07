#pragma once

#include "core/ImgInfo.h"
#include <string>

namespace ImgDetective {
namespace Core {

    INTERFACE class ImgContentStorage : NONCOPYABLE, HAS_VIRTUAL_DESCTUCTOR {
    public:
        virtual void Write(REF ImgInfo& img) = 0;
    };

}
}