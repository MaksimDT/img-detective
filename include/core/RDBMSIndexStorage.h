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
			LookupSession(const REF IFeatureDeserializer& featureDeserializer, const Db::DbWrapper& dbWrapper, unsigned int packetSize, const std::string& tableName);
            ~LookupSession();
			virtual IndexNode::col_t GetNextPacket();
		private:
            class Impl;
            Impl* pimpl;
		};

        #pragma endregion

		CTOR RDBMSIndexStorage(EXCLUSIVE IFeatureDeserializer* featureDeserializer, const Db::DbWrapper& dbWrapper, unsigned int packetSize, const std::string& tableName);
        ~RDBMSIndexStorage();
        virtual ILookupSession* StartLookup() const;
        virtual void AddFeature(const REF IFeature& feature, imgid_t imgId) const;
	private:
        class Impl;
        Impl* pimpl;
	};
}
}