#pragma once

#include "core/ImgInfo.h"
#include <string>

namespace ImgDetective {
namespace Core {

    class ImgContentStorage : NONCOPYABLE, HAS_VIRTUAL_DESCTUCTOR {
    public:
        virtual void Write(ImgInfo& img) const = 0;
    };

}
}