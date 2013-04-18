#include "core/DbWrapper.h"

namespace ImgDetective {
namespace Core {
namespace Db {

    NonQueryExecResult DbWrapper::ExecuteNonQuery(const std::string& sqlStatement) const {
        return ExecuteNonQuery(sqlStatement, params_list_t());
    }

    DbResultReader* DbWrapper::ExecuteReader(const std::string& sqlStatement) const {
        return ExecuteReader(sqlStatement, params_list_t());
    }
}
}
}