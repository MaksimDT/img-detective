#pragma once

#include <vector>

#include "common/CommonInternal.h"
#include "core/FeatureDistance.h"

namespace ImgDetective {
namespace Core {

	ABSTRACT class Feature {
	public:
		typedef int type_id_t;

		/*type of collection that should be used to transfer objects of this type across 
		methods boundaries (i.e. for params and return values). "p" means that elements 
		of collection is pointers*/
		typedef std::vector<Feature*> col_p_t;

		type_id_t GetTypeId() const;
		//computes a distance between two features
		FeatureDistance ComputeDistanceTo(const REF Feature& feature) const;

		virtual blob_p_t Serialize() const = 0;
	protected:
		CTOR Feature(type_id_t typeId);

		virtual FeatureDistance ComputeDistanceInternal(const REF Feature& feature) const = 0;
	private:
		type_id_t typeId;
	};
}
}