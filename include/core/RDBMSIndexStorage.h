#pragma once

#include "core/IndexStorage.h"
#include "core/mysqlheaders.h"
#include "core/DbWrapper.h"

#include <string>

namespace ImgDetective {
namespace Core {

	CONCRETE SEALED class RDBMSIndexStorage : public IndexStorage {
	public:
        #pragma region Lookup session

		CONCRETE SEALED NESTED class LookupSession : public LookupSessionBase {
		public:
			LookupSession(const REF IFeatureDeserializer& featureDeserializer, unsigned int packetSize);
            ~LookupSession();
			virtual bool GetNextPacket(REF IndexNode::col_p_t& packet);
		private:
            class Impl;
            Impl* pimpl;
		};

        #pragma endregion

		CTOR RDBMSIndexStorage(EXCLUSIVE IFeatureDeserializer* featureDeserializer, const REF DbWrapper& dbWrapper, unsigned int packetSize);
        ~RDBMSIndexStorage();
        virtual ILookupSession* StartLookup() const;
        virtual void AddFeature(const REF IFeature& feature, imgid_t imgId);
	private:
        class Impl;
        Impl* pimpl;
	};
}
}