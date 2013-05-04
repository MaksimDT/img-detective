#define BOOST_TEST_MODULE ImgDetectiveIntegrationTests
#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>

#include "core/MySqlDbWrapper.h"
#include "core/FsImgStorage.h"
#include "core/RDBMSImgMetadataStorage.h"
#include "modules/colorhistogram/ModuleFactory.h"
#include "core/FeatureRepository.h"
#include "core/Indexer.h"
#include "testutils/TestUtils.h"
#include "utils/MemoryUtils.h"

using namespace ImgDetective;

BOOST_AUTO_TEST_SUITE(indexing_tests)

BOOST_AUTO_TEST_CASE(uploadimg_test) {
    //setup all the modules and test data required for the test

    boost::filesystem::path uploadDirPath = L"H:\\Институт\\Диплом\\img-detective\\upload_test";
    Core::FsImgStorage fsImgStorage(uploadDirPath);
    Core::Db::DbWrapper* dbWrapper = TestUtils::CreateDbWrapper();
    Core::RDBMSImgMetadataStorage imgMetadataStorage(*dbWrapper);
    Core::IFeatureExtractor::col_p_t featExtractors;
    featExtractors.push_back(Modules::ColorHistogram::ModuleFactory::GetFeatureExtractor());
    Core::IIndexManager::col_p_t indexManagers;
    indexManagers.push_back(Modules::ColorHistogram::ModuleFactory::GetIndexManager(*dbWrapper));
    Core::FeatureRepository featureRepo(indexManagers);
    Core::Indexer indexer(fsImgStorage, imgMetadataStorage, featExtractors, featureRepo);
    Core::ImgInfo* img = TestUtils::ReadImgFromFile("resources\\tests\\jetta.jpg");

    //run test
    indexer.UploadImg(*img);

    //ensure that the record in metadata db was added
    Core::Db::DbResultReader* imagesReader = dbWrapper->ExecuteReader(
        "SELECT \
            Id, Path, Extension \
         FROM \
            Images \
         ORDER BY \
            Id DESC \
         LIMIT 1");

    BOOST_ASSERT(imagesReader->Next());
    BOOST_ASSERT(imagesReader->HasField("Id"));
    BOOST_ASSERT(imagesReader->HasField("Extension"));
    BOOST_ASSERT(imagesReader->HasField("Path"));

    //ensure that the valid id, path and extension were assigned to the added image
    imgid_t imgId = imagesReader->GetField("Id").As<imgid_t>();
    std::wstring path = imagesReader->GetField("Path").FromUtf8();
    std::string extension = imagesReader->GetField("Extension").As<std::string>();
    
    BOOST_CHECK(imgId != IMGID_UNDEFINED);
    BOOST_CHECK(path == L"");
    BOOST_CHECK(extension == "jpg");

    //ensure that image content was written to filesystem
    boost::filesystem::path expectedFilePath = fsImgStorage.GetImgPath(imgId, extension);
    BOOST_CHECK(boost::filesystem::exists(expectedFilePath));

    //ensure that features of image were extracted and stored in persistent storage
    Core::Db::params_list_t colorHistParams;
    Core::Db::DbParamBuffer imgIdParam(&imgId, sizeof(imgId), Core::Db::DbType::LONGLONG);
    colorHistParams.push_back(imgIdParam);
    Core::Db::DbResultReader* colorHistReader = dbWrapper->ExecuteReader(
        "SELECT \
            Data \
         FROM \
            ColorHistograms \
         WHERE \
            ImageId = ?", colorHistParams);

    BOOST_ASSERT(colorHistReader->Next());
    BOOST_ASSERT(colorHistReader->HasField("Data"));
    Core::blob_p_t colorHist = colorHistReader->GetField("Data").CopyToByteArray();
    BOOST_ASSERT(colorHist != NULL);
    BOOST_CHECK(colorHist->size() != 0);

    Utils::Memory::SafeDelete(dbWrapper);
    Utils::Memory::SafeDelete(img);
    Utils::Memory::SafeDelete(imagesReader);
    Utils::Memory::SafeDelete(colorHistReader);
    Utils::Memory::SafeDelete(colorHist);
}

BOOST_AUTO_TEST_SUITE_END()