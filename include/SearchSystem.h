#pragma once

#include "FeatureExtractor.h"
#include "FeatureRepository.h"
#include "ImgDetectiveApi.h"
#include "CommonInternal.h"
#include "ImgContentsRepository.h"
#include <vector>

namespace ImgDetective {
namespace Core {

	/*facade class for the system. Handles multithreading and coordinates work 
	of other subsystems*/
	class SearchSystem {
	public:
		SearchSystem(
			FeatureRepository* featureRepo, 
			ImgContentsRepository* contentsRepo,
			FeatureExtractor::col_p_t featureExtractors);

		~SearchSystem();
		imgid_col_t GetSimilarImgs(ImgQuery query);
	private:
		bool SearchForImageInIndex(ImgInfo& imgInfo);
		Feature::col_p_t ExtractFeatures(ImgInfo& imgInfo, ImgQuery query);

		FeatureRepository* featureRepo;
		ImgContentsRepository* contentsRepo;
		FeatureExtractor::col_p_t featureExtractors;
	};

}
}