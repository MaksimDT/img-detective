#pragma once

#include "ImgDetectiveApi.h"
#include "ImgSignature.h"
#include "CommonInternal.h"

namespace ImgDetective {
namespace Core {

	class ImgSignatureRepository {
	public:
		virtual bool SearchForSignature(ImgSignature* signature, OUT imgid_t& imgid)
	};

}
}