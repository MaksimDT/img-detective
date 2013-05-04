#include "modules/colorhistogram/FeatureDeserializer.h"
#include "modules/colorhistogram/Feature.h"

namespace ImgDetective {
namespace Modules {
namespace ColorHistogram {

    CTOR ColorHistogramFD::ColorHistogramFD() 
        : Core::IFeatureDeserializer() {}

    Core::IFeature* ColorHistogramFD::DeserializeFeature(const Core::blob_t& blob) const {
        ColorHistogramFeat* deserialized = ColorHistogramFeat::Deserialize(blob);
        return deserialized;
    }

}
}
}