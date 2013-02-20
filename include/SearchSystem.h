#pragma once

#include "FeatureExtractor.h"
#include "FeatureRepositoryBase.h"
#include "ImgDetectiveApi.h"
#include "CommonInternal.h"
#include "ImgContentsRepository.h"
#include <vector>

namespace ImgDetective {
namespace Core {

	/*facade class for the system. Handles multithreading and orchestrates work 
	of other subsystems*/
	class SearchSystem {
	public:
		SearchSystem(FeatureRepositoryBase* featureRepo, FeatureExtractor::col_p_t featureExtractors);
		~SearchSystem();
		imgid_col_t GetSimilarImgs(ImgQuery query);
	private:
		Feature::col_p_t ExtractFeatures(ImgInfo& imgInfo, ImgQuery query);

		FeatureRepositoryBase* featureRepo;
		FeatureExtractor::col_p_t featureExtractors;
	};

}
}