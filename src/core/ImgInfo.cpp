#include "core/ImgInfo.h"
#include "utils/ContractUtils.h"
#include "utils/FileSystemUtils.h"
#include "utils/MemoryUtils.h"

using namespace std;

namespace ImgDetective {
namespace Core {

	ImgInfo* ImgInfo::Create(const RawImg& rawImg) {
        Utils::Contract::Assert(rawImg.contentSize > 0);
        Utils::Contract::AssertNotNull(rawImg.content);
        Utils::Contract::AssertNotNull(rawImg.fileExtension);

		ImgInfo* parsedImg = NULL;

		try {
			Magick::Blob imgContentBlob(rawImg.content, rawImg.contentSize);
			Magick::Image magickImg(imgContentBlob);

            parsedImg = new ImgInfo(magickImg, "", REPO_ID_UNDEFINED);
            parsedImg->SetFileExtension(rawImg.fileExtension);
		}
		catch (std::exception ex) {
            Utils::Memory::SafeDelete(parsedImg);
			//TODO: logging
			throw;
		}

		return parsedImg;
	}

    ImgInfo* ImgInfo::Create(repoid_t repoId, const boost::filesystem::path& filePath) {
        blob_p_t fileContents = NULL;
        try {
            fileContents = Utils::FileSystem::ReadAllBytes(filePath);

            Magick::Blob magickBlob(Core::BlobToCharArray(fileContents), fileContents->size());
            Magick::Image magickImg(magickBlob);
            ImgInfo* parsedImg = new ImgInfo(magickImg, filePath, repoId);
            
            Core::SafeFreeBlob(fileContents);
            return parsedImg;
        }
        catch (...) {
            Core::SafeFreeBlob(fileContents);
            throw;
        }
    }

    imgid_t ImgInfo::GetId() const {
        return this->imgId;
    }

    void ImgInfo::SetId(imgid_t imgId) {
        Utils::Contract::Assert(imgId > 0);
        this->imgId = imgId;
    }

    Magick::Image& ImgInfo::GetMagickImage() {
        return magickImg;
    }

    const Magick::Image& ImgInfo::GetMagickImage() const {
        return magickImg;
    }

    boost::filesystem::path ImgInfo::GetPath() const {
        return filePath;
    }

    void ImgInfo::SetPath(const boost::filesystem::path& path) {
        this->filePath = path;
    }

    repoid_t ImgInfo::GetRepoId() const {
        return repoId;
    }

    void ImgInfo::SetRepoId(repoid_t repoId) {
        this->repoId = repoId;
    }

    std::string ImgInfo::GetFileExtension() const {
        return this->fileExtension;
    }

	CTOR ImgInfo::ImgInfo(const REF Magick::Image& magickImg, const boost::filesystem::path& filePath, repoid_t repoId) {
		this->magickImg = magickImg;
        this->imgId = IMGID_UNDEFINED;
        this->filePath = filePath;
        this->repoId = repoId;
	}

    void ImgInfo::SetFileExtension(const std::string& ext) {
        Utils::Contract::Assert(!ext.empty() && !(ext[0] == '.' && ext.length() == 1));

        std::string cleanExt;

        if (ext[0] == '.') {
            cleanExt = ext.substr(1, ext.length() - 1);
        }
        else {
            cleanExt = ext;
        }

        this->fileExtension = cleanExt;
    }
}
}