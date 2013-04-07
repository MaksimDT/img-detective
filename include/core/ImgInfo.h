#pragma once

#include "common/CommonInternal.h"
#include <string>
#pragma warning(push, 0)        
#include <Magick++.h>
#pragma warning(pop)

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