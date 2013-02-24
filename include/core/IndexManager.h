#pragma once

#include "common/CommonInternal.h"
#include "core/Feature.h"
#include "core/IndexSeekResult.h"
#include "core/IndexStorage.h"

namespace ImgDetective {
namespace Core {

	//contains logic of sorting features by similarity to feature of query image
	ABSTRACT NONCOPYABLE class IndexManager {
	public:
		typedef std::vector<IndexManager*> col_p_t;

		virtual ~IndexManager();

		virtual IndexSeekResult* Search(REF Feature& f, const REF ImgQuery& query) const = 0;
		Feature::type_id_t GetFeatureTypeId() const;
	protected:
		CTOR IndexManager(EXCLUSIVE IndexStorage* storage, Feature::type_id_t featureTypeId);
		
		EXCLUSIVE IndexStorage* storage;
	private:
		COPYCTOR IndexManager(const REF IndexManager& other);

		Feature::type_id_t featureTypeId;
	};

}
}