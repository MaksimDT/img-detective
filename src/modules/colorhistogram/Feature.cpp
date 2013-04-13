#include "modules/colorhistogram/Feature.h"
#include "utils/MathUtils.h"
#include "utils/ContractUtils.h"
#include "utils/MemoryUtils.h"

using namespace ImgDetective::Core;
using namespace std;

namespace ImgDetective {
namespace Features {

    #pragma region NESTED IMPL

    CTOR ColorHistogramFeat::ChannelHistogram::ChannelHistogram(bins_vector_t* bins)
        : bins(bins) {
        Utils::Contract::AssertNotNull(bins);
        Utils::Contract::Assert(bins->size() == COLOR_HISTOGRAM_BIN_COUNT);
    }

    ColorHistogramFeat::ChannelHistogram::~ChannelHistogram() {
        Utils::Memory::SafeDelete(bins);
    }

    ColorHistogramFeat::ChannelHistogram* ColorHistogramFeat::ChannelHistogram::Deserialize(const Core::blob_t& buffer, REF size_t& offset) {
        Utils::Contract::Assert(buffer.size() >= offset + COLOR_HISTOGRAM_BIN_COUNT * sizeof(bin_value_t));

        bins_vector_t* bins = NULL;
        ChannelHistogram* result = NULL;

        try {
            bins = new bins_vector_t(COLOR_HISTOGRAM_BIN_COUNT);
            bins_vector_t& binsRef = *bins;

            for (size_t i = 0; i < COLOR_HISTOGRAM_BIN_COUNT; ++i, ++offset) {
                binsRef[i] = buffer[offset];
            }

            result = new ChannelHistogram(bins);
        }
        catch (...) {
            if (result == NULL) {
                Utils::Memory::SafeDelete(bins);
            }
            else {
                Utils::Memory::SafeDelete(result);
            }
            throw;
        }

        return result;
    }

    void ColorHistogramFeat::ChannelHistogram::SerializeToBuffer(REF Core::blob_t& buffer, REF size_t& offset) const {
        Utils::Contract::Assert(buffer.size() >= offset + bins->size());

        bins_vector_t& binsRef = *bins;

        for (size_t i = 0; i < binsRef.size(); ++i, ++offset) {
            buffer[offset] = binsRef[i];
        }
    }

    double ColorHistogramFeat::ChannelHistogram::ComputeDistanceTo(const REF ColorHistogramFeat::ChannelHistogram& other) const {
        const unsigned long maxDiff = (COLOR_HISTOGRAM_BIN_COUNT) * (NUMBER_OF_BIN_VALUES - 1);
        unsigned long diff = 0;

        bins_vector_t& thisBins = *(this->bins);
        bins_vector_t& otherBins = *(other.bins);

        for (size_t i = 0; i < COLOR_HISTOGRAM_BIN_COUNT; ++i) {
            diff += labs(thisBins[i] - otherBins[i]);
        }

        //return the normalized value in the interval [0..1]
        return (double)diff / (double)maxDiff;
    }

    #pragma endregion

    #pragma region ColorHistogramFeat impl

    CTOR ColorHistogramFeat::ColorHistogramFeat(
        ColorHistogramFeat::ChannelHistogram* r, 
        ColorHistogramFeat::ChannelHistogram* g, 
        ColorHistogramFeat::ChannelHistogram* b)
        : Feature(COLOR_HISTOGRAM_FTYPE_ID), r(r), g(g), b(b) { 
        Utils::Contract::AssertNotNull(r);
        Utils::Contract::AssertNotNull(g);
        Utils::Contract::AssertNotNull(b);
    }

    ColorHistogramFeat::~ColorHistogramFeat() {
        Utils::Memory::SafeDelete(r);
        Utils::Memory::SafeDelete(g);
        Utils::Memory::SafeDelete(b);
    }

    ColorHistogramFeat* ColorHistogramFeat::Deserialize(const Core::blob_p_t blob) {
        ColorHistogramFeat* result = NULL;
        ColorHistogramFeat::ChannelHistogram* r = NULL;
        ColorHistogramFeat::ChannelHistogram* g = NULL;
        ColorHistogramFeat::ChannelHistogram* b = NULL;

        try {
            size_t bufOffset = 0;
            r = ColorHistogramFeat::ChannelHistogram::Deserialize(*blob, REF bufOffset);
            g = ColorHistogramFeat::ChannelHistogram::Deserialize(*blob, REF bufOffset);
            b = ColorHistogramFeat::ChannelHistogram::Deserialize(*blob, REF bufOffset);
            result = new ColorHistogramFeat(r, g, b);
        }
        catch (...) {
            if (result != NULL) {
                Utils::Memory::SafeDelete(result);
            }
            else {
                Utils::Memory::SafeDelete(r);
                Utils::Memory::SafeDelete(g);
                Utils::Memory::SafeDelete(b);
            }
            throw;
        }

        return result;
    }

    Core::blob_p_t ColorHistogramFeat::Serialize() const {
        const size_t numberOfChannels = 3;
        const size_t oneBinSizeInBytes = sizeof(ColorHistogramFeat::ChannelHistogram::bin_value_t);
        size_t requiredBufferSize = numberOfChannels * COLOR_HISTOGRAM_BIN_COUNT * oneBinSizeInBytes;

        Core::blob_p_t buffer = NULL;

        try {
            buffer = Core::CreateBlobOfSize(requiredBufferSize);

            size_t bufIndex = 0;
            r->SerializeToBuffer(REF *buffer, REF bufIndex);
            g->SerializeToBuffer(REF *buffer, REF bufIndex);
            b->SerializeToBuffer(REF *buffer, REF bufIndex);
        }
        catch (...) {
            Core::SafeFreeBlob(buffer);
            throw;
        }

        return buffer;
    }

    Core::FeatureDistance ColorHistogramFeat::ComputeDistanceInternal(const REF Core::IFeature& other) const {
        Utils::Contract::Assert(GetTypeId() == other.GetTypeId());
        const ColorHistogramFeat& otherAs = (const ColorHistogramFeat&)other;

        double rDistance = r->ComputeDistanceTo(*otherAs.r);
        double gDistance = g->ComputeDistanceTo(*otherAs.g);
        double bDistance = b->ComputeDistanceTo(*otherAs.b);

        double distanceValue = (rDistance + gDistance + bDistance) / 3;

        return FeatureDistance(distanceValue);
    }

    #pragma endregion
}
}