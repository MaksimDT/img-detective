#pragma once

#include "common/CommonInternal.h"

namespace ImgDetective {
namespace Utils {

    class Graphics {
    public:
        static Core::YCbCrColor RGBtoYCbCr(unsigned char r, unsigned char g, unsigned char b);
    };

}
}