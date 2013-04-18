#pragma once

#include "common/CommonInternal.h"
#include "core/BindInfo.h"
#include "core/DbResultReader.h"
#include "core/NonQueryExecResult.h"
#include <string>
#include <vector>

namespace ImgDetective {
namespace Core {
namespace Db {

    ABSTRACT class DbWrapper : NONCOPYABLE, HAS_VIRTUAL_DESCTUCTOR {
    public:
        NonQueryExecResult ExecuteNonQuery(const std::string& sqlStatement) const;
        virtual NonQueryExecResult ExecuteNonQuery(const std::string& sqlStatement, const params_list_t& params) const = 0;
        DbResultReader* ExecuteReader(const std::string& sqlStatement) const;
        virtual DbResultReader* ExecuteReader(const std::string& sqlStatement, const params_list_t& params) const = 0;
    };

}
}
}