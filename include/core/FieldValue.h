#pragma once

#include "common/CommonInternal.h"
#include "core/DbType.h"
#include <string>
#include <map>

namespace ImgDetective {
namespace Core {
namespace Db {

    class FieldValue {
    public:
        FieldValue(void* dataPtr, size_t dataLength);

        template <typename T>
        T As() const;

        blob_p_t CopyToByteArray() const;
    private:
        void* dataPtr;
        size_t dataLength;
    };

    template <typename T>
    T FieldValue::As() const {
        if (sizeof(T) == dataLength) {
            T* asPtr = (T*)dataPtr;
            return *asPtr;
        }
        else {
            throw std::exception("cannot convert value of the db field to the specified type (sizes mismatch)");
        }
    }
}
}
}