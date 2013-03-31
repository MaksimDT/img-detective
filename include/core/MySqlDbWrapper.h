#pragma once

#include "core/DbWrapper.h"
#include "core/mysqlheaders.h"

namespace ImgDetective {
namespace Core {

    CONCRETE class MySqlDbWrapper : public DbWrapper {
    public:
        NESTED struct ConnectionSettings {
        public:
            std::string host;
            int port;
            std::string login;
            std::string password;
            std::string dbName;
        };

        CTOR MySqlDbWrapper(const REF ConnectionSettings& conSettings);

        virtual void ExecuteNonQuery(const REF std::string& sqlStatement, const REF params_list_t& params) const;
    private:
        MYSQL* Connect() const;
        void Disconnect(MYSQL* con) const;
        MYSQL_BIND PrepareBindStruct(const REF ParamInfo& param) const;

        ConnectionSettings conSettings;
    };

}
}