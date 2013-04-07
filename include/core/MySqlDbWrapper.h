#pragma once

#include "core/DbWrapper.h"
#include "core/MySqlConnectionSettings.h"
#include "core/mysqlheaders.h"

namespace ImgDetective {
namespace Core {
namespace Db {

    CONCRETE SEALED class MySqlDbWrapper : public DbWrapper {
    public:
        CTOR MySqlDbWrapper(const MySqlConnectionSettings& conSettings);

        //used for the execution of insert, update, delete statements
        virtual void ExecuteNonQuery(const std::string& sqlStatement, const REF params_list_t& params) const;
        //used for the execution of select statements
        virtual DbResultReader* ExecuteReader(const std::string& sqlStatement, const params_list_t& params) const;
    private:
        //connects to the mysql server
        MYSQL* Connect() const;
        //disconnects from the mysql server
        void Disconnect(MYSQL* con) const;
        //connects to the mysql server then prepares and executes specified statement. Returns the handles to db connection and executed statement
        void ExecuteStatement(const std::string& sqlStatement, const params_list_t& params, OUT MYSQL*& connection, OUT MYSQL_STMT*& stmt) const;

        //universal params format => mysql params format (for collection of params)
        static MYSQL_BIND* PrepareParams(const params_list_t& params);
        //mysql result metadata => (universal fields format + mysql result set format, sharing the same memory buffers)
        static void PrepareFieldBuffers(MYSQL_RES* resultMetadata, OUT fields_vector_t*& fieldBuffers, OUT MYSQL_BIND*& bindArray);
        //universal param format => mysql param format
        static MYSQL_BIND PrepareParamInfo(const REF DbParamBuffer& param);
        //mysql field metadata => universal field format
        static void InitUniFieldBufferFromMySqlFieldMetadata(const MYSQL_FIELD& fieldMetadata, REF DbFieldBuffer& uniBuf);
        //universal field format + mysq field metadata => mysql field format
        static MYSQL_BIND CreateMySqlBindFromUniFieldBuffer(const DbFieldBuffer& uniBuf, const MYSQL_FIELD& fieldMetadata);

        MySqlConnectionSettings conSettings;
    };

}
}
}