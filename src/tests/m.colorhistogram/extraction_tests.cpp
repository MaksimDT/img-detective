#include <boost/test/unit_test.hpp>
#include <cmath>

#include "testutils/TestUtils.h"
#include "utils/MemoryUtils.h"
#include "modules/colorhistogram/FeatureExtractor.h"

using namespace ImgDetective;

BOOST_AUTO_TEST_SUITE(colorhistogram_extraction_and_similarity_matching)

BOOST_AUTO_TEST_CASE(extract_histograms_from_the_same_images_and_compare_them) {
    Core::ImgInfo* img = TestUtils::ReadImgFromFile("resources\\tests\\jetta.jpg");

    Modules::ColorHistogram::ColorHistogramFE fe;
    Core::IFeature* hist1 = fe.ExtractFrom(*img);

    BOOST_ASSERT(hist1);

    Core::IFeature* hist2 = fe.ExtractFrom(*img);

    BOOST_ASSERT(hist2);

    Core::FeatureDistance distance = hist1->ComputeDistanceTo(*hist2);

    BOOST_CHECK(abs(distance.GetValue()) < 0.0001);

    Utils::Memory::SafeDelete(img);
    Utils::Memory::SafeDelete(hist1);
    Utils::Memory::SafeDelete(hist2);
}

BOOST_AUTO_TEST_SUITE_END()