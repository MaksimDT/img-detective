#pragma once

#include "common/CommonInternal.h"
#include "core/BindInfo.h"
#include "core/FieldValue.h"

namespace ImgDetective {
namespace Core {
namespace Db {

    class DbResultReader : NONCOPYABLE, HAS_VIRTUAL_DESCTUCTOR {
    public:
        //goes to the next row in the result set. Returns true if the result set contains more rows
        virtual bool Next() = 0;
        //checks if the result set contains field with specified name
        virtual bool HasField(const std::string& fieldName) const = 0;
        //returns value of the field with the specified name
        virtual const FieldValue& operator[] (const std::string& fieldName) const = 0;
    };

}
}
}