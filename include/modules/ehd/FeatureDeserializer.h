#pragma once

#include "common/CommonInternal.h"
#include "core/IFeatureDeserializer.h"

namespace ImgDetective {
namespace Modules {
namespace EHD {

    class EHDFD : public Core::IFeatureDeserializer {
    public:
        CTOR EHDFD();

        virtual Core::IFeature* DeserializeFeature(const Core::blob_t& blob) const;
    };

}
}
}