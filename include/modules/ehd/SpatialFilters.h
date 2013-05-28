#pragma once

#include "common/CommonInternal.h"
#include "common/Matrix.h"
#include "modules/ehd/Feature.h"

#include <vector>

namespace ImgDetective {
namespace Modules {
namespace EHD {

    class SpatialFilters {
    public:
        typedef std::vector<double> filter_t;

        CTOR SpatialFilters();
        void Init();

        const filter_t& Get(EHDFeat::EdgeType edgeType) const;
    private:
        filter_t vertical;
        filter_t horizontal;
        filter_t diagonal45;
        filter_t diagonal135;
        filter_t nondirectional;
    };

}
}
}