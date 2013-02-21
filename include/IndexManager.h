#pragma once

#include "CommonInternal.h"
#include "Feature.h"
#include "ImgDetectiveApi.h"
#include "IndexSeekResult.h"

namespace ImgDetective {
namespace Core {

	class IndexManager {
	public:
		typedef std::vector<IndexManager*> col_p_t;

		virtual IndexSeekResult* Search(REF Feature& f, const REF ImgQuery& query) const = 0;
		Feature::type_id_t GetFeatureTypeId() const;
	protected:
		IndexManager(Feature::type_id_t featureTypeId);
	private:
		Feature::type_id_t featureTypeId;
	};

}
}