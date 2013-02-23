#pragma once

#include "IndexManager.h"
#include "CommonInternal.h"

namespace ImgDetective {
namespace Core {

	//performs linear search in an unsorted set of features.
	//reads features packet by packet from some data source
	CONCRETE SEALED class LinearSearchIM : public IndexManager {
	public:
		LinearSearchIM(IndexStorage* storage, Feature::type_id_t featureTypeId);

		virtual IndexSeekResult* Search(REF Feature& f, const REF ImgQuery& query) const;
	protected:
		void ProcessPacket(REF Feature& exampleFeature, REF IndexNode::col_p_t& packet, REF IndexSeekResult& result) const;
	};

}
}