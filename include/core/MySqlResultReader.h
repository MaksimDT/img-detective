#pragma once

#include "common/CommonInternal.h"
#include "core/mysqlheaders.h"
#include "core/DbResultReader.h"

namespace ImgDetective {
namespace Core {
namespace Db {

    CONCRETE SEALED class MySqlResultReader : public DbResultReader {
    public:
        CTOR MySqlResultReader(fields_vector_t* fieldBuffers, MYSQL* connection, MYSQL_BIND* rawFieldBuffers, MYSQL_STMT* stmt);
        ~MySqlResultReader();

        virtual FieldsDictionary* Next();
    private:
        void PrepareVariableLengthBuffers();
        void FetchVariableLengthFields();

        fields_vector_t* fieldBuffers;
        MYSQL* connection;
        MYSQL_BIND* rawFieldBuffers;
        MYSQL_STMT* stmt;
        bool finished;
        std::vector<int> variableLengthFieldsIndexes;
    };

}
}
}