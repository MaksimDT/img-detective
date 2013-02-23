#include "IndexSeekResult.h"

namespace ImgDetective {
namespace Core {

	IndexSeekResult::IndexSeekResult(Feature::type_id_t featureTypeId) {
		this->featureTypeId = featureTypeId;
	}

	Feature::type_id_t IndexSeekResult::GetFeatureTypeId() const {
		return featureTypeId;
	}

	void IndexSeekResult::Insert(imgid_t imgId, FeatureDistance distance) {
		//inserting the specified entry into the sorted collection of entries

		entry_t newEntry(imgId, distance);
		newEntry.imgId = imgId;
		newEntry.distance = distance;

		entry_col_t::iterator it;
		bool inserted = false;
		for (it = entries.begin(); it != entries.end(); ++it) {
			if (newEntry.distance < it->distance) {
				entries.insert(it, 1, newEntry);
				inserted = true;
				break;
			}
		}

		if (!inserted) {
			entries.insert(entries.end(), 1, newEntry);
		}
	}

	const REF IndexSeekResult::entry_col_t& IndexSeekResult::GetEntries() const {
		return entries;
	}

	void IndexSeekResult::ApplyWeightCoeff(double coeff) {
		entry_col_t::iterator it;
		for (it = entries.begin(); it != entries.end(); ++it) {
			it->distance.Multiply(coeff);
		}
	}
}
}