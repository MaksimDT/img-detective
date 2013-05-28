#pragma once

#include "common/CommonInternal.h"
#include "common/Matrix.h"
#include "modules/ehd/Feature.h"
#include "core/FeatureExtractor.h"
#include "modules/ehd/SpatialFilters.h"

#include <vector>

namespace ImgDetective {
namespace Modules {
namespace EHD {

    //Feature extractor for color histogram
    class EHDFE : public Core::FeatureExtractor {
    public:
        CTOR EHDFE();

        virtual Core::IFeature* ExtractFrom(const Core::ImgInfo& imgInfo) const;
    private:
        NESTED class SubImg {
        public:
            CTOR SubImg();
            CTOR SubImg(unsigned int x, unsigned int y, size_t width, size_t height, size_t imgWidth, const Magick::PixelPacket* imgPixels);
            void Init(unsigned int x, unsigned int y, size_t width, size_t height, size_t imgWidth, const Magick::PixelPacket* imgPixels);

            EHDFeat::bins_vect_t CalculateEdgeHistogram() const;
            EHDFeat::EdgeType GetEdgeType() const;
            unsigned int GetAverageIntensity() const;
        private:
            void InitInternal(unsigned int x, unsigned int y, size_t width, size_t height, size_t imgWidth, const Magick::PixelPacket* imgPixels);
            const Magick::PixelPacket* EHDFE::SubImg::GetBlockStart() const;

            const Magick::PixelPacket* imgPixels; 
            unsigned int x;
            unsigned int y;
            size_t width; 
            size_t height;
            size_t imgWidth;
        };

        typedef Core::Matrix<SubImg> subimg_matrix_t;

        void DivideIntoSubImgs(const Core::ImgInfo& imgInfo, REF subimg_matrix_t& subimgs) const;
        void ExtractSubImgsHistograms(const subimg_matrix_t& subimgs, OUT std::vector<EHDFeat::bins_vect_t>& subimgHistograms) const;

        static SpatialFilters filters;
    };

}
}
}