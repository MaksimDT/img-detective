#pragma once

#include "ImgDetectiveApi.h"

namespace ImgDetective {
namespace Core {

	//internal representation of image
	class ImgInfo {
	public:
		static ImgInfo* Create(ImgShortInfo& shortInfo);

		void SetInternalId(imgid_t id);
		imgid_t GetInternalId() const;
		bool HasInternalId() const;
	private:
		ImgInfo();
		ImgInfo(const ImgInfo& other) {};
		imgid_t internalId;
	};

}
}