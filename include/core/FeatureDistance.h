#pragma once

#include "common/CommonInternal.h"

namespace ImgDetective {
namespace Core {

	//represents a distance between two features of image
	CONCRETE SEALED class FeatureDistance {
	public:
		CTOR FeatureDistance(double val);
		double GetValue() const;
        double GetRelevanceValue() const;

		/*multiplies the distance value by the specified 
		  coefficient. Coefficient must be in range [0..1]*/
		void Multiply(double coeff);

		bool operator<(const REF FeatureDistance& other) const;
		bool operator>(const REF FeatureDistance& other) const;
	private:
		//normalized value of distance [0..1]
		double value;
	};

}
}