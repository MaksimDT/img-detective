#include "core/FieldValue.h"
#include "utils/ContractUtils.h"
#include "utils/UnicodeUtils.h"

using namespace std;

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

        blob_p_t result = NULL;

        try {
            result = Core::CreateBlobOfSize(this->dataLength);
            CopyToBlob(dataPtr, result);
        }
        catch (...) {
            Core::SafeFreeBlob(result);
            throw;
        }
        
        return result;
    }

    wstring FieldValue::FromUtf8() const {
        if (this->IsNull()) {
            return wstring();
        }

        string utf8val = this->As<string>();

        return Utils::Unicode::Utf8ToUtf16(utf8val);
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