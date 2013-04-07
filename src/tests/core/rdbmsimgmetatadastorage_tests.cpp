#include <boost/test/unit_test.hpp>
#include "core/MySqlDbWrapper.h"
#include "core/RDBMSImgMetadataStorage.h"
#include "tests/core/TestUtils.h"
#include "utils/MemoryUtils.h"

using namespace ImgDetective;

BOOST_AUTO_TEST_SUITE(rdbmsimgmetadatastorage_tests)

BOOST_AUTO_TEST_CASE(initimgrecord) {
    /*Core::MySqlDbWrapper::ConnectionSettings conSettings;
    conSettings.dbName = "img_detective";
    conSettings.host = "localhost";
    conSettings.login = "root";
    conSettings.password = "";
    conSettings.port = 3306;
    
    Core::MySqlDbWrapper dbWrapper(conSettings);

    Core::RDBMSImgMetadataStorage storage(REF dbWrapper);

    Core::ImgInfo* img = TestUtils::ReadImgFromFile("resources\\tests\\jetta.jpg");
    
    BOOST_ASSERT(img->GetId() == IMGID_UNDEFINED);

    storage.InitImgRecord(*img);

    BOOST_ASSERT(img->GetId() != IMGID_UNDEFINED);

    Utils::Memory::SafeDelete(img);*/
}

BOOST_AUTO_TEST_SUITE_END()