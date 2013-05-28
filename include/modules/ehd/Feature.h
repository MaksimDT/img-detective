#pragma once

#include "common/CommonInternal.h"
#include "core/Feature.h"

#include <vector>

#define EHD_FTYPE_ID 3 /*Feature type ID of edge histogram feature*/
#define NUMBER_OF_EDGE_TYPES 5
#define NUMBER_OF_SUBIMGS_IN_ROW 4
#define TOTAL_NUMBER_OF_SUBIMGS NUMBER_OF_SUBIMGS_IN_ROW * NUMBER_OF_SUBIMGS_IN_ROW
#define TOTAL_NUMBER_OF_BINS NUMBER_OF_EDGE_TYPES * TOTAL_NUMBER_OF_SUBIMGS
#define NUMBER_OF_IMG_BLOCKS_IN_SUB_IMG_ROW 16
#define NUMBER_OF_IMG_BLOCKS_IN_SUB_IMG NUMBER_OF_IMG_BLOCKS_IN_SUB_IMG_ROW * NUMBER_OF_IMG_BLOCKS_IN_SUB_IMG_ROW

namespace ImgDetective {
namespace Modules {
namespace EHD {

    class EHDFeat : NONCOPYABLE, public Core::Feature {
    public:
        enum EdgeType {
            Monotone = -1,
            Vertical = 0,
            Horizontal = 1,
            Diagonal45 = 2,
            Diagonal135 = 3,
            Nondirectional = 4
        };

        typedef unsigned short bin_value_t;
        typedef std::vector<bin_value_t> bins_vect_t;

        CTOR EHDFeat(bins_vect_t* bins);
        ~EHDFeat();

        virtual Core::FeatureDistance ComputeDistanceInternal(const REF IFeature& feature) const;

        static EHDFeat* Deserialize(const Core::blob_t& blob);
        virtual Core::blob_p_t Serialize() const;
    private:
        bins_vect_t* bins;
    };

}
}
}