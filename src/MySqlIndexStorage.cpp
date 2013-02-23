#pragma once

#include "MySqlIndexStorage.h"
#include "ContractUtils.h"

using namespace std;

namespace ImgDetective {
namespace Core {

	MySqlIndexStorage::MySqlIndexStorage(IFeatureDeserializer* featureDeserializer, string connectionString, unsigned int packetSize) {
		Utils::Contract::AssertNotNull(featureDeserializer);
		Utils::Contract::AssertNotEmpty(connectionString);
		Utils::Contract::Assert(packetSize > 0);

		this->featureDeserializer = featureDeserializer;
		this->connectionString = connectionString;
		this->packetSize = packetSize;
	}

	IndexStorage::LookupSessionBase* MySqlIndexStorage::StartLookup() const {
		return new MySqlIndexStorage::LookupSession(featureDeserializer, packetSize);
	}

	MySqlIndexStorage::LookupSession::LookupSession(IFeatureDeserializer* featureDeserializer, unsigned int packetSize) : LookupSessionBase(featureDeserializer) {
		Utils::Contract::AssertNotNull(featureDeserializer);
		Utils::Contract::Assert(packetSize > 0);

		this->featureDeserializer = featureDeserializer;
		this->packetSize = packetSize;
	}
}
}