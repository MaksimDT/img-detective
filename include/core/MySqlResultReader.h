#pragma once

#include "common/CommonInternal.h"
#include "core/mysqlheaders.h"
#include "core/DbResultReader.h"

namespace ImgDetective {
namespace Core {
namespace Db {

    CONCRETE SEALED class MySqlResultReader : public DbResultReader {
    public:
        static MySqlResultReader* Create(MYSQL* connection, MYSQL_STMT* stmt, MYSQL_RES* resultMetadata);
        ~MySqlResultReader();

        virtual FieldsDictionary* Next();
    private:
        NESTED class VariableLengthBuffer {
        public:
            CTOR VariableLengthBuffer();
            ~VariableLengthBuffer();
            void* Acquire(size_t requiredSize);
        private:
            void DeallocateCurrent();

            void* buffer;
            size_t usedSize;
            size_t maxSize;
        };

        CTOR MySqlResultReader(MYSQL* connection, MYSQL_STMT* stmt, MYSQL_RES* resultMetadata);

        void SetupFieldBuffers();
        void FreeFieldBuffers();
        void AdjustVariableLengthBuffers();

        MYSQL* connection;
        MYSQL_STMT* stmt;
        MYSQL_RES* resultMetadata;
        MYSQL_FIELD* fieldsMetadata;
        MYSQL_BIND* fieldBuffers;
        
        bool finished;
        std::vector<VariableLengthBuffer> varLengthBufs;
    };

}
}
}