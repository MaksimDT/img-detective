#include "modules/colorhistogram/FeatureDeserializer.h"
#include "modules/colorhistogram/Feature.h"

namespace ImgDetective {
namespace Modules {
namespace ColorHistogram {

    CTOR ColorHistogramFD::ColorHistogramFD() 
        : Core::IFeatureDeserializer() {}

    Core::IFeature* ColorHistogramFD::DeserializeFeature(const Core::blob_t& blob) const {
        const size_t numberOfChannels = 3;
        const size_t oneBinSizeInBytes = sizeof(ColorHistogramFeat::ChannelHistogram::bin_value_t);
        size_t requiredBufferSize = numberOfChannels * COLOR_HISTOGRAM_BIN_COUNT * oneBinSizeInBytes;

        if (blob.size() != requiredBufferSize) {
            throw Core::FeatureDeserializationException("buffer has a wrong length");
        }

        ColorHistogramFeat* deserialized = ColorHistogramFeat::Deserialize(blob);
        return deserialized;
    }

}
}
}