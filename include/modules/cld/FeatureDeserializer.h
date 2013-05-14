#pragma once

#include "common/CommonInternal.h"
#include "core/IFeatureDeserializer.h"

namespace ImgDetective {
namespace Modules {
namespace CLD {

    class CLDFD : public Core::IFeatureDeserializer {
    public:
        CTOR CLDFD();

        virtual Core::IFeature* DeserializeFeature(const Core::blob_t& blob) const;
    };

}
}
}