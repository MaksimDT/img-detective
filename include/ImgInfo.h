#pragma once

#include "ImgDetectiveApi.h"
#include "CommonInternal.h"

namespace ImgDetective {
namespace Core {

	//internal representation of image
	CONCRETE SEALED class ImgInfo {
	public:
		static ImgInfo* Create(ImgShortInfo& shortInfo);
	private:
		ImgInfo();
	};

}
}