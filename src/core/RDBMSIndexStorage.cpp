#pragma once

#include "core/RDBMSIndexStorage.h"
#include "utils/ContractUtils.h"
#include "utils/MemoryUtils.h"

using namespace std;

namespace ImgDetective {
namespace Core {

    #pragma region RDBMSIndexStorage::LookupSession::Impl

    class RDBMSIndexStorage::LookupSession::Impl {
    public:
        Impl(const REF IFeatureDeserializer& featureDeserializer, unsigned int packetSize);
        ~Impl();
        bool GetNextPacket(REF IndexNode::col_p_t& packet);
    private:
        unsigned int packetSize;
        unsigned int curPacketNumber;
    };

    RDBMSIndexStorage::LookupSession::Impl::Impl(const REF IFeatureDeserializer& featureDeserializer, unsigned int packetSize) {
        Utils::Contract::Assert(packetSize > 0);

        this->packetSize = packetSize;
        this->curPacketNumber = 1;
    }

    RDBMSIndexStorage::LookupSession::Impl::~Impl() {

    }

    bool RDBMSIndexStorage::LookupSession::Impl::GetNextPacket(REF IndexNode::col_p_t& packet) {
        //todo: implement
        return true;
    }

    #pragma endregion

    #pragma region RDBMSIndexStorage::LookupSession

    RDBMSIndexStorage::LookupSession::LookupSession(const REF IFeatureDeserializer& featureDeserializer, unsigned int packetSize) 
        : LookupSessionBase(featureDeserializer) {

        pimpl = new Impl(featureDeserializer, packetSize);
    }

    RDBMSIndexStorage::LookupSession::~LookupSession() {
        Utils::Memory::SafeDelete(pimpl);
    }

    bool RDBMSIndexStorage::LookupSession::GetNextPacket(REF IndexNode::col_p_t& packet) {
        return pimpl->GetNextPacket(REF packet);
    }

    #pragma endregion

    #pragma region RDBMSIndexStorage::Impl

    class RDBMSIndexStorage::Impl {
    public:
        Impl(const REF IFeatureDeserializer& featureDeserializer, const REF Db::DbWrapper& dbWrapper, unsigned int packetSize, const std::string& tableName);
        IIndexStorage::ILookupSession* StartLookup() const;
        void AddFeature(const REF IFeature& feature, imgid_t imgId);
    private:
        MYSQL* ConnectToDb();
        void CloseConnectionToDb(MYSQL* connection);

        const REF IFeatureDeserializer& featureDeserializer;
        const REF Db::DbWrapper& dbWrapper;
        unsigned int packetSize;
        std::string tableName;
        std::string addFeatureQuery;
    };

    RDBMSIndexStorage::Impl::Impl(const REF IFeatureDeserializer& featureDeserializer, const REF Db::DbWrapper& dbWrapper, unsigned int packetSize, const std::string& tableName)
        : featureDeserializer(featureDeserializer), dbWrapper(dbWrapper) {
        Utils::Contract::Assert(packetSize > 0);
        Utils::Contract::Assert(!tableName.empty());

        this->packetSize = packetSize;
        this->tableName = tableName;

        addFeatureQuery = "INSERT INTO ";
        addFeatureQuery.append(tableName);
        addFeatureQuery.append("(ImageId, data) VALUES (?, ?)");
    }

    IIndexStorage::ILookupSession* RDBMSIndexStorage::Impl::StartLookup() const {
        //TODO: implementation
        return new RDBMSIndexStorage::LookupSession(REF featureDeserializer, packetSize);
    }

    void RDBMSIndexStorage::Impl::AddFeature(const REF IFeature& feature, imgid_t imgId) {
        blob_p_t serializedFeature = NULL;

        try {
            serializedFeature = feature.Serialize();

            Db::params_list_t params;
        
            Db::DbParamBuffer imageIdParam(&imgId, sizeof(imgId), Db::DbType::LONGLONG);
            params.push_back(imageIdParam);
        
            Db::DbParamBuffer dataParam(serializedFeature->data(), serializedFeature->size(), Db::DbType::BLOB);
            params.push_back(dataParam);

            dbWrapper.ExecuteNonQuery(this->addFeatureQuery, params);

            Core::SafeFreeBlob(serializedFeature);
        }
        catch (...) {
            Core::SafeFreeBlob(serializedFeature);
            throw;
        }
    }

    #pragma endregion

    #pragma region RDBMSIndexStorage

    RDBMSIndexStorage::RDBMSIndexStorage(IFeatureDeserializer* featureDeserializer, const REF Db::DbWrapper& dbWrapper, unsigned int packetSize, const std::string& tableName) 
        : IndexStorage(featureDeserializer) {

        pimpl = new Impl(*featureDeserializer, dbWrapper, packetSize, tableName);
	}

    RDBMSIndexStorage::~RDBMSIndexStorage() {
        Utils::Memory::SafeDelete(pimpl);
    }

    IIndexStorage::ILookupSession* RDBMSIndexStorage::StartLookup() const {
        return pimpl->StartLookup();
	}

    void RDBMSIndexStorage::AddFeature(const REF IFeature& feature, imgid_t imgId) const {
        pimpl->AddFeature(feature, imgId);
    }

    #pragma endregion
}
}