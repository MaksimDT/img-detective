#include "core/FeatureExtractor.h"

namespace ImgDetective {
namespace Core {

    CTOR FeatureExtractor::FeatureExtractor(IFeature::type_id_t featureTypeId) 
        : featureTypeId(featureTypeId) { }

    IFeature::type_id_t FeatureExtractor::GetFeatureTypeId() const {
        return featureTypeId;
    }
}
}