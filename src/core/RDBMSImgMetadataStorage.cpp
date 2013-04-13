#include "core/RDBMSImgMetadataStorage.h"

namespace ImgDetective {
namespace Core {

    CTOR RDBMSImgMetadataStorage::RDBMSImgMetadataStorage(REF Db::DbWrapper& dbWrapper) 
        : dbWrapper(dbWrapper) {
    }

    void RDBMSImgMetadataStorage::InitImgRecord(REF ImgInfo& imgInfo) const {
        /*Db::params_list_t params;
        Db::DbParamBuffer imgId;*/

        
    }
}
}