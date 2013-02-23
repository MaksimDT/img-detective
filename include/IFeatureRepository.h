#pragma once

#include "ImgDetectiveApi.h"
#include "Feature.h"
#include "CommonInternal.h"

namespace ImgDetective {
namespace Core {

	INTERFACE class IFeatureRepository {
	public:
		virtual imgid_col_t GetSimilarImgs(const REF Feature::col_p_t& exampleFeatSet, const REF ImgQuery& initialQuery) const = 0;
	protected:
		virtual ~IFeatureRepository() {};
	};

}
}