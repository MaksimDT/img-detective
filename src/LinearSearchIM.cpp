#include "LinearSearchIM.h"

namespace ImgDetective {
namespace Core {

	IndexSeekResult* LinearSearchIM::Search(REF Feature& f, const REF ImgQuery& query) const {
		IndexSeekResult* result = new IndexSeekResult(this->GetFeatureTypeId());

		IndexNode::col_p_t curPacket;
		unsigned int curPacketNumber = 1;

		while ((curPacket = storage->GetNextPacket(packetSize, curPacketNumber)).size() != 0) {
			ProcessPacket(REF f, REF curPacket, REF *result);
			++curPacketNumber;
		}

		return result;
	}

	void LinearSearchIM::ProcessPacket(REF Feature& exampleFeature, REF IndexNode::col_p_t& packet, REF IndexSeekResult& result) const {
		IndexNode::col_p_t::const_iterator it;

		for (it = packet.begin(); it != packet.end(); ++it) {
			IndexNode* nodePtr = *it;
			Feature* curFeature = nodePtr->feature;
			FeatureDistance distance = curFeature->ComputeDistanceTo(exampleFeature);
			result.Insert(nodePtr->imgId, distance);
		}
	}

}
}