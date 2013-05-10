#include "core/IndexNode.h"
#include "utils/ContractUtils.h"

namespace ImgDetective {
namespace Core {

    CTOR IndexNode::IndexNode()
        : imgId(IMGID_UNDEFINED), feature(NULL) {}

    CTOR IndexNode::IndexNode(imgid_t imgId, IFeature* feature) {
        Utils::Contract::Assert(imgId != IMGID_UNDEFINED);
        Utils::Contract::AssertNotNull(feature);

        this->imgId = imgId;
        this->feature = feature;
    }

}
}