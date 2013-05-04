#include "core/FsImgStorage.h"
#include "utils/ContractUtils.h"
#include "utils/FileSystemUtils.h"
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/locale/encoding_utf.hpp>

using namespace std;

namespace ImgDetective {
namespace Core {

    CTOR FsImgStorage::FsImgStorage(const REF boost::filesystem::path& uploadDirPath) {
        this->uploadDirPath = uploadDirPath;

        if (!boost::filesystem::exists(uploadDirPath)) {
            boost::filesystem::create_directory(uploadDirPath);
        }
    }
    
    void FsImgStorage::Write(ImgInfo& img) const {
        Utils::Contract::Assert(img.GetId() != IMGID_UNDEFINED);
        Utils::Contract::Assert(!img.GetFileExtension().empty());

        boost::filesystem::path uploadFilePath = GetImgPath(img.GetId(), img.GetFileExtension());

        Magick::Blob magickBlob;
        img.GetMagickImage().write(&magickBlob);

        Utils::FileSystem::WriteAllBytes((const char*)magickBlob.data(), magickBlob.length(), uploadFilePath);
    }

    boost::filesystem::path FsImgStorage::GetImgPath(imgid_t imgId, const std::string& extension) const {
        Utils::Contract::Assert(imgId != IMGID_UNDEFINED);
        Utils::Contract::Assert(!extension.empty());

        boost::filesystem::path uploadFilePath = this->uploadDirPath;
        string fileNameStr = boost::lexical_cast<string, imgid_t>(imgId);
        fileNameStr.append(".");
        fileNameStr.append(extension);
        uploadFilePath.append(fileNameStr.cbegin(), fileNameStr.cend());

        return uploadFilePath;
    }

}
}