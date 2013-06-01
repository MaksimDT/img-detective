#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>

#include "core/MySqlDbWrapper.h"
#include "core/FsImgStorage.h"
#include "core/RDBMSImgMetadataStorage.h"
#include "modules/colorhistogram/ModuleFactory.h"
#include "core/FeatureRepository.h"
#include "core/SearchSystem.h"
#include "testutils/TestUtils.h"
#include "utils/MemoryUtils.h"
#include "utils/FileSystemUtils.h"

using namespace ImgDetective;

BOOST_AUTO_TEST_SUITE(indexing_tests)

BOOST_AUTO_TEST_CASE(search_throws_no_exceptions_test) {
    Core::Db::DbWrapper* dbWrapper = TestUtils::CreateDbWrapper();
    Core::IFeatureExtractor::col_p_t featExtractors;
    featExtractors.push_back(Modules::ColorHistogram::ModuleFactory::GetFeatureExtractor());
    Core::IIndexManager::col_p_t indexManagers;
    indexManagers.push_back(Modules::ColorHistogram::ModuleFactory::GetIndexManager(*dbWrapper));
    Core::FeatureRepository featureRepo(indexManagers);

    Core::SearchSystem ss(featureRepo, featExtractors);

    ImgQuery query;
    memset(&query, 0, sizeof(ImgQuery));
    query.tolerance = 0.5;

    Core::ImgInfo* img = TestUtils::ReadImgFromFile("resources\\tests\\field.JPG");

    Core::SearchResultInternal* searchResultInternal = ss.GetSimilarImgs(*img, query);

    Utils::Memory::SafeDelete(dbWrapper);
    Utils::Memory::SafeDeleteCollectionOfPointers(featExtractors);
    Utils::Memory::SafeDeleteCollectionOfPointers(indexManagers);
    Utils::Memory::SafeDelete(img);
    Utils::Memory::SafeDelete(searchResultInternal);
}

BOOST_AUTO_TEST_SUITE_END()