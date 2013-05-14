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
#include "modules/cld/ModuleFactory.h"

#include <Windows.h>

using namespace ImgDetective;

#pragma region internal functions prototypes

void InitVars();

#pragma endregion

#pragma region static variables

Core::Indexer* indexer = NULL;
Core::ImgContentStorage* imgContentStorage = NULL;
Core::Db::DbWrapper* dbWrapper = NULL;
Core::ImgMetadataStorage* imgMetadataStorage = NULL;
Core::IFeatureExtractor::col_p_t featExtractors;
Core::IIndexManager::col_p_t indexManagers;
Core::IFeatureRepository* featureRepo = NULL;
Core::SearchSystem* searchSystem = NULL;

#pragma endregion

#pragma region internal functions impl

void InitVars() {
    Core::Db::MySqlConnectionSettings dbConSettings;
    dbConSettings.dbName = "img_detective";
    dbConSettings.host = "localhost";
    dbConSettings.login = "root";
    dbConSettings.password = "";
    dbConSettings.port = 3306;

    try {
        boost::filesystem::path uploadDirPath = L"H:\\Институт\\Диплом\\img-detective\\upload";
        imgContentStorage = new Core::FsImgStorage(uploadDirPath);
        dbWrapper = new Core::Db::MySqlDbWrapper(dbConSettings);
        imgMetadataStorage = new Core::RDBMSImgMetadataStorage(*dbWrapper);
        /*featExtractors.push_back(Modules::ColorHistogram::ModuleFactory::GetFeatureExtractor());
        indexManagers.push_back(Modules::ColorHistogram::ModuleFactory::GetIndexManager(*dbWrapper));*/
        featExtractors.push_back(Modules::CLD::ModuleFactory::GetFeatureExtractor());
        indexManagers.push_back(Modules::CLD::ModuleFactory::GetIndexManager(*dbWrapper));
        featureRepo = new Core::FeatureRepository(indexManagers);
        indexer = new Core::Indexer(*imgContentStorage, *imgMetadataStorage, featExtractors, *featureRepo);
        searchSystem = new Core::SearchSystem(*featureRepo, featExtractors);
    }
    catch (...) {
        exit(-1);
    }
}

#pragma endregion

BOOL WINAPI DllMain(HINSTANCE hInstDll, DWORD fdwReason, PVOID fImpLoad) {

    switch (fdwReason) {
        case DLL_PROCESS_ATTACH:
            InitVars();
            break;
        case DLL_THREAD_ATTACH:
            // A thread is being created.
            break;
        case DLL_THREAD_DETACH:
            // A thread is exiting cleanly.
            break;
        case DLL_PROCESS_DETACH:
            // The DLL is being unmapped from the process' address space.
            break;
    }
    return(TRUE); // Used only for DLL_PROCESS_ATTACH
}

void Search1(ImgQuery query) {
    char* content = query.exampleContent;
}

void Search(ImgQuery query, SearchResult* result) {
    if (result == NULL) {
        return;
    }

    memset(result, 0, sizeof(SearchResult));

    Core::ImgInfo* img = NULL;

	try {
        RawImg exampleRawImg;
        memset(&exampleRawImg, 0, sizeof(RawImg));
        exampleRawImg.content = query.exampleContent;
        exampleRawImg.contentSize = query.exampleContentSize;
        exampleRawImg.fileExtension = NULL;

        img = Core::ImgInfo::Create(exampleRawImg);
        Core::SearchResultInternal searchResultInternal = searchSystem->GetSimilarImgs(*img, query);

        SearchResult convertedResult = searchResultInternal.ConvertToSearchResult(
            [] (size_t bytesCount) {
                return (void*)CoTaskMemAlloc(bytesCount);
            },
            [] (void* ptr) {
                CoTaskMemFree(ptr);
            });
        result->opStatus = OPSTATUS_OK;
        result->arraySize = convertedResult.arraySize;
        result->items = convertedResult.items;
        result->itemsRelevance = convertedResult.itemsRelevance;

        Utils::Memory::SafeDelete(img);
	}
	catch (std::exception ex) {
        Utils::Memory::SafeDelete(img);
        //TODO: logging
        if (result->items != NULL) {
            CoTaskMemFree(result->items);
            result->items = NULL;
        }

        if (result->itemsRelevance != NULL) {
            CoTaskMemFree(result->itemsRelevance);
            result->itemsRelevance = NULL;
        }

        result->arraySize = 0;
        result->opStatus = OPSTATUS_INTERNAL_ERROR;
	}
}

void UploadImg(RawImg rawImg, UploadImgResult* result) {
    if (result == NULL) {
        return;
    }

    memset(result, 0, sizeof(UploadImgResult));

    Core::ImgInfo* parsedImg = NULL;

    try {
        parsedImg = Core::ImgInfo::Create(rawImg);
        indexer->UploadImg(*parsedImg);
        result->opStatus = OPSTATUS_OK;

        Utils::Memory::SafeDelete(parsedImg);
    }
    catch (...) {
        Utils::Memory::SafeDelete(parsedImg);
        //TODO: logging
        result->opStatus = OPSTATUS_INTERNAL_ERROR;
    }
}

void CanIndexDirectory(wchar_t* dirPath, CanIndexDirectoryResult* result) {
    if (result == NULL) {
        return;
    }

    memset(result, 0, sizeof(CanIndexDirectoryResult));

    try {
        result->resultCode = indexer->CanIndexDirectory(dirPath);
        result->opStatus = OPSTATUS_OK;
    }
    catch (...) {
        //TODO: logging
        result->opStatus = OPSTATUS_INTERNAL_ERROR;
    }
}

void IndexDirectory(wchar_t* dirPath, IndexDirectoryResult* result) {
    if (result == NULL) {
        return;
    }

    memset(result, 0, sizeof(IndexDirectoryResult));

    try {
        indexer->IndexDirectory(dirPath);
        result->opStatus = OPSTATUS_OK;
    }
    catch (...) {
        //TODO: logging
        result->opStatus = OPSTATUS_INTERNAL_ERROR;
    }
}