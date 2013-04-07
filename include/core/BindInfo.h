#pragma once

#include <vector>
#include "core/DbType.h"
#include "common/CommonInternal.h"

namespace ImgDetective {
namespace Core {
namespace Db {

    //below are data structs that universally (in a RDBMS agnostic way) describe 
    //parameters and output buffers used by RDBMS API

    //buffer used by a RDBMS engine either for input or output
    class DbBuffer : NONCOPYABLE {
    public:
        virtual ~DbBuffer();

        //returns the pointer to the memory location where the buffer starts
        void* GetDataPtr() const;
        //return the length of the buffer in bytes
        size_t GetDataLength() const;
        
        //returns the universal type of data that is (or will be) stored in this buffer
        DbType::Enum GetType() const;
        //sets the universal type of data that is (or will be) stored in this buffer
        void SetType(DbType::Enum type);
    protected:
        CTOR DbBuffer(bool needToFreeMemory);

        void* data;
        size_t dataLength;
    private:
        DbType::Enum typeId;
        bool needToFreeMemory;
    };

    //buffer used by a RDBMS engine for input parameters
    class DbParamBuffer : public DbBuffer {
    public:
        //Initializes a new parameter object using the provided raw buffer.
        DbParamBuffer(void* data, size_t length, bool needToFreeMemory);
    };

    //buffer used by a RDBMS engine for output values (e.g. when returning a result set)
    class DbFieldBuffer : public DbBuffer {
    public:
        CTOR DbFieldBuffer();

        //Allocates the buffer of specified length. Doesn't initialize it
        void Allocate(size_t numOfBytes);

        std::string GetFieldName() const;
        void SetFieldName(const std::string& fieldName);

        bool IsOfVariableLength() const;
        void SetIsOfVariableLength(bool isOfVariableLength);
    private:
        std::string fieldName;
        unsigned long realLength;
        bool isOfVariableLength;
    };

    typedef ::std::vector<DbParamBuffer> params_list_t;
    typedef ::std::vector<DbFieldBuffer> fields_vector_t;
}
}
}