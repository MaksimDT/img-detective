#pragma once

#include "CommonInternal.h"

namespace ImgDetective {
namespace Core {

	//represents a distance between two features of image
	CONCRETE SEALED class FeatureDistance {
	public:
		FeatureDistance(double val);
		double GetValue() const;

		/*multiplies the distance value by the specified 
		  coefficient. Coefficient must be in range [0..1]*/
		void Multiply(double coeff);
	private:
		//normalized value of distance [0..1]
		double value;
	};

}
}