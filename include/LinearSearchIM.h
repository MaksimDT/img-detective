#pragma once

#include "IndexManager.h"
#include "CommonInternal.h"
#include "IIndexStorage.h"

namespace ImgDetective {
namespace Core {

	//performs linear search in an unsorted set of features.
	//reads features packet by packet from some data source
	class LinearSearchIM : public IndexManager {
	public:
		virtual IndexSeekResult* Search(REF Feature& f, const REF ImgQuery& query) const;
	protected:
		LinearSearchIM(IIndexStorage* storage, Feature::type_id_t featureTypeId, unsigned int packetSize);
		void ProcessPacket(REF Feature& exampleFeature, REF IndexNode::col_p_t& packet, REF IndexSeekResult& result) const;

		unsigned int packetSize;
		IIndexStorage* storage;
	};

}
}