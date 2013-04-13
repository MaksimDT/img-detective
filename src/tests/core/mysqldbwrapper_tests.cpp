#include <boost/test/unit_test.hpp>
#include "core/MySqlDbWrapper.h"
#include "utils/MemoryUtils.h"

using namespace ImgDetective;
using namespace ImgDetective::Core;
using namespace ImgDetective::Core::Db;

BOOST_AUTO_TEST_SUITE(mysqldbwrapper_tests)

BOOST_AUTO_TEST_CASE(select_from_images) {

    MySqlConnectionSettings settings;
    settings.dbName = "img_detective";
    settings.host = "localhost";
    settings.login = "root";
    settings.password = "";
    settings.port = 3306;

    DbWrapper* dbWrapper = new MySqlDbWrapper(settings);

    DbResultReader* reader = dbWrapper->ExecuteReader("select Id from Images", params_list_t());
    DbResultReader& readerRef = *reader;

    imgid_t imgId;

    while (reader->Next()) {
        BOOST_CHECK(reader->HasField("Id"));
        imgId = readerRef["Id"].As<imgid_t>();
    }

    Utils::Memory::SafeDelete(dbWrapper);
    Utils::Memory::SafeDelete(reader);
}

BOOST_AUTO_TEST_SUITE_END()