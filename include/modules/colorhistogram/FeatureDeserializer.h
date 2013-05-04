#pragma once

#include "common/CommonInternal.h"
#include "core/IFeatureDeserializer.h"

namespace ImgDetective {
namespace Modules {
namespace ColorHistogram {

    class ColorHistogramFD : public Core::IFeatureDeserializer {
    public:
        CTOR ColorHistogramFD();

        virtual Core::IFeature* DeserializeFeature(const Core::blob_t& blob) const;
    };

}
}
}