#include "modules/cld/FeatureExtractor.h"
#include "utils/MathUtils.h"
#include "utils/GraphicsUtils.h"

#define _USE_MATH_DEFINES
#include <math.h>

namespace ImgDetective {
namespace Modules {
namespace CLD {

    #pragma region CLDFE::ImgBlock impl

    CTOR CLDFE::ImgBlock::ImgBlock() {
        InitInternal(0, 0, 0, 0, 0, NULL);
    }

    void CLDFE::ImgBlock::Init(unsigned int x, unsigned int y, size_t width, size_t height, size_t imgWidth, const Magick::PixelPacket* imgPixels) {
        Utils::Contract::Assert(x >= 0);
        Utils::Contract::Assert(y >= 0);
        Utils::Contract::Assert(width > 0);
        Utils::Contract::Assert(height > 0);
        Utils::Contract::Assert(imgWidth > 0);
        Utils::Contract::AssertNotNull(imgPixels);

        InitInternal(x, y, width, height, imgWidth, imgPixels);
    }

    void CLDFE::ImgBlock::InitInternal(unsigned int x, unsigned int y, size_t width, size_t height, size_t imgWidth, const Magick::PixelPacket* imgPixels) {
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;
        this->imgWidth = imgWidth;
        this->imgPixels = imgPixels;
    }

    const Magick::PixelPacket* CLDFE::ImgBlock::GetBlockStart() const {
        return imgPixels + y * imgWidth + x;
    }

    unsigned long CLDFE::ImgBlock::GetPixelCount() const {
        return width * height;
    }

    #pragma endregion

    CLDFE::CLDFE()
        : Core::FeatureExtractor(CLD_FTYPE_ID) {}

    Core::IFeature* CLDFE::ExtractFrom(const Core::ImgInfo& imgInfo) const {
        //TODO: what if the image's size is less or equal to number of blocks in each row/column?

        const unsigned int N = NUMBER_OF_IMG_BLOCKS_IN_ROW;

        img_block_matrix_t blocks(N, N);
        colors_matrix_t colors(N, N);
        channel_matrix_t y(N, N);
        channel_matrix_t cb(N, N);
        channel_matrix_t cr(N, N);

        //step 1
        DivideImgIntoBlocks(imgInfo, REF blocks);

        //step 2
        CalculateRepresentativeColors(blocks, REF colors);

        //preparing for step 3
        ExtractChannelMatrix(colors, [] (Core::YCbCrColor c) {
            return c.y;
        }, REF y);
        ExtractChannelMatrix(colors, [] (Core::YCbCrColor c) {
            return c.cb;
        }, REF cb);
        ExtractChannelMatrix(colors, [] (Core::YCbCrColor c) {
            return c.cr;
        }, REF cr);

        dct_coeff_matrix_t yDct(N, N);
        dct_coeff_matrix_t cbDct(N, N);
        dct_coeff_matrix_t crDct(N, N);

        //step 3
        DoDct(y, REF yDct);
        DoDct(cb, REF cbDct);
        DoDct(cr, REF crDct);

        CLDFeat::dct_coeff_vect_t* yDctVect = NULL;
        CLDFeat::dct_coeff_vect_t* cbDctVect = NULL;
        CLDFeat::dct_coeff_vect_t* crDctVect = NULL;

        try {
            yDctVect = new CLDFeat::dct_coeff_vect_t(N * N);
            cbDctVect = new CLDFeat::dct_coeff_vect_t(N * N);
            crDctVect = new CLDFeat::dct_coeff_vect_t(N * N);

            //step 4
            yDct.ZigzagScan(REF *yDctVect);
            cbDct.ZigzagScan(REF *cbDctVect);
            crDct.ZigzagScan(REF *crDctVect);

            return new CLDFeat(yDctVect, cbDctVect, crDctVect);
        }
        catch (...) {
            Utils::Memory::SafeDelete(yDctVect);
            Utils::Memory::SafeDelete(cbDctVect);
            Utils::Memory::SafeDelete(crDctVect);
            throw;
        }
    }

