#include "FeatureDistance.h"
#include "ContractUtils.h"

namespace ImgDetective {
namespace Core {

	CTOR FeatureDistance::FeatureDistance(double val) {
		Utils::Contract::Assert(0.0 <= val && val <= 1.0);

		this->value = val;
	}

	double FeatureDistance::GetValue() const {
		return value;
	}

	void FeatureDistance::Multiply(double coeff) {
		Utils::Contract::Assert(0.0 <= coeff && coeff <= 1.0);

		this->value *= coeff;
	}

	bool FeatureDistance::operator<(const REF FeatureDistance& other) const {
		return this->GetValue() < other.GetValue();
	}

	bool FeatureDistance::operator>(const REF FeatureDistance& other) const {
		return this->GetValue() > other.GetValue();
	}
}
}