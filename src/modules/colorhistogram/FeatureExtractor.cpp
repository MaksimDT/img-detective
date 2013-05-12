#include "modules/colorhistogram/Feature.h"
#include "modules/colorhistogram/FeatureExtractor.h"
#include "utils/ContractUtils.h"
#include "utils/MemoryUtils.h"

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
        unsigned long* binsR = NULL;
        unsigned long* binsG = NULL;
        unsigned long* binsB = NULL;
        ColorHistogramFeat* result = NULL;


        try {
            binsR = new unsigned long[COLOR_HISTOGRAM_BIN_COUNT];
            memset(binsR, 0, COLOR_HISTOGRAM_BIN_COUNT * sizeof(unsigned long));
            binsG = new unsigned long[COLOR_HISTOGRAM_BIN_COUNT];
            memset(binsG, 0, COLOR_HISTOGRAM_BIN_COUNT * sizeof(unsigned long));
            binsB = new unsigned long[COLOR_HISTOGRAM_BIN_COUNT];
            memset(binsB, 0, COLOR_HISTOGRAM_BIN_COUNT * sizeof(unsigned long));

            Magick::Image img = imgInfo.GetMagickImage();
            Magick::Geometry imgSize = img.size();
            unsigned int imgWidth = imgSize.width();
            unsigned int imgHeight = imgSize.height();

            const MagickLib::PixelPacket* pixels = img.getConstPixels(0, 0, imgWidth, imgHeight);
            unsigned long totalPixelCount = imgWidth * imgHeight;

            for (unsigned long i = 0; i < totalPixelCount; ++i)
            {
                MagickLib::Quantum r = pixels[i].red;
                MagickLib::Quantum g = pixels[i].green;
                MagickLib::Quantum b = pixels[i].blue;

                ++binsR[r];
                ++binsG[g];
                ++binsB[b];
            }

            result = new ColorHistogramFeat(
                ConvertToChannelHist(binsR, totalPixelCount), 
                ConvertToChannelHist(binsG, totalPixelCount),
                ConvertToChannelHist(binsB, totalPixelCount));

            Utils::Memory::SafeDeleteArray(binsR);
            Utils::Memory::SafeDeleteArray(binsG);
            Utils::Memory::SafeDeleteArray(binsB);

            return result;
        }
        catch (...) {
            try {
                Utils::Memory::SafeDeleteArray(binsR);
                Utils::Memory::SafeDeleteArray(binsG);
                Utils::Memory::SafeDeleteArray(binsB);
                Utils::Memory::SafeDelete(result);
            }
            catch (...) {
                //TODO: logging
            }

            throw;
        }
    }

    ColorHistogramFeat::ChannelHistogram* ColorHistogramFE::ConvertToChannelHist(const unsigned long* histAbsVals, unsigned long totalPixelCount) const {
        if (totalPixelCount == 0) {
            //in order to avoid division by zero
            totalPixelCount = 1;
        }


        ColorHistogramFeat::ChannelHistogram::bins_vector_t* histRelVals = new ColorHistogramFeat::ChannelHistogram::bins_vector_t(COLOR_HISTOGRAM_BIN_COUNT);

        unsigned long normDivisor = (unsigned long)ceil((double)totalPixelCount / MAX_BIN_VALUE);

        for (size_t i = 0; i < COLOR_HISTOGRAM_BIN_COUNT; ++i) {
            (*histRelVals)[i] = histAbsVals[i] / normDivisor;
        }

        return new ColorHistogramFeat::ChannelHistogram(histRelVals);
    }
}
}
}