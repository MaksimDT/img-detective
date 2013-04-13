#include "core/FieldValue.h"
#include "utils/ContractUtils.h"

namespace ImgDetective {
namespace Core {
namespace Db {

    FieldValue FieldValue::Create(void* dataPtr, size_t dataLength) {
        Utils::Contract::AssertNotNull(dataPtr);
        Utils::Contract::Assert(dataLength != 0);

        return FieldValue(dataPtr, dataLength);
    }

    FieldValue FieldValue::CreateNull() {
        return FieldValue();
    }

    FieldValue::FieldValue() {
        dataPtr = NULL;
        dataLength = 0;
    }

    blob_p_t FieldValue::CopyToByteArray() const {
        if (this->IsNull()) {
            return NULL;
        }
        else {
            blob_p_t result = Core::CreateBlobOfSize(this->dataLength);
            CopyToBlob(dataPtr, result);
            
            return result;
        }
    }

    bool FieldValue::IsNull() const {
        return dataPtr == NULL;
    }

    CTOR FieldValue::FieldValue(void* dataPtr, size_t dataLength) {
        this->dataPtr = dataPtr;
        this->dataLength = dataLength;
    }
}
}
}