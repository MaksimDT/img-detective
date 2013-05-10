#include "modules/colorhistogram/Feature.h"
#include "modules/colorhistogram/FeatureExtractor.h"
#include "utils/ContractUtils.h"

#include <cmath>

using namespace std;

namespace ImgDetective {
namespace Modules {
namespace ColorHistogram {

    CTOR ColorHistogramFE::ColorHistogramFE()
        : Core::FeatureExtractor(COLOR_HISTOGRAM_FTYPE_ID) {
        Utils::Contract::Assert(sizeof(MagickLib::Quantum) == 1);
    }

    Core::IFeature* ColorHistogramFE::ExtractFrom(const Core::ImgInfo& imgInfo) const {
        vector<unsigned long> binsR(COLOR_HISTOGRAM_BIN_COUNT, 0);
        vector<unsigned long> binsG(COLOR_HISTOGRAM_BIN_COUNT, 0);
        vector<unsigned long> binsB(COLOR_HISTOGRAM_BIN_COUNT, 0);

        Magick::Image img = imgInfo.GetMagickImage();
        Magick::Geometry imgSize = img.size();
        unsigned int imgWidth = imgSize.width();
        unsigned int imgHeight = imgSize.height();

        unsigned long factMaxBinValue = 0;

        for (unsigned int y = 0; y < imgHeight; ++y) {
            for (unsigned int x = 0; x < imgWidth; ++x) {
                Magick::Color clr = img.pixelColor(x, y);
                MagickLib::Quantum r = clr.redQuantum();
                MagickLib::Quantum g = clr.greenQuantum();
                MagickLib::Quantum b = clr.blueQuantum();

                ++binsR[r];
                ++binsG[g];
                ++binsB[b];

                if (binsR[r] > factMaxBinValue) {
                    factMaxBinValue = binsR[r];
                }

                if (binsR[g] > factMaxBinValue) {
                    factMaxBinValue = binsR[g];
                }

                if (binsR[b] > factMaxBinValue) {
                    factMaxBinValue = binsR[b];
                }
            }
        }

        unsigned long totalPixelCount = factMaxBinValue;

        return new ColorHistogramFeat(
            ConvertToChannelHist(binsR, totalPixelCount), 
            ConvertToChannelHist(binsG, totalPixelCount),
            ConvertToChannelHist(binsB, totalPixelCount));
    }

    ColorHistogramFeat::ChannelHistogram* ColorHistogramFE::ConvertToChannelHist(const std::vector<unsigned long>& histAbsVals, unsigned long totalPixelCount) const {
        Utils::Contract::Assert(histAbsVals.size() == COLOR_HISTOGRAM_BIN_COUNT);

        if (totalPixelCount == 0) {
            //in order to avoid division by zero
            totalPixelCount = 1;
        }

        ColorHistogramFeat::ChannelHistogram::bins_vector_t* histRelVals = new ColorHistogramFeat::ChannelHistogram::bins_vector_t(COLOR_HISTOGRAM_BIN_COUNT);

        unsigned long divisor = (unsigned long)ceil((double)(totalPixelCount / MAX_BIN_VALUE));

        for (size_t i = 0; i < COLOR_HISTOGRAM_BIN_COUNT; ++i) {
            (*histRelVals)[i] = histAbsVals[i] / divisor;
        }

        return new ColorHistogramFeat::ChannelHistogram(histRelVals);
    }
}
}
}