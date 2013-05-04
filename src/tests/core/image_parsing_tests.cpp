#include <boost/test/unit_test.hpp>
#include "utils/FileSystemUtils.h"
#include "utils/MemoryUtils.h"
#include "core/ImgInfo.h"

using namespace ImgDetective;

BOOST_AUTO_TEST_SUITE(image_parsing_tests)

BOOST_AUTO_TEST_CASE(parse_jpeg_test_from_rawimg) {
    Core::ImgInfo* parsedImg = NULL;
    Core::blob_p_t blob = NULL;

    blob = Utils::FileSystem::ReadAllBytes("resources\\tests\\jetta.jpg");
    BOOST_ASSERT(blob != NULL);

    RawImg rawImg;
    memset(&rawImg, 0, sizeof(rawImg));
    rawImg.content = Core::BlobToCharArray(blob);
    rawImg.contentSize = blob->size();
    rawImg.fileExtension = "jpg";

    parsedImg = Core::ImgInfo::Create(rawImg);
    BOOST_ASSERT(parsedImg != NULL);
    off_t fileSize = parsedImg->GetMagickImage().fileSize();

    unsigned int actualWidth = parsedImg->GetMagickImage().size().width();
    unsigned int actualHeight = parsedImg->GetMagickImage().size().height();

    BOOST_CHECK(actualWidth == 1600);
    BOOST_CHECK(actualHeight == 1200);

    Core::SafeFreeBlob(blob);
    Utils::Memory::SafeDelete(parsedImg);
}

BOOST_AUTO_TEST_CASE(parse_jpeg_test_from_filesystem_path) {
    Core::ImgInfo* parsedImg = Core::ImgInfo::Create(1, "resources/tests/jetta.jpg");

    unsigned int actualWidth = parsedImg->GetMagickImage().size().width();
    unsigned int actualHeight = parsedImg->GetMagickImage().size().height();

    BOOST_CHECK(actualWidth == 1600);
    BOOST_CHECK(actualHeight == 1200);

    Utils::Memory::SafeDelete(parsedImg);
}

BOOST_AUTO_TEST_SUITE_END()