#pragma once

#include "common/CommonInternal.h"
#include "core/Feature.h"

namespace ImgDetective {
namespace Core {

	INTERFACE class IFeatureRepository {
	public:
		virtual ~IFeatureRepository() {};

		virtual imgid_col_t GetSimilarImgs(const REF Feature::col_p_t& exampleFeatSet, const REF ImgQuery& initialQuery) const = 0;
	};

}
}