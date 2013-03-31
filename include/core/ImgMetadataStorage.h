#pragma once

#include "common/CommonInternal.h"
#include <string>

namespace ImgDetective {
namespace Core {

    INTERFACE class ImgMetadataStorage : NONCOPYABLE, HAS_VIRTUAL_DESCTUCTOR {
    public:
        virtual void InitImgRecord(REF ImgInfo& imgInfo) const = 0;
        virtual void SaveImgRecord(const REF ImgInfo& imgInfo) const = 0;
    };

}
}