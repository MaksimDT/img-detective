#define BOOST_TEST_MODULE ImgDetectiveUnitTests
#include <boost/test/unit_test.hpp>
#include "modules/colorhistogram/Feature.h"
#include "utils/MemoryUtils.h"
#include <cmath>
#include <ctime>

using namespace ImgDetective::Features;
using namespace ImgDetective::Core;
using namespace std;

ColorHistogramFeat* CreateRandomHistogram() {
    srand(time(0));

    ColorHistogramFeat::ChannelHistogram::bins_vector_t* binsR = new ColorHistogramFeat::ChannelHistogram::bins_vector_t(COLOR_HISTOGRAM_BIN_COUNT);
    ColorHistogramFeat::ChannelHistogram::bins_vector_t* binsG = new ColorHistogramFeat::ChannelHistogram::bins_vector_t(COLOR_HISTOGRAM_BIN_COUNT);
    ColorHistogramFeat::ChannelHistogram::bins_vector_t* binsB = new ColorHistogramFeat::ChannelHistogram::bins_vector_t(COLOR_HISTOGRAM_BIN_COUNT);

    for (int i = 0; i < COLOR_HISTOGRAM_BIN_COUNT; ++i) {
        (*binsR)[i] = rand() % NUMBER_OF_BIN_VALUES;
        (*binsG)[i] = rand() % NUMBER_OF_BIN_VALUES;
        (*binsB)[i] = rand() % NUMBER_OF_BIN_VALUES;
    }

    ColorHistogramFeat::ChannelHistogram* chR = new ColorHistogramFeat::ChannelHistogram(binsR);
    ColorHistogramFeat::ChannelHistogram* chG = new ColorHistogramFeat::ChannelHistogram(binsG);
    ColorHistogramFeat::ChannelHistogram* chB = new ColorHistogramFeat::ChannelHistogram(binsB);

    ColorHistogramFeat* colH = new ColorHistogramFeat(chR, chG, chB);

    return colH;
}

BOOST_AUTO_TEST_SUITE(colorhistogram_serialization)

BOOST_AUTO_TEST_CASE(colorhistogram_serializes_to_something) {
    const size_t maxBinValue = 256;
    const size_t binSize = sizeof(ColorHistogramFeat::ChannelHistogram::bin_value_t);
    const size_t numberOfChannels = 3;

    ColorHistogramFeat* colH = CreateRandomHistogram();

    blob_p_t blob = colH->Serialize();

    BOOST_ASSERT(blob != NULL);
    BOOST_CHECK(blob->size() != 0);
    BOOST_CHECK(blob->size() == COLOR_HISTOGRAM_BIN_COUNT * binSize * numberOfChannels);

    ImgDetective::Core::SafeFreeBlob(blob);
    ImgDetective::Utils::Memory::SafeDelete(colH);
}

BOOST_AUTO_TEST_CASE(deserialized_histogram_is_identical_to_initial) {
    ColorHistogramFeat* colH = CreateRandomHistogram();

    blob_p_t blob = colH->Serialize();

    BOOST_ASSERT(blob != NULL);

    ColorHistogramFeat* deserializedHist = ColorHistogramFeat::Deserialize(blob);

    BOOST_ASSERT(deserializedHist != NULL);

    ImgDetective::Core::SafeFreeBlob(blob);
    
    FeatureDistance distance = colH->ComputeDistanceTo(*deserializedHist);

    BOOST_CHECK(abs(distance.GetValue()) < 0.0001);

    ImgDetective::Utils::Memory::SafeDelete(colH);
    ImgDetective::Utils::Memory::SafeDelete(deserializedHist);
}

BOOST_AUTO_TEST_SUITE_END()