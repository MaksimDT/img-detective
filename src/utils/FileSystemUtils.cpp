#include "utils/FileSystemUtils.h"
#include "utils/MemoryUtils.h"
#include <boost/filesystem.hpp>
#include <fstream>

using namespace std;

namespace ImgDetective {
namespace Utils {

    FileNotFoundException::FileNotFoundException(const std::string& msg, const std::wstring& filePath)
        :exception(msg.c_str()), filePath(filePath) {}

    std::wstring FileNotFoundException::GetFilePath() const {
        return filePath;
    }

    Core::blob_p_t FileSystem::ReadAllBytes(const boost::filesystem::path& path) {
        wstring pathStr = path.wstring();

        ifstream file(pathStr, ios::in|ios::binary|ios::ate);
        if (file.is_open())
        {
            Core::blob_p_t contents;
            try {
                ifstream::pos_type size = file.tellg();
                contents = Core::CreateBlobOfSize(size);
                file.seekg(0, ios::beg);
                file.read(contents->data(), size);
                file.close();
            }
            catch (...) {
                Core::SafeFreeBlob(contents);
                throw;
            }

            return contents;
        }
        else {
            throw FileNotFoundException("Cannot open file", pathStr);
        }
    }

    void FileSystem::WriteAllBytes(const char* buffer, size_t bufferSize, const boost::filesystem::path& targetPath) {
        ofstream file(targetPath.wstring(), ios::out|ios::binary|ios::ate);

        if (file.is_open()) {
            file.write(buffer, bufferSize);
        }
    }

    boost::filesystem::path FileSystem::MakeRelative(const boost::filesystem::path& basePath, const boost::filesystem::path& target) {
        if (!basePath.is_absolute() || !target.is_absolute()) {
            throw std::exception("both specified paths must be absolute");
        }

        if (!HasParentChildRelationship(basePath, target)) {
            throw std::exception("the target must be a child node of the basePath");
        }

        boost::filesystem::path::iterator basePathIt = basePath.begin();
        boost::filesystem::path::iterator targetIt = target.begin();

        while (basePathIt != basePath.end()) {
            if (*basePathIt == ".") {
                ++basePathIt;
                continue;
            }

            ++basePathIt;
            ++targetIt;
        }

        boost::filesystem::path resultPath;

        while (targetIt != target.end()) {
            resultPath.append(targetIt->wstring().cbegin(), targetIt->wstring().cend());
            ++targetIt;
        }

        return resultPath;
    }

    bool FileSystem::HasParentChildRelationship(const boost::filesystem::path& supposedParent, const boost::filesystem::path& supposedChild) {
        boost::filesystem::path::iterator parentIt = supposedParent.begin();
        boost::filesystem::path::iterator childIt = supposedChild.begin();

        while (parentIt != supposedParent.end() && childIt != supposedChild.end()) {
            //if parent path has a slash at the end then its iterator's last value will be a dot.
            //let's ignore that final slash
            if (*parentIt == ".") {
                ++parentIt;
                continue;
            }

            if (*parentIt != *childIt) {
                return false;
            }

            ++parentIt; 
            ++childIt;
        }

        if (parentIt == supposedParent.end() && childIt != supposedChild.end()) {
            return true;
        }
        else {
            return false;
        }
    }
}
}