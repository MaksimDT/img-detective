#include "modules/ehd/FeatureExtractor.h"
#include "utils/MathUtils.h"
#include "utils/GraphicsUtils.h"

#define _USE_MATH_DEFINES
#include <math.h>

namespace ImgDetective {
namespace Modules {
namespace EHD {

    SpatialFilters EHDFE::filters;

    #pragma region EHDFE::SubImg impl

    CTOR EHDFE::SubImg::SubImg() {
        InitInternal(0, 0, 0, 0, 0, NULL);
    }

    CTOR EHDFE::SubImg::SubImg(unsigned int x, unsigned int y, size_t width, size_t height, size_t imgWidth, const Magick::PixelPacket* imgPixels) {
        Init(x, y, width, height, imgWidth, imgPixels);
    }

    void EHDFE::SubImg::Init(unsigned int x, unsigned int y, size_t width, size_t height, size_t imgWidth, const Magick::PixelPacket* imgPixels) {
        Utils::Contract::Assert(x >= 0);
        Utils::Contract::Assert(y >= 0);
        Utils::Contract::Assert(width > 0);
        Utils::Contract::Assert(height > 0);
        Utils::Contract::Assert(imgWidth > 0);
        Utils::Contract::AssertNotNull(imgPixels);

        InitInternal(x, y, width, height, imgWidth, imgPixels);
    }

    EHDFeat::bins_vect_t EHDFE::SubImg::CalculateEdgeHistogram() const {
        const unsigned int cols = NUMBER_OF_IMG_BLOCKS_IN_SUB_IMG_ROW;
        const unsigned int rows = NUMBER_OF_IMG_BLOCKS_IN_SUB_IMG_ROW;

        EHDFeat::bins_vect_t bins(NUMBER_OF_EDGE_TYPES, 0);

        //size of block must be a multiple of 2
        size_t blockWidth = width / cols;
        if (blockWidth % 2 != 0) {
            blockWidth -= 1;
        }

        //size of block must be a multiple of 2
        size_t blockHeight = height / rows;
        if (blockHeight % 2 != 0) {
            blockHeight -= 1;
        }

        unsigned int curY = y;
        for (unsigned int rowIndex = 0; rowIndex < rows; ++rowIndex) {
            unsigned int curX = x;
            for (unsigned int colIndex = 0; colIndex < cols; ++colIndex) {
                SubImg imgBlock(curX, curY, blockWidth, blockHeight, imgWidth, imgPixels);
                EHDFeat::EdgeType et = imgBlock.GetEdgeType();

                if (et != EHDFeat::EdgeType::Monotone) {
                    ++bins[(int)et];
                }

                curX += blockWidth;
            }
            curY += blockHeight;
        }

        return bins;
    }

    EHDFeat::EdgeType EHDFE::SubImg::GetEdgeType() const {
        const double threshold = 50;

        //divide subimage into four blocks

        unsigned int xCenter = this->x + this->width / 2;
        unsigned int yCenter = this->y + this->height / 2;

        std::vector<unsigned int> a(4);
        
        SubImg lefttop(x, y, width / 2, height / 2, imgWidth, imgPixels);
        SubImg righttop(xCenter, y, width / 2, height / 2, imgWidth, imgPixels);
        SubImg leftbottom(x, yCenter, width / 2, height / 2, imgWidth, imgPixels);
        SubImg rightbottom(xCenter, yCenter, width / 2, height / 2, imgWidth, imgPixels);

        a[0] = lefttop.GetAverageIntensity();
        a[1] = righttop.GetAverageIntensity();
        a[2] = leftbottom.GetAverageIntensity();
        a[3] = rightbottom.GetAverageIntensity();

        int domEdgeTypeId = -1;
        double maxM = 0.0;
        for (int edgeTypeId = 0; edgeTypeId < NUMBER_OF_EDGE_TYPES; ++edgeTypeId) {
            //apply the next spatial filter to intensity matrix of subimages
            SpatialFilters::filter_t f = EHDFE::filters.Get((EHDFeat::EdgeType)edgeTypeId);
            double m = 0.0;
            for (unsigned int i = 0; i < 4; ++i) {
                m += a[i] * f[i];
            }
            m = abs(m);
            if (m > maxM) {
                maxM = m;
                domEdgeTypeId = edgeTypeId;
            }
        }

        if (maxM > threshold) {
            return (EHDFeat::EdgeType)domEdgeTypeId;
        }
        else {
            return EHDFeat::EdgeType::Monotone;
        }
    }

