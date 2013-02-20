#pragma once

#include "ImgDetectiveApi.h"
#include "Feature.h"
#include "CommonInternal.h"

namespace ImgDetective {
namespace Core {

	class IFeatureRepository {
	public:
		virtual imgid_col_t GetSimilarImgs(Feature::col_p_t exampleFeatSet, ImgQuery initialQuery) const = 0;
	};

}
}