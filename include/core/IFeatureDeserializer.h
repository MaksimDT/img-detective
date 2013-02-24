#pragma once

#include "core/Feature.h"
#include "common/CommonInternal.h"

namespace ImgDetective {
namespace Core {

	INTERFACE class IFeatureDeserializer {
	public:
		virtual ~IFeatureDeserializer();

		virtual Feature* DeserializeFeature(const REF blob_t blob) const = 0;
	};

}
}