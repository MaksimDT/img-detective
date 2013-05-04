#include "common/CommonInternal.h"
#include "facade/ImgDetectiveApi.h"
#include "core/SearchSystem.h"
#include "core/FeatureRepository.h"
#include "utils/MemoryUtils.h"
#include "core/Indexer.h"
#include "core/FsImgStorage.h"
#include "core/RDBMSImgMetadataStorage.h"
#include "core/Indexer.h"
#include "core/MySqlDbWrapper.h"
#include "core/RDBMSIndexStorage.h"
#include "modules/colorhistogram/ModuleFactory.h"

using namespace ImgDetective;

SearchResult SearchByExample(ImgQuery query) {
	SearchResult result;
    result.opStatus = OPSTATUS_OK;
	result.arraySize = 0;
	result.items = NULL;

	try {
		Core::IFeatureRepository* featureRepo = new Core::FeatureRepository(Core::IndexManager::col_p_t());
		Core::FeatureExtractor::col_p_t featureExtractors;
		Core::SearchSystem ss(featureRepo, featureExtractors);

		Core::imgid_col_t imgIds = ss.GetSimilarImgs(query);
		Core::ConvertToSearchResult(imgIds, REF result);
	}
	catch (std::exception ex) {
		//TODO: logging
        result.opStatus = OPSTATUS_INTERNAL_ERROR;
	}

	return result;
}

UploadImgResult UploadImg(RawImg imgInfo) {
    /*

    Core::FsImgStorage imgContentStorage("img_detective_test");
    Core::RDBMSImgMetadataStorage imgMetadataStorage;
    Core::FeatureExtractor::col_p_t featureExtractors;

    Core::Indexer indexer(REF imgContentStorage, REF imgMetadataStorage, featureExtractors);*/

	UploadImgResult result;// = indexer.UploadImg(imgInfo);
	result.opStatus = OPSTATUS_INTERNAL_ERROR;
	return result;
}

IndexDirectoryResult IndexDirectory(wchar_t* dirPath) {
    //TODO: implement

    //IndexDirectoryResult result;
    //result.opStatus = OPSTATUS_OK;

    //try {

    //    Core::FsImgStorage fsImgStorage(L"H:\img-detective-upload");

    //    Core::Db::MySqlConnectionSettings conSettings;
    //    conSettings.dbName = "img_detective";
    //    conSettings.host = "localhost";
    //    conSettings.login = "root";
    //    conSettings.password = "";
    //    conSettings.port = 3306;

    //    Core::Db::MySqlDbWrapper dbWrapper(conSettings);

    //    Core::RDBMSImgMetadataStorage imgMetadataStorage(dbWrapper);

    //    Core::IFeatureExtractor::col_p_t featExtractors;
    //    featExtractors.push_back(Modules::ColorHistogram::ModuleFactory::GetFeatureExtractor());

    //    Core::IIndexManager::col_p_t indexManagers;
    //    indexManagers.push_back(Modules::ColorHistogram::ModuleFactory::GetIndexManager(dbWrapper));

    //    Core::FeatureRepository featureRepo(indexManagers);

    //    Core::Indexer indexer(fsImgStorage, imgMetadataStorage, featExtractors, featureRepo);

    //    indexer.IndexDirectory(dirPath);

    //    return result;
    //}
    //catch (...) {
    //    //TODO: logging
    //    result.opStatus = OPSTATUS_INTERNAL_ERROR;
    //    return result;
    //}

    return IndexDirectoryResult();
}

IndexDirectoryResult IndexDirectoryStub(wchar_t* dirPath) {
    IndexDirectoryResult result;
    result.opStatus = OPSTATUS_OK;
    return result;
}