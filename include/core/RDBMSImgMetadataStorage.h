#pragma once

#include "core/DbWrapper.h"
#include "core/ImgMetadataStorage.h"

namespace ImgDetective {
namespace Core {

    CONCRETE SEALED class RDBMSImgMetadataStorage : public ImgMetadataStorage {
    public:
        CTOR RDBMSImgMetadataStorage(REF DbWrapper& dbWrapper);

        virtual void InitImgRecord(REF ImgInfo& imgInfo) const;
        virtual void SaveImgRecord(const REF ImgInfo& imgInfo) const;
    private:
        REF DbWrapper& dbWrapper;
    };

}
}