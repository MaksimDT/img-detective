#include "core/RDBMSImgMetadataStorage.h"
#include "utils/MemoryUtils.h"

using namespace ImgDetective::Core::Db;

namespace ImgDetective {
namespace Core {

    CTOR RDBMSImgMetadataStorage::RDBMSImgMetadataStorage(REF Db::DbWrapper& dbWrapper) 
        : dbWrapper(dbWrapper) {
    }

    void RDBMSImgMetadataStorage::InitImgRecord(REF ImgInfo& imgInfo) const {
        NonQueryExecResult dbResult = dbWrapper.ExecuteNonQuery(
            "INSERT INTO Images \
             (Path, CreationDate, Description) \
             VALUES \
             (NULL, NOW(), NULL)");
        
        //NOTE: convertion!!
        imgid_t generatedImgId = dbResult.GetLastInsertId();
        imgInfo.SetId(generatedImgId);
    }

    void RDBMSImgMetadataStorage::SaveImgRecord(const REF ImgInfo& imgInfo) const {
        char* pathBuffer = NULL;
        
        try {
            params_list_t params;

            std::string path = imgInfo.GetPath();
            pathBuffer = new char[path.length()];
            memcpy(pathBuffer, path.data(), path.length());
            DbParamBuffer pathParam(pathBuffer, path.length(), DbType::VARCHAR);
            params.push_back(pathParam);

            imgid_t imgId = imgInfo.GetId();
            params.push_back(DbParamBuffer(&imgId, sizeof(imgId), DbType::LONGLONG));

            NonQueryExecResult dbResult = dbWrapper.ExecuteNonQuery(
                "UPDATE Images \
                 SET Path = ? \
                 WHERE Id = ?",
                 params);

            Utils::Memory::SafeDeleteArray(pathBuffer);
        }
        catch (...) {
            Utils::Memory::SafeDeleteArray(pathBuffer);
            throw;
        }
    }
}
}