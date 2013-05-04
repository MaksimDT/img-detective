#include <boost/test/unit_test.hpp>
#include "utils/FileSystemUtils.h"

using namespace ImgDetective;
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

    BOOST_CHECK_THROW( 
        blob = ImgDetective::Utils::FileSystem::ReadAllBytes("resources\\tests\\idontexist.jpg"),
        ImgDetective::Utils::FileNotFoundException);

    SafeFreeBlob(blob);
}

BOOST_AUTO_TEST_CASE(check_parent_child_relationship_true_both_windows_native_format) {
    boost::filesystem::path parent(L"H:\\Институт\\Диплом");
    boost::filesystem::path child(L"H:\\Институт\\Диплом\\img-detective\\include\\core");

    BOOST_CHECK(Utils::FileSystem::HasParentChildRelationship(parent, child));
}

BOOST_AUTO_TEST_CASE(check_parent_child_relationship_true_mixed_formats) {
    boost::filesystem::path parent(L"H:\\Институт\\Диплом");
    boost::filesystem::path child(L"H:/Институт/Диплом/img-detective/include/core");

    BOOST_CHECK(Utils::FileSystem::HasParentChildRelationship(parent, child));
}

BOOST_AUTO_TEST_CASE(check_parent_child_relationship_true_slash_at_the_end_of_parent) {
    boost::filesystem::path parent(L"H:\\Институт\\Диплом\\");
    boost::filesystem::path child(L"H:\\Институт\\Диплом\\img-detective\\include\\core");

    BOOST_CHECK(Utils::FileSystem::HasParentChildRelationship(parent, child));
}

BOOST_AUTO_TEST_CASE(check_parent_child_relationship_false_windows_different_drives) {
    boost::filesystem::path parent(L"G:\\Институт\\Диплом");
    boost::filesystem::path child(L"H:\\Институт\\Диплом\\img-detective\\include\\core");

    BOOST_CHECK(!Utils::FileSystem::HasParentChildRelationship(parent, child));
}

BOOST_AUTO_TEST_CASE(check_parent_child_relationship_false_same_paths) {
    boost::filesystem::path parent(L"H:\\Институт\\Диплом\\");
    boost::filesystem::path child(L"H:\\Институт\\Диплом");

    BOOST_CHECK(!Utils::FileSystem::HasParentChildRelationship(parent, child));
}

BOOST_AUTO_TEST_CASE(make_relative) {
    boost::filesystem::path parent(L"H:\\Институт\\Диплом\\");
    boost::filesystem::path child(L"H:\\Институт\\Диплом\\img-detective\\include\\core");

    boost::filesystem::path relativeChild = Utils::FileSystem::MakeRelative(parent, child);

    BOOST_CHECK(relativeChild == L"img-detective\\include\\core");
}

BOOST_AUTO_TEST_SUITE_END()