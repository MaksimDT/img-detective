#include "core/ImgInfo.h"
#include "utils/ContractUtils.h"

namespace ImgDetective {
namespace Core {

	ImgInfo* ImgInfo::Create(const REF ImgShortInfo& shortInfo) {
		if (shortInfo.contentSize <= 0) {
			throw std::exception("contentSize field must be a positive integer");
		}

		ImgInfo* parsedImg;

		try {
			Magick::Blob imgContentBlob(shortInfo.content, shortInfo.contentSize);
			Magick::Image magickImg(imgContentBlob);

			parsedImg = new ImgInfo(magickImg);
		}
		catch (std::exception ex) {
			//TODO: logging
			throw;
		}

		return parsedImg;
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

	CTOR ImgInfo::ImgInfo(const REF Magick::Image& magickImg) {
		this->magickImg = magickImg;
	}
}
}