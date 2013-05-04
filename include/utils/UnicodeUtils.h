#pragma once

#include <string>
#include "common/CommonInternal.h"

namespace ImgDetective {
namespace Utils {

    class Unicode {
    public:
        static void ToUtf8cstr(const std::wstring& source, OUT char*& bufferPtr, OUT size_t& bufferLength);
        static std::string ToUtf8(const std::wstring& source);
        static std::wstring Utf8ToUtf16(const std::string& source);
    };

}
}