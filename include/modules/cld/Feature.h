#pragma once

#include "common/CommonInternal.h"
#include "core/Feature.h"

#include <vector>

#define CLD_FTYPE_ID 2 /*Feature type ID of color histogram feature*/
#define NUMBER_OF_IMG_BLOCKS_IN_ROW 8
#define NUMBER_OF_DCT_COEFS NUMBER_OF_IMG_BLOCKS_IN_ROW * NUMBER_OF_IMG_BLOCKS_IN_ROW

namespace ImgDetective {
namespace Modules {
namespace CLD {

    class CLDFeat : NONCOPYABLE, public Core::Feature {
    public:
        typedef double dct_coeff_t;
        typedef std::vector<dct_coeff_t> dct_coeff_vect_t;

        CTOR CLDFeat(dct_coeff_vect_t* dctY, dct_coeff_vect_t* dctCb, dct_coeff_vect_t* dctCr);
        ~CLDFeat();

        virtual Core::FeatureDistance ComputeDistanceInternal(const REF IFeature& feature) const;

        static CLDFeat* Deserialize(const Core::blob_t& blob);
        virtual Core::blob_p_t Serialize() const;
    private:
        dct_coeff_vect_t* dctY;
        dct_coeff_vect_t* dctCb;
        dct_coeff_vect_t* dctCr;

        static void ReadDctCoeffsFromBuffer(const Core::blob_t& buffer, REF size_t& offset, REF dct_coeff_vect_t& dctCoeffs);
        static void WriteDctCoeffsToBuffer(REF Core::blob_t& buffer, REF size_t& offset, const dct_coeff_vect_t& dctCoeffs);

        static double ComputeDistanceBtwChannelCoefs(const dct_coeff_vect_t& c1, const dct_coeff_vect_t& c2, unsigned int usedCoefsNumber);
    };

}
}
}