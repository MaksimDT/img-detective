#include <boost/test/unit_test.hpp>
#include "core/MySqlDbWrapper.h"
#include "utils/MemoryUtils.h"
#include "testutils/TestUtils.h"

using namespace ImgDetective;
using namespace ImgDetective::Core;
using namespace ImgDetective::Core::Db;

BOOST_AUTO_TEST_SUITE(mysqldbwrapper_tests)

BOOST_AUTO_TEST_CASE(insert_then_select_from_images) {
    DbWrapper* dbWrapper = TestUtils::CreateMySqlDbWrapper();

    NonQueryExecResult insertResult = dbWrapper->ExecuteNonQuery(
        "insert into Images (`Path`, `CreationDate`, `Description`) \
         values \
         (NULL, NOW(), NULL)");
    imgid_t insertedImgId = insertResult.GetLastInsertId();
    BOOST_ASSERT(insertedImgId != 0);

    DbResultReader* reader = dbWrapper->ExecuteReader("select Id from Images order by Id desc");
    BOOST_ASSERT(reader != NULL);
    DbResultReader& readerRef = *reader;

    BOOST_ASSERT(reader->Next() == true);
    BOOST_ASSERT(reader->HasField("Id"));

    imgid_t imgId = readerRef["Id"].As<imgid_t>();

    BOOST_CHECK(imgId == insertedImgId);

    Utils::Memory::SafeDelete(dbWrapper);
    Utils::Memory::SafeDelete(reader);
}

BOOST_AUTO_TEST_SUITE_END()