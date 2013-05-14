#include "modules/cld/FeatureDeserializer.h"
#include "modules/cld/Feature.h"

namespace ImgDetective {
namespace Modules {
namespace CLD {

    CTOR CLDFD::CLDFD() 
        : Core::IFeatureDeserializer() {}

    Core::IFeature* CLDFD::DeserializeFeature(const Core::blob_t& blob) const {
        size_t requiredBufferSize = NUMBER_OF_DCT_COEFS * sizeof(CLDFeat::dct_coeff_t) * 3; //3 means 3 channels (Y, Cb, Cr)

        if (blob.size() != requiredBufferSize) {
            throw Core::FeatureDeserializationException("buffer has a wrong length");
        }

        CLDFeat* deserialized = CLDFeat::Deserialize(blob);
        return deserialized;
    }

}
}
}