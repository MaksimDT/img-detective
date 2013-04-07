#include "core/RDBMSImgMetadataStorage.h"

namespace ImgDetective {
namespace Core {

    CTOR RDBMSImgMetadataStorage::RDBMSImgMetadataStorage(REF DbWrapper& dbWrapper) 
        : dbWrapper(dbWrapper) {
    }

    void RDBMSImgMetadataStorage::InitImgRecord(REF ImgInfo& imgInfo) const {
        DbWrapper::params_list_t params;
        DbWrapper::ParamInfo imgId;

        
    }
}
}