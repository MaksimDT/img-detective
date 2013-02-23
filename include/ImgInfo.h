#pragma once

#include "ImgDetectiveApi.h"
#include "CommonInternal.h"
#include <Magick++.h>

namespace ImgDetective {
namespace Core {

	//internal representation of image
	CONCRETE SEALED NONCOPYABLE class ImgInfo {
	public:
		static ImgInfo* Create(const REF ImgShortInfo& shortInfo);
	private:
		CTOR ImgInfo(const REF Magick::Image& magickImg);
		COPYCTOR ImgInfo(const REF ImgInfo& other);
		
		EXCLUSIVE Magick::Image magickImg;
	};

}
}