    unsigned int EHDFE::SubImg::GetAverageIntensity() const {
        const MagickLib::PixelPacket* pixelsPtr = GetBlockStart();

        unsigned long sumR = 0;
        unsigned long sumG = 0;
        unsigned long sumB = 0;
        for (unsigned int row = 0; row < height; ++row) {
            for (unsigned int column = 0; column < width; ++column) {
                const MagickLib::PixelPacket& pp = *pixelsPtr;
                sumR += pp.red;
                sumG += pp.green;
                sumB += pp.blue;
                ++pixelsPtr;
            }
            pixelsPtr += imgWidth - this->width;
        }

        unsigned long pixelCount = width * height;
        unsigned char avgR = sumR / pixelCount;
        unsigned char avgG = sumG / pixelCount;
        unsigned char avgB = sumB / pixelCount;

        Core::YCbCrColor ycbcrAvg = Utils::Graphics::RGBtoYCbCr(avgR, avgG, avgB);
        return ycbcrAvg.y;
    }

    void EHDFE::SubImg::InitInternal(unsigned int x, unsigned int y, size_t width, size_t height, size_t imgWidth, const Magick::PixelPacket* imgPixels) {
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;
        this->imgWidth = imgWidth;
        this->imgPixels = imgPixels;
    }

    const Magick::PixelPacket* EHDFE::SubImg::GetBlockStart() const {
        return imgPixels + y * imgWidth + x;
    }

    #pragma endregion

    EHDFE::EHDFE()
        : Core::FeatureExtractor(EHD_FTYPE_ID) {}

    Core::IFeature* EHDFE::ExtractFrom(const Core::ImgInfo& imgInfo) const {
        const unsigned int N = NUMBER_OF_SUBIMGS_IN_ROW;

        subimg_matrix_t subimgs(N, N);

        DivideIntoSubImgs(imgInfo, REF subimgs);

        std::vector<EHDFeat::bins_vect_t> subimgHistograms;
        ExtractSubImgsHistograms(subimgs, OUT subimgHistograms);

        EHDFeat::bins_vect_t* bins = NULL;
        try {
            bins = new EHDFeat::bins_vect_t(subimgHistograms.size() * NUMBER_OF_EDGE_TYPES);
            EHDFeat::bins_vect_t& binsRef = *bins;

            //copy subimg histograms into the overall histogram
            unsigned int binsOffset = 0;
            for (unsigned int subimgIndex = 0; subimgIndex < subimgHistograms.size(); ++subimgIndex) {
                const EHDFeat::bins_vect_t& subimgH = subimgHistograms[subimgIndex];

                for (unsigned int i = 0; i < subimgH.size(); ++i) {
                    binsRef[binsOffset] = subimgH[i];
                    ++binsOffset;
                }
            }

            return new EHDFeat(bins);
        }
        catch (...) {
            Utils::Memory::SafeDelete(bins);
            throw;
        }
    }

    void EHDFE::DivideIntoSubImgs(const Core::ImgInfo& imgInfo, REF subimg_matrix_t& subimgs) const {
        const unsigned int N = NUMBER_OF_SUBIMGS_IN_ROW;

        const Magick::Image& img = imgInfo.GetMagickImage();

        unsigned int imgWidth = img.size().width();
        unsigned int imgHeight = img.size().height();

        const Magick::PixelPacket* imgPixels = img.getConstPixels(0, 0, imgWidth, imgHeight);

        unsigned int blockWidth = imgWidth / N;
        unsigned int blockHeight = imgHeight / N;

        unsigned int x;
        unsigned int y;

        y = 0;
        for (unsigned int i = 0; i < N - 1; ++i) {
            x = 0;
            for (unsigned int j = 0; j < N - 1; ++j) {
                SubImg& subImg = subimgs(i, j);
                subImg.Init(x, y, blockWidth, blockHeight, imgWidth, imgPixels);
                x += blockWidth;
            }

            SubImg& lastBlockInRow = subimgs(i, N - 1);
            lastBlockInRow.Init(x, y, imgWidth - x, blockHeight, imgWidth, imgPixels);

            y += blockHeight;
        }

        x = 0;
        for (unsigned int j = 0; j < N - 1; ++j) {
            SubImg& bottomBlock = subimgs(N - 1, j);
            bottomBlock.Init(x, y, blockWidth, imgHeight - y, imgWidth, imgPixels);

            x += blockWidth;
        }

        SubImg& cornerBlock = subimgs(N - 1, N - 1);
        cornerBlock.Init(x, y, imgWidth - x, imgHeight - y, imgWidth, imgPixels);
    }

    void EHDFE::ExtractSubImgsHistograms(const subimg_matrix_t& subimgs, OUT std::vector<EHDFeat::bins_vect_t>& subimgHistograms) const {
        subimgHistograms.clear();
        subimgHistograms.reserve(subimgs.Rows() * subimgs.Columns());

        size_t rows = subimgs.Rows();
        size_t columns = subimgs.Columns();
        for (unsigned int row = 0; row < rows; ++row) {
            for (unsigned int column = 0; column < columns; ++column) {
                EHDFeat::bins_vect_t h = subimgs(row, column).CalculateEdgeHistogram();
                subimgHistograms.push_back(h);
            }
        }
    }
}
}
}