#define BOOST_TEST_MODULE ImgDetectiveUnitTests
#include <boost/test/unit_test.hpp>
#include <cmath>
#include <ctime>

#include "modules/cld/Feature.h"
#include "modules/cld/FeatureExtractor.h"
#include "utils/MemoryUtils.h"
#include "testutils/TestUtils.h"


using namespace ImgDetective::Modules::CLD;
using namespace ImgDetective::Core;
using namespace std;

BOOST_AUTO_TEST_SUITE(colorhistogram_serialization)

BOOST_AUTO_TEST_CASE(colorhistogram_serializes_to_something) {
    ImgInfo* img = TestUtils::ReadImgFromFile("resources\\tests\\cld_test.jpg");

    CLDFE fe;
    IFeature* feat = fe.ExtractFrom(*img);

    blob_p_t blob = feat->Serialize();

    BOOST_ASSERT(blob != NULL);
    BOOST_CHECK(blob->size() != 0);

    ImgDetective::Core::SafeFreeBlob(blob);
    ImgDetective::Utils::Memory::SafeDelete(feat);
    ImgDetective::Utils::Memory::SafeDelete(img);
}

BOOST_AUTO_TEST_CASE(deserialized_histogram_is_identical_to_initial) {
    ImgInfo* img = TestUtils::ReadImgFromFile("resources\\tests\\jetta.jpg");

    CLDFE fe;
    IFeature* feat = fe.ExtractFrom(*img);

    blob_p_t blob = feat->Serialize();

    BOOST_ASSERT(blob != NULL);

    IFeature* deserializedFeat = CLDFeat::Deserialize(*blob);

    BOOST_ASSERT(deserializedFeat != NULL);

    ImgDetective::Core::SafeFreeBlob(blob);
    
    FeatureDistance distance = feat->ComputeDistanceTo(*deserializedFeat);

    BOOST_CHECK(abs(distance.GetValue()) < 0.0001);

    ImgDetective::Utils::Memory::SafeDelete(feat);
    ImgDetective::Utils::Memory::SafeDelete(deserializedFeat);
    ImgDetective::Utils::Memory::SafeDelete(img);
}

BOOST_AUTO_TEST_SUITE_END()