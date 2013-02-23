#include "ImgDetectiveApi.h"
#include "SearchSystem.h"
#include "FeatureRepository.h"
#include "CommonInternal.h"
#include "MemoryUtils.h"

using namespace ImgDetective;

SearchResult SearchByExample(ImgQuery query) {
	SearchResult result;
	result.opStatus = OPSTATUS_INTERNAL_ERROR;
	result.arraySize = 0;
	result.items = NULL;

	try {
		Core::IFeatureRepository* featureRepo = new Core::FeatureRepository(Core::IndexManager::col_p_t());
		Core::FeatureExtractor::col_p_t featureExtractors;
		Core::SearchSystem ss(featureRepo, featureExtractors);

		Core::imgid_col_t imgIds = ss.GetSimilarImgs(query);
		Core::ConvertToSearchResult(imgIds, REF result);
		result.opStatus = OPSTATUS_OK;
	}
	catch (std::exception ex) {
		//TODO: logging
	}

	return result;
}

UploadImgResult UploadImg(ImgShortInfo imgInfo) {
	UploadImgResult result;
	result.opStatus = OPSTATUS_INTERNAL_ERROR;
	return result;
}