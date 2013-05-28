#include <boost/test/unit_test.hpp>
#include <cmath>

#include "testutils/TestUtils.h"
#include "utils/MemoryUtils.h"
#include "modules/ehd/FeatureExtractor.h"

using namespace ImgDetective;

BOOST_AUTO_TEST_SUITE(ehd_extraction_and_similarity_matching)

BOOST_AUTO_TEST_CASE(extract_features_from_the_same_images_and_compare_them) {
    Core::ImgInfo* img = TestUtils::ReadImgFromFile("resources\\tests\\cld_test.jpg");

    Modules::EHD::EHDFE fe;
    Core::IFeature* hist1 = fe.ExtractFrom(*img);

    BOOST_ASSERT(hist1 != NULL);

    Core::IFeature* hist2 = fe.ExtractFrom(*img);

    BOOST_ASSERT(hist2 != NULL);

    Core::FeatureDistance distance = hist1->ComputeDistanceTo(*hist2);

    BOOST_CHECK(distance.GetValue() >= 0);
    BOOST_CHECK(distance.GetValue() <= 1);
    BOOST_CHECK(distance.GetValue() < 0.0001);

    Utils::Memory::SafeDelete(img);
    Utils::Memory::SafeDelete(hist1);
    Utils::Memory::SafeDelete(hist2);
}

BOOST_AUTO_TEST_CASE(compare_absolute_white_and_black_images_features) {
    Modules::EHD::EHDFE fe;

    Core::ImgInfo* black = TestUtils::ReadImgFromFile("resources\\tests\\absolute_black.jpg");
    Core::ImgInfo* white = TestUtils::ReadImgFromFile("resources\\tests\\absolute_white.jpg");

    Core::IFeature* blackHist = fe.ExtractFrom(*black);

    BOOST_ASSERT(blackHist != NULL);

    Core::IFeature* whiteHist = fe.ExtractFrom(*white);

    BOOST_ASSERT(whiteHist != NULL);

    Core::FeatureDistance distance = blackHist->ComputeDistanceTo(*whiteHist);

    BOOST_CHECK(distance.GetValue() >= 0);
    BOOST_CHECK(distance.GetValue() <= 1);
    BOOST_CHECK(distance.GetValue() <= 0.001);

    Utils::Memory::SafeDelete(black);
    Utils::Memory::SafeDelete(white);
    Utils::Memory::SafeDelete(blackHist);
    Utils::Memory::SafeDelete(whiteHist);
}

BOOST_AUTO_TEST_SUITE_END()