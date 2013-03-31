#pragma once

#include "common/CommonInternal.h"
#include <string>
#include <Magick++.h>

namespace ImgDetective {
namespace Core {

	//internal representation of image
	CONCRETE SEALED class ImgInfo : NONCOPYABLE {
	public:
		static ImgInfo* Create(const REF ImgShortInfo& shortInfo);

        imgid_t GetId() const;
        void SetId(imgid_t imgId);
        Magick::Image& GetMagickImage();
	private:
		CTOR ImgInfo(const REF Magick::Image& magickImg);
		COPYCTOR ImgInfo(const REF ImgInfo& other);
		
		EXCLUSIVE Magick::Image magickImg;
        imgid_t imgId;
        std::string path;
	};

}
}