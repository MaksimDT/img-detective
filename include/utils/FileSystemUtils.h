#pragma once

#include "common/CommonInternal.h"
#include <boost/filesystem.hpp>
#include <string>

namespace ImgDetective {
namespace Utils {

    class FileNotFoundException : public std::exception {
    public:
        CTOR FileNotFoundException(const std::string& message, const std::wstring& filePath);

        std::wstring GetFilePath() const;
    private:
        std::wstring filePath;
    };

    class FileSystem {
    public:
        static Core::blob_p_t ReadAllBytes(const boost::filesystem::path& path);
        static void WriteAllBytes(const char* buffer, size_t bufferSize, const boost::filesystem::path& targetPath);
        static boost::filesystem::path MakeRelative(const boost::filesystem::path& basePath, const boost::filesystem::path& target);
        static bool HasParentChildRelationship(const boost::filesystem::path& supposedParent, const boost::filesystem::path& supposedChild);
    };

}
}