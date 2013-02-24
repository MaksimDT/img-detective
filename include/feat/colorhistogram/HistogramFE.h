#pragma once

#include "core/FeatureExtractor.h"

namespace ImgDetective {
namespace FeatureExtractor {

	//Feature extractor for color histogram
	class HistogramFE : public Core::FeatureExtractor {
	public:
		virtual Core::Feature* ExtractFrom(Core::ImgInfo& imgInfo);
	};

}
}