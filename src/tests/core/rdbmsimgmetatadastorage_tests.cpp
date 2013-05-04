#include <boost/test/unit_test.hpp>
#include "core/MySqlDbWrapper.h"
#include "core/RDBMSImgMetadataStorage.h"
#include "testutils/TestUtils.h"
#include "utils/MemoryUtils.h"

using namespace ImgDetective;
using namespace ImgDetective::Core;
using namespace ImgDetective::Core::Db;

using namespace std;

BOOST_AUTO_TEST_SUITE(rdbmsimgmetadatastorage_tests)

BOOST_AUTO_TEST_CASE(initimgrecord) {
    DbWrapper* dbWrapper = TestUtils::CreateDbWrapper();

    Core::RDBMSImgMetadataStorage storage(*dbWrapper);

    Core::ImgInfo* img = TestUtils::ReadImgFromFile("resources\\tests\\jetta.jpg");
    
    BOOST_ASSERT(img->GetId() == IMGID_UNDEFINED);

    try {
        storage.InitImgRecord(*img);
    }
    catch (std::exception ex) {
        throw;
    }

    BOOST_ASSERT(img->GetId() != IMGID_UNDEFINED);

    Utils::Memory::SafeDelete(img);
    Utils::Memory::SafeDelete(dbWrapper);
}

BOOST_AUTO_TEST_CASE(initimgrecord_with_path) {
    DbWrapper* dbWrapper = TestUtils::CreateDbWrapper();

    Core::RDBMSImgMetadataStorage storage(*dbWrapper);

    Core::ImgInfo* img = TestUtils::ReadImgFromFile("resources\\tests\\jetta.jpg");
    img->SetPath("H:\\img_detective\\upload");
    
    BOOST_ASSERT(img->GetId() == IMGID_UNDEFINED);

    storage.InitImgRecord(*img);

    BOOST_ASSERT(img->GetId() != IMGID_UNDEFINED);

    imgid_t imgId = img->GetId();

    params_list_t params;
    params.push_back(DbParamBuffer(&imgId, sizeof(imgId), DbType::LONGLONG));

    DbResultReader* reader = dbWrapper->ExecuteReader("select Path from Images where Id = ?", params);

    BOOST_ASSERT(reader->Next());
    BOOST_ASSERT(reader->HasField("Path"));
    
    string path = reader->GetField("Path").As<string>();

    BOOST_CHECK(path == img->GetPath());

    Utils::Memory::SafeDelete(reader);
    Utils::Memory::SafeDelete(img);
    Utils::Memory::SafeDelete(dbWrapper);
}

BOOST_AUTO_TEST_CASE(createfsrepositoryrecord_and_getallrepositories_smoketest) {
    DbWrapper* dbWrapper = TestUtils::CreateDbWrapper();

    Core::RDBMSImgMetadataStorage storage(*dbWrapper);

    Core::FsRepositoryInfo newRepo = storage.CreateFsRepositoryRecord(L"H:\\фотографии");

    Core::FsRepositoryInfo::col_t allRepos = storage.GetAllRepositories();

    bool found = false;
    Core::FsRepositoryInfo::col_t::const_iterator it;
    
    for (it = allRepos.cbegin(); it != allRepos.cend(); ++it) {
        if (it->GetId() == newRepo.GetId() && it->GetPath() == newRepo.GetPath()) {
            found = true;
        }
    }

    BOOST_CHECK(found);

    Utils::Memory::SafeDelete(dbWrapper);
}

BOOST_AUTO_TEST_SUITE_END()