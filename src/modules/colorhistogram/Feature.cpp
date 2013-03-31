#include "modules/colorhistogram/Feature.h"
#include "utils/MathUtils.h"
#include "utils/ContractUtils.h"

using namespace ImgDetective::Core;

namespace ImgDetective {
namespace Features {

    #pragma region NESTED IMPL
    
    CTOR ColorHistogramFeat::ChannelHistogram::ChannelHistogram(const REF std::vector<bin_value_t>& bins)
        : bins(bins) {

        Utils::Contract::Assert(bins.size() == COLOR_HISTOGRAM_BIN_COUNT);
    }

    void ColorHistogramFeat::ChannelHistogram::SerializeToBuffer(const REF Core::blob_t& buffer, REF Core::blob_t::iterator& bufferIt) const {
        int i;
        for (i = 0; i < bins.size() && bufferIt != buffer.end(); ++i, ++bufferIt) {
            *bufferIt = bins[i];
        }

        //to ensure that the histogram was completely written to buffer (i.e. buffer's size was sufficient)
        Utils::Contract::Ensure(i == bins.size());
    }

    double ColorHistogramFeat::ChannelHistogram::ComputeDistanceTo(const REF ColorHistogramFeat::ChannelHistogram& other) const {
        //TODO: wtf
        const unsigned long maxDiff = COLOR_HISTOGRAM_BIN_COUNT * Utils::Math::GetNumberOfStates(sizeof(ColorHistogramFeat::ChannelHistogram::bin_value_t));
        unsigned long diff = 0;

        for (size_t i = 0; i < COLOR_HISTOGRAM_BIN_COUNT; ++i) {
            diff += labs(this->bins[i] - other.bins[i]);
        }

        //return normalized value in internal [0..1]
        return (double)diff / (double)maxDiff;
    }

    #pragma endregion

    CTOR ColorHistogramFeat::ColorHistogramFeat(
        const REF ColorHistogramFeat::ChannelHistogram& r, 
        const REF ColorHistogramFeat::ChannelHistogram& g, 
        const REF ColorHistogramFeat::ChannelHistogram& b)
        : Feature(COLOR_HISTOGRAM_FTYPE_ID), r(r), g(g), b(b) { 
    }

    Core::blob_p_t ColorHistogramFeat::Serialize() const {
        const size_t numberOfChannels = 3;
        const size_t oneBinSizeInBytes = sizeof(ColorHistogramFeat::ChannelHistogram::bin_value_t);
        size_t requiredBufferSize = numberOfChannels * COLOR_HISTOGRAM_BIN_COUNT * oneBinSizeInBytes;

        Core::blob_p_t buffer = Core::CreateBlobOfSize(requiredBufferSize);
        Core::blob_t::iterator bufferIt;
        
        r.SerializeToBuffer(REF *buffer, REF bufferIt);
        g.SerializeToBuffer(REF *buffer, REF bufferIt);
        b.SerializeToBuffer(REF *buffer, REF bufferIt);

        return buffer;
    }

    Core::FeatureDistance ColorHistogramFeat::ComputeDistanceInternal(const REF Core::IFeature& other) const {
        Utils::Contract::Assert(GetTypeId() == other.GetTypeId());
        ColorHistogramFeat otherAs = (const REF ColorHistogramFeat&)other;

        double rDistance = r.ComputeDistanceTo(otherAs.r);
        double gDistance = g.ComputeDistanceTo(otherAs.g);
        double bDistance = b.ComputeDistanceTo(otherAs.b);

        double distanceValue = (rDistance + gDistance + bDistance) / 3;

        return FeatureDistance(distanceValue);
    }
}
}