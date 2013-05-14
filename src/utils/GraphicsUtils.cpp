#include "utils/GraphicsUtils.h"

namespace ImgDetective {
namespace Utils {

    Core::YCbCrColor Graphics::RGBtoYCbCr(unsigned char r, unsigned char g, unsigned char b) {
        const double k_ry = 0.299;
        const double k_by = 0.114;
        const double k_gy = 0.587;  //1 - k_ry - k_by

        unsigned char y = (unsigned char)(k_ry * r + k_gy * g + k_by * b);
        unsigned char cb = b - y;
        unsigned char cr = r - y;

        Core::YCbCrColor result;
        result.y = y;
        result.cb = cb;
        result.cr = cr;

        return result;
    }

}
}