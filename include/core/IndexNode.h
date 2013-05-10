#pragma once

#include <vector>
#include "common/CommonInternal.h"
#include "core/Feature.h"

namespace ImgDetective {
namespace Core {

    class IndexNode {
    public:
        typedef std::vector<IndexNode> col_t;

        CTOR IndexNode();
        CTOR IndexNode(imgid_t imgId, IFeature* feature);

        imgid_t imgId;
        IFeature* feature;
    };

}
}