    void CLDFE::DivideImgIntoBlocks(const Core::ImgInfo& imgInfo, REF img_block_matrix_t& blocks) const {
        const Magick::Image& img = imgInfo.GetMagickImage();

        unsigned int imgWidth = img.size().width();
        unsigned int imgHeight = img.size().height();

        const Magick::PixelPacket* imgPixels = img.getConstPixels(0, 0, imgWidth, imgHeight);

        unsigned int blockWidth = imgWidth / NUMBER_OF_IMG_BLOCKS_IN_ROW;
        unsigned int blockHeight = imgHeight / NUMBER_OF_IMG_BLOCKS_IN_ROW;

        unsigned int x;
        unsigned int y;

        y = 0;
        for (unsigned int i = 0; i < NUMBER_OF_IMG_BLOCKS_IN_ROW - 1; ++i) {
            x = 0;
            for (unsigned int j = 0; j < NUMBER_OF_IMG_BLOCKS_IN_ROW - 1; ++j) {
                ImgBlock& block = blocks(i, j);
                block.Init(x, y, blockWidth, blockHeight, imgWidth, imgPixels);
                x += blockWidth;
            }

            ImgBlock& lastBlockInRow = blocks(i, NUMBER_OF_IMG_BLOCKS_IN_ROW - 1);
            lastBlockInRow.Init(x, y, imgWidth - x, blockHeight, imgWidth, imgPixels);

            y += blockHeight;
        }

        x = 0;
        for (unsigned int j = 0; j < NUMBER_OF_IMG_BLOCKS_IN_ROW - 1; ++j) {
            ImgBlock& bottomBlock = blocks(NUMBER_OF_IMG_BLOCKS_IN_ROW - 1, j);
            bottomBlock.Init(x, y, blockWidth, imgHeight - y, imgWidth, imgPixels);

            x += blockWidth;
        }

        ImgBlock& cornerBlock = blocks(NUMBER_OF_IMG_BLOCKS_IN_ROW - 1, NUMBER_OF_IMG_BLOCKS_IN_ROW - 1);
        cornerBlock.Init(x, y, imgWidth - x, imgHeight - y, imgWidth, imgPixels);
    }

    void CLDFE::CalculateRepresentativeColors(const img_block_matrix_t& blocks, REF colors_matrix_t& colors) const {
        for (unsigned int i = 0; i < blocks.Rows(); ++i) {
            for (unsigned int j = 0; j < blocks.Columns(); ++j) {
                Core::YCbCrColor rc = CalculateRepresentativeColor(blocks(i, j));
                colors(i, j) = rc;
            }
        }
    }

    void CLDFE::DoDct(const channel_matrix_t& channelMatrix, REF dct_coeff_matrix_t& dctCoeffs) const {
        for (unsigned int i = 0; i < channelMatrix.Rows(); ++i) {
            for (unsigned int j = 0; j < channelMatrix.Columns(); ++j) {
                dctCoeffs(i, j) = CalculateDctCoeff(channelMatrix, i, j);
            }
        }
    }

    Core::YCbCrColor CLDFE::CalculateRepresentativeColor(const ImgBlock& block) const {
        unsigned long sumR = 0;
        unsigned long sumG = 0;
        unsigned long sumB = 0;
        const unsigned long pixelCount = block.GetPixelCount();

        const Magick::PixelPacket* curPixel = block.GetBlockStart();

        for (unsigned int y = 0; y < block.height; ++y) {
            for (unsigned int x = 0; x < block.width; ++x) {
                sumR += curPixel->red;
                sumG += curPixel->green;
                sumB += curPixel->blue;
                
                ++curPixel;
            }

            //move to the next row
            curPixel += (block.imgWidth - block.width);
        }

        unsigned char avgR = sumR / pixelCount;
        unsigned char avgG = sumG / pixelCount;
        unsigned char avgB = sumB / pixelCount;

        return Utils::Graphics::RGBtoYCbCr(avgR, avgG, avgB);
    }

    CLDFeat::dct_coeff_t CLDFE::CalculateDctCoeff(const channel_matrix_t& channelMatrix, unsigned int p, unsigned int q) const {
        const unsigned int M = channelMatrix.Columns();
        const unsigned int N = channelMatrix.Rows();
        const double normalizationCoeff = 510 * N;  //calculated analytically

        double ap = 0;
        if (p == 0) {
            ap = 1.0 / sqrt(M);
        }
        else {
            ap = sqrt(2) / sqrt(M);
        }

        double aq = 0;
        if (q == 0) {
            aq = 1.0 / sqrt(N);
        }
        else {
            aq = sqrt(2) / sqrt(N);
        }

        const channel_matrix_t& A = channelMatrix;
        double cosPartArg1 = (M_PI * p) / (2.0 * M);
        double cosPartArg2 = (M_PI * q) / (2.0 * N);
        CLDFeat::dct_coeff_t coefVal = 0.0;
        for (unsigned int m = 0; m < M; ++m) {
            for (unsigned int n = 0; n < N; ++n) {
                coefVal += A(m, n) * cos(cosPartArg1 * (2 * m + 1)) * cos(cosPartArg2 * (2 * n + 1));
            }
        }
        coefVal *= ap * aq;
        coefVal /= normalizationCoeff;

        return coefVal;
    }

    void CLDFE::ExtractChannelMatrix(const colors_matrix_t& colors, unsigned char (*extractor)(Core::YCbCrColor), REF channel_matrix_t& channelMatrix) const {
        for (unsigned int i = 0; i < colors.Rows(); ++i) {
            for (unsigned int j = 0; j < colors.Columns(); ++j) {
                channelMatrix(i, j) = extractor(colors(i, j));
            }
        }
    }
}
}
}