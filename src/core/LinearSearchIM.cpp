#include "core/LinearSearchIM.h"
#include "utils/MemoryUtils.h"

namespace ImgDetective {
namespace Core {

	CTOR LinearSearchIM::LinearSearchIM(EXCLUSIVE IIndexStorage* storage, IFeature::type_id_t featureTypeId)
		: IndexManager(storage, featureTypeId) { }
	
	IndexSeekResult* LinearSearchIM::Search(REF IFeature& f, const REF ImgQuery& query) const {
		IndexSeekResult* result = new IndexSeekResult(this->GetFeatureTypeId());

        IIndexStorage::ILookupSession* lookupSession = storage->StartLookup();
        IndexNode::col_t curPacket;

		try {
            while (true) {
                curPacket = lookupSession->GetNextPacket();
                bool finished = curPacket.empty();
                if (finished) {
                    break;
                }

                ProcessPacket(f, curPacket, REF *result);
                FreePacket(REF curPacket);
            }

			Utils::Memory::SafeDelete(lookupSession);
		}
		catch (...) {
            Utils::Memory::SafeDelete(lookupSession);
            FreePacket(curPacket);
		}

		return result;
	}

	void LinearSearchIM::ProcessPacket(const IFeature& exampleFeature, const IndexNode::col_t& packet, REF IndexSeekResult& result) const {
		IndexNode::col_t::const_iterator it;

		for (it = packet.begin(); it != packet.end(); ++it) {
			IFeature* curFeature = it->feature;
			FeatureDistance distance = curFeature->ComputeDistanceTo(exampleFeature);
			result.Insert(it->imgId, distance);
		}
	}

    void LinearSearchIM::FreePacket(REF IndexNode::col_t& packet) const {
        IndexNode::col_t::iterator it;

        for (it = packet.begin(); it != packet.end(); ++it) {
            if (it->feature != NULL) {
                delete it->feature;
                it->feature = NULL;
            }
        }
    }
}
}