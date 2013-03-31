#include "core/Feature.h"
#include "utils/ContractUtils.h"

namespace ImgDetective {
namespace Core {

	Feature::type_id_t Feature::GetTypeId() const {
		return typeId;
	}

	FeatureDistance Feature::ComputeDistanceTo(const REF IFeature& feature) const {
		Utils::Contract::Assert(this->GetTypeId() == feature.GetTypeId());

		return ComputeDistanceInternal(REF feature);
	}

	CTOR Feature::Feature(Feature::type_id_t typeId) {
		this->typeId = typeId;
	}

}
}