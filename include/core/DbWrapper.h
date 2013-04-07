#pragma once

#include "common/CommonInternal.h"
#include "core/BindInfo.h"
#include "core/DbResultReader.h"
#include <string>
#include <vector>

namespace ImgDetective {
namespace Core {
namespace Db {

    class DbWrapper : NONCOPYABLE, HAS_VIRTUAL_DESCTUCTOR {
    public:
        virtual void ExecuteNonQuery(const ::std::string& sqlStatement, const params_list_t& params) const = 0;
        virtual DbResultReader* ExecuteReader(const ::std::string& sqlStatement, const params_list_t& params) const = 0;
    };

}
}
}