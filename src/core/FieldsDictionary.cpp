#include "core/FieldsDictionary.h"

using namespace std;

namespace ImgDetective {
namespace Core {
namespace Db {

    void FieldsDictionary::Add(const std::string& fieldName, const FieldValue& fieldValue) {
        internalDict[fieldName] = fieldValue;
    }

    const FieldValue& FieldsDictionary::Get(const std::string& fieldName) const {
        map<string, FieldValue>::const_iterator it = internalDict.find(fieldName);

        if (it == internalDict.cend()) {
            string msg = "Cannot find field ";
            msg.append(fieldName);
            throw exception(msg.c_str());
        }

        return it->second;
    }

    bool FieldsDictionary::HasField(const std::string fieldName) const {
        map<string, FieldValue>::const_iterator found = internalDict.find(fieldName);

        return found != internalDict.cend();
    }

    const FieldValue& FieldsDictionary::operator [] (const std::string& fieldName) const {
        return Get(fieldName);
    }

    void FieldsDictionary::Clear() {
        internalDict.clear();
    }
}
}
}