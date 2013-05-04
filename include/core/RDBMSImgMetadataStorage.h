#pragma once

#include "core/DbWrapper.h"
#include "core/ImgMetadataStorage.h"

namespace ImgDetective {
namespace Core {

    CONCRETE SEALED class RDBMSImgMetadataStorage : public ImgMetadataStorage {
    public:
        CTOR RDBMSImgMetadataStorage(REF Db::DbWrapper& dbWrapper);

        virtual void InitImgRecord(REF ImgInfo& imgInfo) const;

        virtual FsRepositoryInfo CreateFsRepositoryRecord(const boost::filesystem::path& dirPath) const;
        virtual FsRepositoryInfo::col_t GetAllRepositories() const;
    private:
        REF Db::DbWrapper& dbWrapper;
    };

}
}