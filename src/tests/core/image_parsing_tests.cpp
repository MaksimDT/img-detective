#include <boost/test/unit_test.hpp>
#include "utils/FileSystemUtils.h"
#include "utils/MemoryUtils.h"
#include "core/ImgInfo.h"

using namespace ImgDetective;

BOOST_AUTO_TEST_SUITE(image_parsing_tests)

BOOST_AUTO_TEST_CASE(parse_jpeg_test) {
    Core::ImgInfo* parsedImg = NULL;
    Core::blob_p_t blob = NULL;

    blob = Utils::FileSystem::ReadAllBytes("resources\\tests\\jetta.jpg");
    BOOST_ASSERT(blob != NULL);

    ImgShortInfo imgShortInfo;
    imgShortInfo.content = Core::BlobToCharArray(blob);
    imgShortInfo.contentSize = blob->size();

    parsedImg = Core::ImgInfo::Create(imgShortInfo);
    BOOST_ASSERT(parsedImg != NULL);
    off_t fileSize = parsedImg->GetMagickImage().fileSize();

    unsigned int actualWidth = parsedImg->GetMagickImage().size().width();
    unsigned int actualHeight = parsedImg->GetMagickImage().size().height();

    BOOST_CHECK(actualWidth == 1600);
    BOOST_CHECK(actualHeight == 1200);

    Core::SafeFreeBlob(blob);
    Utils::Memory::SafeDelete(parsedImg);
}

BOOST_AUTO_TEST_SUITE_END()