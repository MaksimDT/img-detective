#include "modules/cld/Feature.h"
#include "utils/ContractUtils.h"
#include "utils/MemoryUtils.h"

namespace ImgDetective {
namespace Modules {
namespace CLD {

    CTOR CLDFeat::CLDFeat(dct_coeff_vect_t* dctY, dct_coeff_vect_t* dctCb, dct_coeff_vect_t* dctCr)
        : Core::Feature(CLD_FTYPE_ID) {
        Utils::Contract::AssertNotNull(dctY);
        Utils::Contract::AssertNotNull(dctCb);
        Utils::Contract::AssertNotNull(dctCr);
        Utils::Contract::Assert(dctY->size() == NUMBER_OF_DCT_COEFS);
        Utils::Contract::Assert(dctCb->size() == NUMBER_OF_DCT_COEFS);
        Utils::Contract::Assert(dctCr->size() == NUMBER_OF_DCT_COEFS);

        this->dctY = dctY;
        this->dctCb = dctCb;
        this->dctCr = dctCr;
    }

    CLDFeat::~CLDFeat() {
        Utils::Memory::SafeDelete(dctY);
        Utils::Memory::SafeDelete(dctCb);
        Utils::Memory::SafeDelete(dctCr);
    }

    Core::FeatureDistance CLDFeat::ComputeDistanceInternal(const REF IFeature& feature) const {
        Utils::Contract::Assert(feature.GetTypeId() == this->GetTypeId());

        CLDFeat& other = (CLDFeat&)feature;

        const double normFactor = 18 * sqrt(NUMBER_OF_DCT_COEFS);  //computed analytically

        double yDistance = ComputeDistanceBtwChannelCoefs(*(this->dctY), *(other.dctY));
        double cbDistance = ComputeDistanceBtwChannelCoefs(*(this->dctCb), *(other.dctCb));
        double crDistance = ComputeDistanceBtwChannelCoefs(*(this->dctCr), *(other.dctCr));

        double featureDistance = (yDistance + cbDistance + crDistance) / normFactor;

        return Core::FeatureDistance(featureDistance);
    }

    CLDFeat* CLDFeat::Deserialize(const Core::blob_t& blob) {
        size_t expectedBufferLen = 3 * NUMBER_OF_DCT_COEFS * sizeof(dct_coeff_t);

        Utils::Contract::Assert(blob.size() == expectedBufferLen);

        dct_coeff_vect_t* dctY = NULL;
        dct_coeff_vect_t* dctCb = NULL;
        dct_coeff_vect_t* dctCr = NULL;

        try {
            dctY = new dct_coeff_vect_t();
            dctY->resize(NUMBER_OF_DCT_COEFS);
            dctCb = new dct_coeff_vect_t();
            dctCb->resize(NUMBER_OF_DCT_COEFS);
            dctCr = new dct_coeff_vect_t();
            dctCr->resize(NUMBER_OF_DCT_COEFS);

            size_t bufferOffset = 0;
            ReadDctCoeffsFromBuffer(blob, REF bufferOffset, REF *dctY);
            ReadDctCoeffsFromBuffer(blob, REF bufferOffset, REF *dctCb);
            ReadDctCoeffsFromBuffer(blob, REF bufferOffset, REF *dctCr);

            return new CLDFeat(dctY, dctCb, dctCr);
        }
        catch (...) {
            Utils::Memory::SafeDelete(dctY);
            Utils::Memory::SafeDelete(dctCb);
            Utils::Memory::SafeDelete(dctCr);
            throw;
        }
    }

    Core::blob_p_t CLDFeat::Serialize() const {
        size_t bufferLen = 3 * NUMBER_OF_DCT_COEFS * sizeof(dct_coeff_t);

        Core::blob_p_t buffer = NULL;

        try {
            buffer = Core::CreateBlobOfSize(bufferLen);

            size_t bufferOffset = 0;
            WriteDctCoeffsToBuffer(REF *buffer, REF bufferOffset, *dctY);
            WriteDctCoeffsToBuffer(REF *buffer, REF bufferOffset, *dctCb);
            WriteDctCoeffsToBuffer(REF *buffer, REF bufferOffset, *dctCr);

            return buffer;
        }
        catch (...) {
            Core::SafeFreeBlob(buffer);
            throw;
        }
    }

    void CLDFeat::ReadDctCoeffsFromBuffer(const Core::blob_t& buffer, REF size_t& offset, REF dct_coeff_vect_t& dctCoeffs) {
        Utils::Contract::Assert(buffer.size() >= offset + dctCoeffs.size() * sizeof(dct_coeff_t));

        size_t coeffIndex = 0;
        while (coeffIndex < dctCoeffs.size()) {
            dctCoeffs[coeffIndex] = Core::ReadFromBlob<dct_coeff_t>(buffer, offset);
            offset += sizeof(dct_coeff_t);
            ++coeffIndex;
        }
    }

    void CLDFeat::WriteDctCoeffsToBuffer(REF Core::blob_t& buffer, REF size_t& offset, const dct_coeff_vect_t& dctCoeffs) {
        Utils::Contract::Assert(buffer.size() >= offset + dctCoeffs.size() * sizeof(dct_coeff_t));

        size_t coeffIndex = 0;
        while (coeffIndex < dctCoeffs.size()) {
            Core::WriteToBlob(buffer, offset, dctCoeffs[coeffIndex]);
            offset += sizeof(dct_coeff_t);
            ++coeffIndex;
        }
    }

    double CLDFeat::ComputeDistanceBtwChannelCoefs(const dct_coeff_vect_t& c1, const dct_coeff_vect_t& c2) {
        Utils::Contract::Assert(c1.size() == c2.size());
        
        const dct_coeff_t* c1raw = c1.data();
        const dct_coeff_t* c2raw = c2.data();

        double sum = 0.0;
        for (unsigned int i = 0; i < c1.size(); ++i) {
            double diff = c1raw[i] - c2raw[i];
            sum += diff * diff;
        }

        return sqrt(sum);
    }
}
}
}