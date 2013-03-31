#include "core/LinearSearchIM.h"
#include "utils/MemoryUtils.h"

namespace ImgDetective {
namespace Core {

	CTOR LinearSearchIM::LinearSearchIM(EXCLUSIVE IIndexStorage* storage, IFeature::type_id_t featureTypeId)
		: IndexManager(storage, featureTypeId) { }
	
	IndexSeekResult* LinearSearchIM::Search(REF IFeature& f, const REF ImgQuery& query) const {
		IndexSeekResult* result = new IndexSeekResult(this->GetFeatureTypeId());

        IIndexStorage::ILookupSession* lookupSession = storage->StartLookup();

		try {
			IndexNode::col_p_t curPacket;

			while (lookupSession->GetNextPacket(REF curPacket)) {
				ProcessPacket(REF f, REF curPacket, REF *result);
			}

			Utils::Memory::SafeDelete(lookupSession);
		}
		catch (...) {
			try {
				Utils::Memory::SafeDelete(lookupSession);
			}
			catch (...) {
				//TODO: logging
			}
			throw;
		}

		return result;
	}

	void LinearSearchIM::ProcessPacket(REF IFeature& exampleFeature, REF IndexNode::col_p_t& packet, REF IndexSeekResult& result) const {
		IndexNode::col_p_t::const_iterator it;

		for (it = packet.begin(); it != packet.end(); ++it) {
			IndexNode* nodePtr = *it;
			IFeature* curFeature = nodePtr->feature;
			FeatureDistance distance = curFeature->ComputeDistanceTo(exampleFeature);
			result.Insert(nodePtr->imgId, distance);
		}
	}

}
}