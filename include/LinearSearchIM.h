#pragma once

#include "IndexManager.h"
#include "CommonInternal.h"

namespace ImgDetective {
namespace Core {

	//performs linear search in an unsorted set of features
	class LinearSearchIM : public IndexManager {
	public:
		virtual IndexSeekResult* Search(REF Feature& f, const REF ImgQuery& query) const;
	protected:
		LinearSearchIM(Feature::type_id_t featureTypeId, unsigned int packetSize);
		virtual IndexNode::col_p_t GetNextPacket(unsigned int packetSize, unsigned int packetNumber) const = 0;
		void ProcessPacket(REF Feature& exampleFeature, REF IndexNode::col_p_t& packet, REF IndexSeekResult& result) const;

		unsigned int packetSize;
	};

}
}