#pragma once

#include "common/CommonInternal.h"
#include "common/Matrix.h"
#include "modules/cld/Feature.h"
#include "core/FeatureExtractor.h"

#include <vector>

namespace ImgDetective {
namespace Modules {
namespace CLD {

    //Feature extractor for color histogram
    class CLDFE : public Core::FeatureExtractor {
    public:
        CTOR CLDFE();

        virtual Core::IFeature* ExtractFrom(const Core::ImgInfo& imgInfo) const;
    private:
        NESTED class ImgBlock {
        public:
            CTOR ImgBlock();
            void Init(unsigned int x, unsigned int y, size_t width, size_t height, size_t imgWidth, const Magick::PixelPacket* imgPixels);
            const Magick::PixelPacket* GetBlockStart() const;
            unsigned long GetPixelCount() const;

            unsigned int x;
            unsigned int y;
            size_t width; 
            size_t height;
            size_t imgWidth;
        private:
            void InitInternal(unsigned int x, unsigned int y, size_t width, size_t height, size_t imgWidth, const Magick::PixelPacket* imgPixels);
            const Magick::PixelPacket* imgPixels; 
        };

        typedef Core::Matrix<ImgBlock> img_block_matrix_t;
        typedef Core::Matrix<Core::YCbCrColor> colors_matrix_t;
        typedef Core::Matrix<unsigned char> channel_matrix_t;
        typedef Core::Matrix<CLDFeat::dct_coeff_t> dct_coeff_matrix_t;

        //step one
        void DivideImgIntoBlocks(const Core::ImgInfo& imgInfo, REF img_block_matrix_t& blocks) const;
        //step two
        void CalculateRepresentativeColors(const img_block_matrix_t& blocks, REF colors_matrix_t& colors) const;
        Core::YCbCrColor CalculateRepresentativeColor(const ImgBlock& block) const;
        void ExtractChannelMatrix(const colors_matrix_t& colors, unsigned char (*extractor)(Core::YCbCrColor), REF channel_matrix_t& channelMatrix) const;
        //step three
        void DoDct(const channel_matrix_t& channelMatrix, REF dct_coeff_matrix_t& dctCoeffs) const;
        CLDFeat::dct_coeff_t CalculateDctCoeff(const channel_matrix_t& channelMatrix, unsigned int p, unsigned int q) const;
    };

}
}
}