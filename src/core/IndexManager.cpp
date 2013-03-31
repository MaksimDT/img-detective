#include "core/IndexManager.h"
#include "utils/ContractUtils.h"
#include "utils/MemoryUtils.h"

namespace ImgDetective {
namespace Core {

	Feature::type_id_t IndexManager::GetFeatureTypeId() const {
		return featureTypeId;
	}

	CTOR IndexManager::IndexManager(IIndexStorage* storage, Feature::type_id_t featureTypeId) {
		Utils::Contract::AssertNotNull(storage);

		this->storage = storage;
		this->featureTypeId = featureTypeId;
	}

    void IndexManager::AddFeature(REF IFeature& feature, imgid_t imgId) {
        storage->AddFeature(feature, imgId);
    }

	IndexManager::~IndexManager() {
		Utils::Memory::SafeDelete(storage);
	}
}
}