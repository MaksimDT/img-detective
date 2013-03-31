#include <boost/test/unit_test.hpp>
#include "utils/FileSystemUtils.h"

using namespace ImgDetective::Core;

BOOST_AUTO_TEST_SUITE(filesystem_utils_tests)

BOOST_AUTO_TEST_CASE(read_all_bytes) {
    blob_p_t blob = ImgDetective::Utils::FileSystem::ReadAllBytes("resources\\tests\\jetta.jpg");

    BOOST_ASSERT(blob != NULL);
    BOOST_CHECK(blob->size() != 0);

    SafeFreeBlob(blob);
}

BOOST_AUTO_TEST_CASE(read_all_bytes_non_existing_file) {
    blob_p_t blob = NULL;

    try {
        blob = ImgDetective::Utils::FileSystem::ReadAllBytes("resources\\tests\\idontexist.jpg");

        BOOST_ASSERT(blob != NULL);
        BOOST_CHECK(blob->size() != 0);

        SafeFreeBlob(blob);
    }
    catch (...) {
        SafeFreeBlob(blob);
    }
}

BOOST_AUTO_TEST_SUITE_END()