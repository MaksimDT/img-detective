#pragma once

#include <vector>
#include "common/CommonInternal.h"
#include "core/FeatureDistance.h"

namespace ImgDetective {
namespace Core {

    class IFeature : HAS_VIRTUAL_DESCTUCTOR {
    public:
        typedef int type_id_t;

		/*type of collection that should be used to transfer objects of this type across 
		methods boundaries (i.e. for params and return values). "p" means that elements 
		of collection is pointers*/
		typedef std::vector<IFeature*> col_p_t;

        virtual type_id_t GetTypeId() const = 0;
        virtual blob_p_t Serialize() const = 0;
        //computes a distance between two features
		virtual FeatureDistance ComputeDistanceTo(const REF IFeature& feature) const = 0;
    };

	ABSTRACT class Feature : public IFeature {
	public:
		virtual type_id_t GetTypeId() const;
		virtual FeatureDistance ComputeDistanceTo(const REF IFeature& feature) const;
	protected:
		CTOR Feature(type_id_t typeId);

		virtual FeatureDistance ComputeDistanceInternal(const REF IFeature& feature) const = 0;
	private:
		type_id_t typeId;
	};
}
}