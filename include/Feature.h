#pragma once

#include <vector>
#include <memory>
#include <boost/shared_ptr.hpp>

#include "FeatureDistance.h"
#include "CommonInternal.h"

namespace ImgDetective {
namespace Core {

	class Feature {
	public:
		typedef int type_id_t;

		/*type of collection that should be used to transfer objects of this type across 
		methods boundaries (i.e. for params and return values). "p" means that elements 
		of collection is pointers*/
		typedef std::vector<Feature*> col_p_t;

		type_id_t GetTypeId() const;
		//computes a distance between two features
		FeatureDistance ComputeDistanceTo(Feature& feature) const;

		virtual blob_p_t Serialize() const = 0;
	protected:
		FeatureDistance ComputeDistanceInternal(Feature& feature) const;
	private:
		type_id_t typeId;
	};
}
}