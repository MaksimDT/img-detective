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

void UploadImg(RawImg rawImg, UploadImgResult* result) {
    memset(result, 0, sizeof(UploadImgResult));

    Core::Db::MySqlConnectionSettings dbConSettings;
    dbConSettings.dbName = "img_detective";
    dbConSettings.host = "localhost";
    dbConSettings.login = "root";
    dbConSettings.password = "";
    dbConSettings.port = 3306;

    Core::IFeatureExtractor::col_p_t featExtractors;
    Core::IIndexManager::col_p_t indexManagers;

    try {
        boost::filesystem::path uploadDirPath = L"H:\\Институт\\Диплом\\img-detective\\upload";
        Core::FsImgStorage fsImgStorage(uploadDirPath);
        Core::Db::MySqlDbWrapper dbWrapper(dbConSettings);
        Core::RDBMSImgMetadataStorage imgMetadataStorage(dbWrapper);
        featExtractors.push_back(Modules::ColorHistogram::ModuleFactory::GetFeatureExtractor());
        indexManagers.push_back(Modules::ColorHistogram::ModuleFactory::GetIndexManager(dbWrapper));
        Core::FeatureRepository featureRepo(indexManagers);
        Core::Indexer indexer(fsImgStorage, imgMetadataStorage, featExtractors, featureRepo);

        Core::ImgInfo* parsedImg = Core::ImgInfo::Create(rawImg);

        indexer.UploadImg(*parsedImg);

        result->opStatus = OPSTATUS_OK;
    }
    catch (...) {
        //TODO: logging
        result->opStatus = OPSTATUS_INTERNAL_ERROR;
    }
}

IndexDirectoryResult IndexDirectory(wchar_t* dirPath) {
    return IndexDirectoryResult();
}

IndexDirectoryResult IndexDirectoryStub(wchar_t* dirPath) {
    IndexDirectoryResult result;
    result.opStatus = OPSTATUS_OK;
    return result;
}