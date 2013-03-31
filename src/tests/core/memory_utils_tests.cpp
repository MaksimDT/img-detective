#include <boost/test/unit_test.hpp>
#include "utils/MemoryUtils.h"

using namespace ImgDetective::Utils;

BOOST_AUTO_TEST_SUITE(memory_utils_tests)

BOOST_AUTO_TEST_CASE(delete_single_object) {
    int* oneObject = new int;

    Memory::SafeDelete(oneObject);

    BOOST_CHECK(oneObject == NULL);
}

BOOST_AUTO_TEST_CASE(delete_array_of_objects) {
    int* ar = new int[1000];

    Memory::SafeDeleteArray(ar);

    BOOST_CHECK(ar == NULL);
}

BOOST_AUTO_TEST_CASE(delete_collection_of_pointers) {
    std::vector<int*> v;

    for (int i = 0; i < 10; ++i) {
        v.push_back(new int);
    }

    Memory::SafeDeleteCollectionOfPointers(v);

    BOOST_CHECK(v.empty());
}


BOOST_AUTO_TEST_SUITE_END()