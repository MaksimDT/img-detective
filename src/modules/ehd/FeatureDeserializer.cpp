#include "modules/ehd/FeatureDeserializer.h"
#include "modules/ehd/Feature.h"

namespace ImgDetective {
namespace Modules {
namespace EHD {

    CTOR EHDFD::EHDFD() 
        : Core::IFeatureDeserializer() {}

    Core::IFeature* EHDFD::DeserializeFeature(const Core::blob_t& blob) const {
        size_t requiredBufferSize = TOTAL_NUMBER_OF_BINS * sizeof(EHDFeat::bin_value_t);

        if (blob.size() != requiredBufferSize) {
            throw Core::FeatureDeserializationException("buffer has a wrong length");
        }

        EHDFeat* deserialized = EHDFeat::Deserialize(blob);
        return deserialized;
    }

}
}
}