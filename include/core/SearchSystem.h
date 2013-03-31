#pragma once

#include "common/CommonInternal.h"
#include "core/FeatureExtractor.h"
#include "core/FeatureRepository.h"

#include <vector>

namespace ImgDetective {
namespace Core {

	/*facade class for the system. Handles multithreading and orchestrates work 
	of other subsystems*/
	CONCRETE SEALED class SearchSystem : NONCOPYABLE {
	public:
		SearchSystem(IFeatureRepository* featureRepo, FeatureExtractor::col_p_t featureExtractors);
		virtual ~SearchSystem();
		imgid_col_t GetSimilarImgs(ImgQuery query);
	private:
		IFeature::col_p_t ExtractFeatures(ImgInfo& imgInfo, ImgQuery query);

		IFeatureRepository* featureRepo;
		FeatureExtractor::col_p_t featureExtractors;
	};

}
}