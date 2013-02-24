#include <boost/test/unit_test.hpp>
#include "common/CommonInternal.h"

using namespace ImgDetective::Core;

BOOST_AUTO_TEST_SUITE(common_tests)

BOOST_AUTO_TEST_CASE(convert_to_search_result) {
    const int testArraySize = 10;
    imgid_col_t imgIds;
    
    for (int i = 0; i < testArraySize; ++i) {
        imgIds.push_back(i);
    }

    SearchResult result;
    result.arraySize = 0;
    result.items = NULL;
    result.opStatus = OPSTATUS_OK;

    ConvertToSearchResult(REF imgIds, REF result);

    BOOST_ASSERT(result.arraySize == testArraySize);
    bool ok = true;
    
    for (int i = 0; i < testArraySize; ++i) {
        if (result.items[i] != i) {
            ok = false;
            break;
        }
    }

    delete[] result.items;

    BOOST_CHECK(ok);
}

BOOST_AUTO_TEST_SUITE_END();