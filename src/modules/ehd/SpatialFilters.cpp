#include "modules/ehd/SpatialFilters.h"
#include "utils/ContractUtils.h"

namespace ImgDetective {
namespace Modules {
namespace EHD {

    CTOR SpatialFilters::SpatialFilters()
        : vertical(4), horizontal(4), diagonal45(4), diagonal135(4), nondirectional(4) {
        Init();
    }

    void SpatialFilters::Init() {
        vertical[0] = 1;
        vertical[1] = -1;
        vertical[2] = 1;
        vertical[3] = -1;

        horizontal[0] = 1;
        horizontal[1] = 1;
        horizontal[2] = -1;
        horizontal[3] = -1;

        diagonal45[0] = sqrt(2);
        diagonal45[1] = 0;
        diagonal45[2] = 0;
        diagonal45[3] = -sqrt(2);

        diagonal135[0] = 0;
        diagonal135[1] = sqrt(2);
        diagonal135[2] = -sqrt(2);
        diagonal135[3] = 0;

        nondirectional[0] = 2;
        nondirectional[1] = -2;
        nondirectional[2] = -2;
        nondirectional[3] = 2;
    }

    const SpatialFilters::filter_t& SpatialFilters::Get(EHDFeat::EdgeType edgeType) const {
        Utils::Contract::Assert(edgeType != EHDFeat::EdgeType::Monotone);

        //TODO: rewrite using table method instead of the switch

        switch (edgeType)
        {
        case ImgDetective::Modules::EHD::EHDFeat::Vertical:
            return vertical;
        case ImgDetective::Modules::EHD::EHDFeat::Horizontal:
            return horizontal;
        case ImgDetective::Modules::EHD::EHDFeat::Diagonal45:
            return diagonal45;
        case ImgDetective::Modules::EHD::EHDFeat::Diagonal135:
            return diagonal135;
        case ImgDetective::Modules::EHD::EHDFeat::Nondirectional:
            return nondirectional;
        default:
            throw std::runtime_error("Cannot obtain a spatial filter: unknown edge type was specified");
        }
    }

}
}
}