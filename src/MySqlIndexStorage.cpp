#pragma once

#include "MySqlIndexStorage.h"
#include "ContractUtils.h"

using namespace std;

namespace ImgDetective {
namespace Core {

	MySqlIndexStorage::MySqlIndexStorage(IFeatureDeserializer* featureDeserializer, string connectionString, unsigned int packetSize) 
		: IndexStorage(featureDeserializer) {
		Utils::Contract::AssertNotEmpty(connectionString);
		Utils::Contract::Assert(packetSize > 0);

		this->connectionString = connectionString;
		this->packetSize = packetSize;
	}

	IndexStorage::LookupSessionBase* MySqlIndexStorage::StartLookup() const {
		return new MySqlIndexStorage::LookupSession(*featureDeserializer, packetSize);
	}

	MySqlIndexStorage::LookupSession::LookupSession(const REF IFeatureDeserializer& featureDeserializer, unsigned int packetSize) 
		: LookupSessionBase(featureDeserializer) {
		Utils::Contract::Assert(packetSize > 0);

		this->packetSize = packetSize;
	}
}
}