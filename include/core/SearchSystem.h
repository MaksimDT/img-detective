#pragma once

#include "common/CommonInternal.h"
#include "core/FeatureExtractor.h"
#include "core/IFeatureRepository.h"

#include <vector>

namespace ImgDetective {
namespace Core {

	/*facade class for the system. Handles multithreading and orchestrates work 
	of other subsystems*/
	CONCRETE SEALED class SearchSystem {
	public:
		SearchSystem(IFeatureRepository* featureRepo, FeatureExtractor::col_p_t featureExtractors);
		virtual ~SearchSystem();
		imgid_col_t GetSimilarImgs(ImgQuery query);
	private:
		Feature::col_p_t ExtractFeatures(ImgInfo& imgInfo, ImgQuery query);

		IFeatureRepository* featureRepo;
		FeatureExtractor::col_p_t featureExtractors;
	};

}
}