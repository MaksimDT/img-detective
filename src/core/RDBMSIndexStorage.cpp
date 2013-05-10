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
        Impl(const REF IFeatureDeserializer& featureDeserializer, const Db::DbWrapper& dbWrapper, unsigned int packetSize, const std::string& tableName);
        ~Impl();
        IndexNode::col_t GetNextPacket();
    private:
        IndexNode GetNextIndexNodeFromReader();

        unsigned int packetSize;
        const Db::DbWrapper& dbWrapper;
        Db::DbResultReader* resultReader;
        const IFeatureDeserializer& featureDeserializer;
    };

    RDBMSIndexStorage::LookupSession::Impl::Impl(const REF IFeatureDeserializer& featureDeserializer, const Db::DbWrapper& dbWrapper, unsigned int packetSize, const std::string& tableName) 
        : featureDeserializer(featureDeserializer), dbWrapper(dbWrapper), resultReader(NULL) {
        Utils::Contract::Assert(packetSize > 0);

        this->packetSize = packetSize;

        string query = "SELECT ImageId, Data FROM ";
        query.append(tableName);

        resultReader = dbWrapper.ExecuteReader(query);
    }

    RDBMSIndexStorage::LookupSession::Impl::~Impl() {
        Utils::Memory::SafeDelete(resultReader);
    }

    IndexNode::col_t RDBMSIndexStorage::LookupSession::Impl::GetNextPacket() {
        IndexNode::col_t packet;
        packet.reserve(packetSize);

        for (unsigned int i = 0; i < packetSize; ++i) {
            if (this->resultReader->Next()) {
                try {
                    IndexNode nextNode = GetNextIndexNodeFromReader();
                    packet.push_back(nextNode);
                }
                catch (...) {
                    IndexNode::col_t::iterator it;
                    for (it = packet.begin(); it != packet.end(); ++it) {
                        if (it->feature != NULL) {
                            delete it->feature;
                        }
                    }
                    throw;
                }
            }
            else {
                break;
            }
        }

        return packet;
    }

    IndexNode RDBMSIndexStorage::LookupSession::Impl::GetNextIndexNodeFromReader() {
        blob_p_t data = NULL;
        IFeature* feature = NULL;

        try {
            imgid_t imgId = this->resultReader->GetField("ImageId").As<imgid_t>();
            //TODO: optimize memory allocation
            data = this->resultReader->GetField("Data").CopyToByteArray();
            feature = this->featureDeserializer.DeserializeFeature(*data);

            Core::SafeFreeBlob(data);
            return IndexNode(imgId, feature);
        }
        catch (...) {
            Core::SafeFreeBlob(data);
            Utils::Memory::SafeDelete(feature);
            throw;
        }
    }

    #pragma endregion

    #pragma region RDBMSIndexStorage::LookupSession

    RDBMSIndexStorage::LookupSession::LookupSession(const REF IFeatureDeserializer& featureDeserializer, const Db::DbWrapper& dbWrapper, unsigned int packetSize, const std::string& tableName) 
        : LookupSessionBase(featureDeserializer) {

        pimpl = new Impl(featureDeserializer, dbWrapper, packetSize, tableName);
    }

    RDBMSIndexStorage::LookupSession::~LookupSession() {
        Utils::Memory::SafeDelete(pimpl);
    }

    IndexNode::col_t RDBMSIndexStorage::LookupSession::GetNextPacket() {
        return pimpl->GetNextPacket();
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
        addFeatureQuery.append(" (ImageId, data) VALUES (?, ?)");
    }

    IIndexStorage::ILookupSession* RDBMSIndexStorage::Impl::StartLookup() const {
        return new RDBMSIndexStorage::LookupSession(featureDeserializer, dbWrapper, packetSize, tableName);
    }

    void RDBMSIndexStorage::Impl::AddFeature(const IFeature& feature, imgid_t imgId) {
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