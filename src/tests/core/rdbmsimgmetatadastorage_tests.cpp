#include <boost/test/unit_test.hpp>
#include "core/MySqlDbWrapper.h"
#include "core/RDBMSImgMetadataStorage.h"
#include "tests/core/TestUtils.h"
#include "utils/MemoryUtils.h"

using namespace ImgDetective;
using namespace ImgDetective::Core;
using namespace ImgDetective::Core::Db;

using namespace std;

BOOST_AUTO_TEST_SUITE(rdbmsimgmetadatastorage_tests)

BOOST_AUTO_TEST_CASE(initimgrecord) {
    MySqlConnectionSettings conSettings;
    conSettings.dbName = "img_detective_test";
    conSettings.host = "localhost";
    conSettings.login = "root";
    conSettings.password = "";
    conSettings.port = 3306;
    
    MySqlDbWrapper dbWrapper(conSettings);

    Core::RDBMSImgMetadataStorage storage(REF dbWrapper);

    Core::ImgInfo* img = TestUtils::ReadImgFromFile("resources\\tests\\jetta.jpg");
    
    BOOST_ASSERT(img->GetId() == IMGID_UNDEFINED);

    storage.InitImgRecord(*img);

    BOOST_ASSERT(img->GetId() != IMGID_UNDEFINED);

    Utils::Memory::SafeDelete(img);
}

BOOST_AUTO_TEST_CASE(initimgrecord_then_saveimtrecord) {
    MySqlConnectionSettings conSettings;
    conSettings.dbName = "img_detective_test";
    conSettings.host = "localhost";
    conSettings.login = "root";
    conSettings.password = "";
    conSettings.port = 3306;
    
    MySqlDbWrapper dbWrapper(conSettings);

    Core::RDBMSImgMetadataStorage storage(REF dbWrapper);

    Core::ImgInfo* img = TestUtils::ReadImgFromFile("resources\\tests\\jetta.jpg");
    
    BOOST_ASSERT(img->GetId() == IMGID_UNDEFINED);

    storage.InitImgRecord(*img);

    BOOST_ASSERT(img->GetId() != IMGID_UNDEFINED);

    img->SetPath("H:\\img_detective\\upload");

    storage.SaveImgRecord(*img);

    imgid_t imgId = img->GetId();

    params_list_t params;
    params.push_back(DbParamBuffer(&imgId, sizeof(imgId), DbType::LONGLONG));

    DbResultReader* reader = dbWrapper.ExecuteReader("select Path from Images where Id = ?", params);

    BOOST_ASSERT(reader->Next());
    BOOST_ASSERT(reader->HasField("Path"));
    
    string path = reader->GetField("Path").As<string>();

    BOOST_CHECK(path == img->GetPath());

    Utils::Memory::SafeDelete(reader);
    Utils::Memory::SafeDelete(img);
}

BOOST_AUTO_TEST_SUITE_END()