#pragma once

#include <vector>
#include "core/DbType.h"
#include "common/CommonInternal.h"

namespace ImgDetective {
namespace Core {
namespace Db {

    //buffer used by a RDBMS engine either for input or output
    class DbParamBuffer {
    public:
        CTOR DbParamBuffer(void* data, size_t length, DbType::Enum dbType);

        //returns the pointer to the memory location where the buffer starts
        void* GetDataPtr() const;
        //return the length of the buffer in bytes
        size_t GetDataLength() const;
        
        //returns the universal type of data that is (or will be) stored in this buffer
        DbType::Enum GetType() const;

        bool IsNull() const;
    private:
        void* data;
        size_t dataLength;
        DbType::Enum dbType;
    };

    typedef ::std::vector<DbParamBuffer> params_list_t;
}
}
}