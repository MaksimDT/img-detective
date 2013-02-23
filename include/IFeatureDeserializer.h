#pragma once

#include "Feature.h"
#include "CommonInternal.h"

namespace ImgDetective {
namespace Core {

	INTERFACE class IFeatureDeserializer {
	public:
		virtual Feature* DeserializeFeature(const REF blob_t blob) const = 0;
	protected:
		virtual ~IFeatureDeserializer();
	};

}
}