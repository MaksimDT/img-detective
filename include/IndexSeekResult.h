#pragma once

#include "CommonInternal.h"
#include "FeatureDistance.h"

#include <vector>
#include <list>

namespace ImgDetective {
namespace Core {

	//collection of pairs <image_id, feature_distance> sorted in descending order by the feature distance field
	class IndexSeekResult {
	public:
		typedef struct entry_t {
			imgid_t imgId;
			FeatureDistance distance;
		};

		typedef std::list<entry_t> entry_col;

		IndexSeekResult(Feature::type_id_t featureTypeId);
		Feature::type_id_t GetFeatureTypeId() const;

		//inserts the specified entry in the underlying sorted collection
		void Insert(imgid_t imgId, FeatureDistance distance);
		const REF entry_col& GetEntries() const;
		void ApplyWeightCoeff(double coeff);
	private:
		Feature::type_id_t featureTypeId;
		entry_col entries;
	};

}
}