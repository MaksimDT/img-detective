#pragma once

#include "common/CommonInternal.h"
#include "core/FeatureDistance.h"
#include "core/Feature.h"

#include <list>

namespace ImgDetective {
namespace Core {

	//collection of pairs <image_id, feature_distance> sorted in descending order by the feature distance field
	CONCRETE class IndexSeekResult {
	public:
		typedef NESTED struct entry_t {
			entry_t(imgid_t imgId, FeatureDistance distance);

			imgid_t imgId;
			FeatureDistance distance;
		};

		typedef std::list<entry_t> entry_col_t;

		CTOR IndexSeekResult(Feature::type_id_t featureTypeId);
		Feature::type_id_t GetFeatureTypeId() const;

		//inserts the specified entry in the underlying sorted collection
		void Insert(imgid_t imgId, const REF FeatureDistance& distance);
		/*returns the reference to entries collection sorted in 
		ascending order by distance to the query image's feature*/
		const REF entry_col_t& GetEntries() const;
		void ApplyWeightCoeff(double coeff);
	private:
		Feature::type_id_t featureTypeId;
		entry_col_t entries;
	};

}
}