#pragma once

#include "core/FieldValue.h"
#include <string>
#include <map>

namespace ImgDetective {
namespace Core {
namespace Db {

    class FieldsDictionary {
    public:
        void Add(const std::string& fieldName, const FieldValue& fieldValue);
        const FieldValue& Get(const std::string& fieldName) const;
        bool HasField(const std::string fieldName) const;
        const FieldValue& operator [] (const std::string& fieldName) const;
        void Clear();
    private:
        std::map<std::string, FieldValue> internalDict;
    };

}
}
}