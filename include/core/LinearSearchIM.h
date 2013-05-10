#pragma once

#include "common/CommonInternal.h"
#include "core/IndexManager.h"

namespace ImgDetective {
namespace Core {

	//performs linear search in an unsorted set of features.
	//reads features packet by packet from some data source
	CONCRETE SEALED class LinearSearchIM : public IndexManager {
	public:
		CTOR LinearSearchIM(EXCLUSIVE IIndexStorage* storage, IFeature::type_id_t featureTypeId);

		virtual IndexSeekResult* Search(REF IFeature& f, const REF ImgQuery& query) const;
	private:
		void ProcessPacket(const IFeature& exampleFeature, const IndexNode::col_t& packet, REF IndexSeekResult& result) const;
        void FreePacket(REF IndexNode::col_t& packet) const;
	};

}
}