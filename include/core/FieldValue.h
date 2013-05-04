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
        static FieldValue Create(void* dataPtr, size_t dataLength);
        static FieldValue CreateNull();

        //default ctor for using this class' instances in stl containers
        CTOR FieldValue();

        //must be used only for simple types like int, double, float, etc. CANNOT be used for array or strings
        template <typename T>
        T As() const;
        template <>
        std::string As<std::string>() const;
        std::wstring FromUtf8() const;

        blob_p_t CopyToByteArray() const;

        bool IsNull() const;
    private:
        CTOR FieldValue(void* dataPtr, size_t dataLength);

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

    template <>
    std::string FieldValue::As<std::string>() const {
        std::string result;
        
        if (this->dataLength > 0) {
            //HACK: we assume that string received from db is null terminated

            size_t realLength = strnlen_s((char*)dataPtr, dataLength);
            result.assign((char*)dataPtr, realLength);
        }

        return result;
    }
    
}
}
}