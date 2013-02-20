#include "ImgDetectiveApi.h"
#include <SearchSystem.h>

using namespace ImgDetective;

SearchResult SearchByExample(ImgQuery query) {
	try {
		Core::IFeatureRepository* featureRepo = NULL;
		Core::FeatureExtractor::col_p_t featureExtractors;

		Core::SearchSystem ss(featureRepo, featureExtractors);

		imgid_col_t result = ss.GetSimilarImgs(query);

		//result->opStatus = OPSTATUS_INTERNAL_ERROR;
		//return result;
	}
	catch (std::exception ex) {
		
	}
}

UploadImgResult UploadImg(ImgShortInfo imgInfo) {
	UploadImgResult result;
	result.opStatus = OPSTATUS_INTERNAL_ERROR;
	return result;
}