#pragma once

#include "core/ImgMetadataStorage.h"

namespace ImgDetective {
namespace Core {

    CONCRETE SEALED class RDBMSImgMetadataStorage : public ImgMetadataStorage {
    public:
        virtual imgid_t InitImgRecord() const;
        virtual void SaveImgPath(imgid_t imgId, const std::string& path) const;
    };

}
}