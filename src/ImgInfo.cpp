#include "ImgInfo.h"
#include "ContractUtils.h"

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
			throw std::exception("Error occured while parsing provided image data");
		}

		return parsedImg;
	}

	CTOR ImgInfo::ImgInfo(const REF Magick::Image& magickImg) {
		this->magickImg = magickImg;
	}
}
}