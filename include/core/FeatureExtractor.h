#pragma once

#include "common/CommonInternal.h"
#include "core/Feature.h"
#include "core/ImgInfo.h"
#include <vector>

namespace ImgDetective {
namespace Core {

    class IFeatureExtractor : NONCOPYABLE, HAS_VIRTUAL_DESCTUCTOR {
    public:
        typedef std::vector<IFeatureExtractor*> col_p_t;

        virtual IFeature* ExtractFrom(const REF ImgInfo& imgInfo) const = 0;
    };

    ABSTRACT class FeatureExtractor : public IFeatureExtractor {
	protected:
        CTOR FeatureExtractor(IFeature::type_id_t featureTypeId);
        IFeature::type_id_t GetFeatureTypeId() const;
    private:
        IFeature::type_id_t featureTypeId;
	};
}
}