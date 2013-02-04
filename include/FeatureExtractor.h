#pragma once

#include "Feature.h"
#include "ImgDetectiveApi.h"
#include "ImgInfo.h"
#include <vector>

namespace ImgDetective {
namespace Core {

	class FeatureExtractor {
	public:
		typedef std::vector<FeatureExtractor*> col_p_t;

		virtual Feature* ExtractFrom(ImgInfo& imgInfo) = 0;
	};
}
}