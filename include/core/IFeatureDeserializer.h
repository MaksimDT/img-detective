#pragma once

#include "core/Feature.h"
#include "common/CommonInternal.h"

namespace ImgDetective {
namespace Core {

    class IFeatureDeserializer : NONCOPYABLE, HAS_VIRTUAL_DESCTUCTOR {
	public:
		virtual IFeature* DeserializeFeature(const blob_t& blob) const = 0;
	};

}
}