#pragma once

#include "modules/colorhistogram/Feature.h"
#include "core/FeatureExtractor.h"
#include <vector>

namespace ImgDetective {
namespace Modules {
namespace ColorHistogram {

	//Feature extractor for color histogram
	class ColorHistogramFE : public Core::FeatureExtractor {
	public:
        CTOR ColorHistogramFE();

		virtual Core::IFeature* ExtractFrom(const Core::ImgInfo& imgInfo) const;
    private:
        ColorHistogramFeat::ChannelHistogram* ConvertToChannelHist(const std::vector<unsigned long>& histAbsVals, unsigned long totalPixelCount) const;
	};

}
}
}