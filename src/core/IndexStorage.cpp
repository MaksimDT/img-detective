#include "core/IndexStorage.h"
#include "utils/ContractUtils.h"
#include "utils/MemoryUtils.h"

namespace ImgDetective {
namespace Core {

	CTOR IndexStorage::IndexStorage(IFeatureDeserializer* featureDeserializer) {
		Utils::Contract::AssertNotNull(featureDeserializer);

		this->featureDeserializer = featureDeserializer;
	}

	IndexStorage::~IndexStorage() {
		Utils::Memory::SafeDelete(featureDeserializer);
	}

	IndexStorage::LookupSessionBase::LookupSessionBase(const REF IFeatureDeserializer& featureDeserializer)
		: featureDeserializer(featureDeserializer) { }
}
}