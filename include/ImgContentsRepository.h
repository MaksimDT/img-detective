#pragma once

#include "ImgInfo.h"
#include "CommonInternal.h"

namespace ImgDetective {
namespace Core {

	class ImgContentsRepository {
	public:
		virtual bool SearchForImage(const ImgInfo& img, OUT imgid_t& imgid, OUT bool& indexed) = 0;
	};

}
}