#include "utils/UnicodeUtils.h"
#include "utils/MemoryUtils.h"
#include <stdexcept>
#include <Windows.h>

namespace ImgDetective {
namespace Utils {

    std::string Unicode::ToUtf8(const std::wstring& source) {
        char* utf8buf = NULL;
        size_t utf8buf_length = 0;

        try {
            ToUtf8cstr(source, REF utf8buf, REF utf8buf_length);
            std::string result;

            if (utf8buf != NULL) {
                result.assign(utf8buf, utf8buf_length);
                Utils::Memory::SafeDeleteArray(utf8buf);
            }
            
            return result;
        }
        catch (...) {
            Utils::Memory::SafeDeleteArray(utf8buf);
            throw;
        }
    }

    void Unicode::ToUtf8cstr(const std::wstring& source, REF char*& bufferPtr, REF size_t& bufferLength) {
        if (source.empty()) {
            bufferPtr = NULL;
            bufferLength = 0;
            return;
        }

        char* utf8buf = NULL;

        try {
            bufferLength = WideCharToMultiByte(CP_UTF8, 0, source.c_str(), source.length(), NULL, 0, NULL, NULL);

            if (bufferLength == 0) {
                throw std::exception("could not convert utf16 string to utf8 string");
            }

            utf8buf = new char[bufferLength + 1]; //additional 1 byte for null terminator
            WideCharToMultiByte(CP_UTF8, 0, source.c_str(), source.length(), utf8buf, bufferLength, NULL, NULL);
            utf8buf[bufferLength] = '\0';

            bufferPtr = utf8buf;
        }
        catch (...) {
            Utils::Memory::SafeDeleteArray(utf8buf);
            throw;
        }
    }

    std::wstring Unicode::Utf8ToUtf16(const std::string& source) {
        if (source.empty()) {
            return std::wstring();
        }

        wchar_t* utf16buf = NULL;

        try {
            size_t requiredSize = MultiByteToWideChar(CP_UTF8, 0, source.c_str(), source.length(), NULL, 0);

            if (requiredSize == 0) {
                throw std::exception("could not convert utf8 string to utf16 string");
            }

            utf16buf = new wchar_t[requiredSize + 1];
            MultiByteToWideChar(CP_UTF8, 0, source.c_str(), source.length(), utf16buf, requiredSize);
            //just in case
            utf16buf[requiredSize] = '\0';

            std::wstring result(utf16buf, requiredSize);
            Utils::Memory::SafeDeleteArray(utf16buf);
            return result;
        }
        catch (...) {
            Utils::Memory::SafeDeleteArray(utf16buf);
            throw;
        }
    }

}
}