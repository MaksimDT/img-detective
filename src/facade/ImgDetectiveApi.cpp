#include "common/CommonInternal.h"
#include "facade/ImgDetectiveApi.h"
#include "core/SearchSystem.h"
#include "core/FeatureRepository.h"
#include "utils/MemoryUtils.h"
#include "core/Indexer.h"
#include "core/FsImgStorage.h"
#include "core/RDBMSImgMetadataStorage.h"

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
    Core::FsImgStorage imgContentStorage("img_detective_test");
    Core::RDBMSImgMetadataStorage imgMetadataStorage;
    Core::FeatureExtractor::col_p_t featureExtractors;

    Core::Indexer indexer(REF imgContentStorage, REF imgMetadataStorage, featureExtractors);

	UploadImgResult result = indexer.UploadImg(imgInfo);
	result.opStatus = OPSTATUS_INTERNAL_ERROR;
	return result;
}