#pragma once

#include "common/CommonInternal.h"
#include <vector>

namespace ImgDetective {
namespace Core {

    class DbWrapper : NONCOPYABLE, HAS_VIRTUAL_DESCTUCTOR {
    public:
        struct DbType {
            NESTED enum Enum {
                UNKNOWN = 0,
                LONGLONG,
                BLOB
            };
        };

        NESTED struct ParamInfo {
        public:
            void* data;
            size_t dataLength;
            int typeId;
            bool isSigned;
        };

        typedef std::vector<ParamInfo> params_list_t;

        virtual void ExecuteNonQuery(const REF std::string& sqlStatement, const REF params_list_t& params) const = 0;
    };

}
}