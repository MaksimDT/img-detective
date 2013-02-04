#pragma once

#include "ImgDetectiveApi.h"
#include "Feature.h"
#include "CommonInternal.h"
#include <vector>

namespace ImgDetective {
namespace Core {

	class FeatureRepository {
	public:
		virtual imgid_col_t FindSimilarImgs(Feature::col_p_t& exampleFeatSet, ImgQuery initialQuery) const = 0;
		virtual imgid_col_t FindSimilarImgs(imgid_t exampleImgId, ImgQuery initialQuery) const = 0;
	};

}
}