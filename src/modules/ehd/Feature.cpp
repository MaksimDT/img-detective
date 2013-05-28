#include "modules/ehd/Feature.h"
#include "utils/ContractUtils.h"
#include "utils/MemoryUtils.h"

namespace ImgDetective {
namespace Modules {
namespace EHD {

    CTOR EHDFeat::EHDFeat(bins_vect_t* bins)
        : Core::Feature(EHD_FTYPE_ID) {

        Utils::Contract::AssertNotNull(bins);
        Utils::Contract::Assert(bins->size() == TOTAL_NUMBER_OF_BINS);

        this->bins = bins;
    }

    EHDFeat::~EHDFeat() {
        Utils::Memory::SafeDelete(bins);
    }

    Core::FeatureDistance EHDFeat::ComputeDistanceInternal(const REF IFeature& feature) const {
        Utils::Contract::Assert(feature.GetTypeId() == this->GetTypeId());

        //sum of bin values in each subimg's histogram = number of img blocks in this subimg
        const unsigned long maxDiff = NUMBER_OF_IMG_BLOCKS_IN_SUB_IMG * TOTAL_NUMBER_OF_SUBIMGS;

        EHDFeat& other = (EHDFeat&)feature;

        EHDFeat::bins_vect_t& thisBins = *this->bins;
        EHDFeat::bins_vect_t& otherBins = *other.bins;

        if (thisBins.size() != otherBins.size()) {
            return Core::FeatureDistance(0);
        }

        unsigned long diffSum = 0;
        for (int i = 0; i < thisBins.size(); ++i) {
            diffSum += labs(thisBins[i] - otherBins[i]);
        }

        double distance = (double)diffSum / maxDiff;

        return Core::FeatureDistance(distance);
    }

    EHDFeat* EHDFeat::Deserialize(const Core::blob_t& blob) {
        size_t expectedBufferLen = TOTAL_NUMBER_OF_BINS * sizeof(bin_value_t);
        Utils::Contract::Assert(blob.size() == expectedBufferLen);

        EHDFeat::bins_vect_t* bins = new EHDFeat::bins_vect_t(TOTAL_NUMBER_OF_BINS);
        EHDFeat::bins_vect_t& binsRef = *bins;

        size_t bufferOffset = 0;
        for (unsigned int binIndex = 0; binIndex < TOTAL_NUMBER_OF_BINS; ++binIndex) {
            EHDFeat::bin_value_t binValue = Core::ReadFromBlob<EHDFeat::bin_value_t>(blob, bufferOffset);
            bufferOffset += sizeof(EHDFeat::bin_value_t);
            binsRef[binIndex] = binValue;
        }

        return new EHDFeat(bins);
    }

    Core::blob_p_t EHDFeat::Serialize() const {
        size_t bufferLen = TOTAL_NUMBER_OF_BINS * sizeof(bin_value_t);

        Core::blob_p_t buffer = NULL;
        try {
            buffer = Core::CreateBlobOfSize(bufferLen);
            EHDFeat::bins_vect_t& binsRef = *this->bins;

            size_t bufferOffset = 0;
            for (unsigned int binIndex = 0; binIndex < TOTAL_NUMBER_OF_BINS; ++binIndex) {
                EHDFeat::bin_value_t binValue = binsRef[binIndex];
                Core::WriteToBlob(*buffer, bufferOffset, binValue);
                bufferOffset += sizeof(EHDFeat::bin_value_t);
            }

            return buffer;
        }
        catch (...) {
            Core::SafeFreeBlob(buffer);
            throw;
        }
    }
}
}
}