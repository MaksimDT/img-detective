#pragma once

#include "CommonInternal.h"
#include "Feature.h"
#include "ImgDetectiveApi.h"
#include "IndexSeekResult.h"
#include "IndexStorage.h"

namespace ImgDetective {
namespace Core {

	//contains logic of sorting features by similarity to feature of query image
	ABSTRACT class IndexManager {
	public:
		typedef std::vector<IndexManager*> col_p_t;

		virtual IndexSeekResult* Search(REF Feature& f, const REF ImgQuery& query) const = 0;
		Feature::type_id_t GetFeatureTypeId() const;
	protected:
		IndexManager(IndexStorage* storage, Feature::type_id_t featureTypeId);
		virtual ~IndexManager();

		IndexStorage* storage;
	private:
		Feature::type_id_t featureTypeId;
	};

}
}