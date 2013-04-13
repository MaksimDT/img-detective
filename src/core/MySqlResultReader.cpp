#include "core/MySqlResultReader.h"
#include "utils/ContractUtils.h"
#include "utils/MemoryUtils.h"
#include "core/MySqlTypesHelper.h"

namespace ImgDetective {
namespace Core {
namespace Db {

    #pragma region MySqlResultReader::VariableLengthBuffer impl

    CTOR MySqlResultReader::VariableLengthBuffer::VariableLengthBuffer()
        : buffer(NULL), usedSize(0), maxSize(0) {}

    MySqlResultReader::VariableLengthBuffer::~VariableLengthBuffer() {
        DeallocateCurrent();
    }

    void* MySqlResultReader::VariableLengthBuffer::Acquire(size_t requiredSize) {
        Utils::Contract::Assert(requiredSize > 0);

        if (requiredSize > maxSize) {
            DeallocateCurrent();
            buffer = new char[requiredSize];
            maxSize = requiredSize;
            usedSize = requiredSize;
        }

        return buffer;
    }

    void MySqlResultReader::VariableLengthBuffer::DeallocateCurrent() {
        Utils::Memory::SafeDeleteArray(buffer);
    }

    #pragma endregion

    MySqlResultReader* MySqlResultReader::Create(MYSQL* connection, MYSQL_STMT* stmt, MYSQL_RES* resultMetadata) {
        MySqlResultReader* resultReader = new MySqlResultReader(connection, stmt, resultMetadata);
        try {
            resultReader->SetupFieldBuffers();
            return resultReader;
        }
        catch (...) {
            delete resultReader;
            throw;
        }
    }

    CTOR MySqlResultReader::MySqlResultReader(MYSQL* connection, MYSQL_STMT* stmt, MYSQL_RES* resultMetadata) 
        : connection(connection), stmt(stmt), resultMetadata(resultMetadata), fieldsMetadata(NULL), fieldBuffers(NULL) {

        Utils::Contract::AssertNotNull(connection);
        Utils::Contract::AssertNotNull(stmt);
        Utils::Contract::AssertNotNull(resultMetadata);

        finished = false;
    }

    MySqlResultReader::~MySqlResultReader() {
        FreeFieldBuffers();
        if (resultMetadata != NULL) {
            mysql_free_result(resultMetadata);
        }
        if (stmt != NULL) {
            mysql_stmt_close(stmt);
        }
        if (connection != NULL) {
            mysql_close(connection);
        }
    }

    void MySqlResultReader::SetupFieldBuffers() {
        //if exception is raised in this routine, all allocated memory will be freed in the class' destructor

        //retrieve fields metadata
        fieldsMetadata = mysql_fetch_fields(resultMetadata);

        //setup buffers
        fieldBuffers = new MYSQL_BIND[resultMetadata->field_count];
        memset(fieldBuffers, 0, sizeof(MYSQL_BIND) * resultMetadata->field_count);

        //for each field in the result metadata we create MYSQL_BIND struct with memory buffer in it
        for (int i = 0; i < resultMetadata->field_count; ++i) {
            MYSQL_FIELD f = fieldsMetadata[i];
            unsigned int typeLength = MySqlTypesHelper::GetMySqlTypeLength(f.type);

            if (typeLength != 0) {
                fieldBuffers[i].buffer = new char[typeLength];
                fieldBuffers[i].buffer_length = typeLength;
            }
            else {
                //here the variable length buffer is needed, let's handle this in the Next() routine
                fieldBuffers[i].buffer = NULL;
                fieldBuffers[i].buffer_length = 0;
            }

            fieldBuffers[0].buffer_type = f.type;
        }

        varLengthBufs.resize(resultMetadata->field_count);
    }

    FieldsDictionary* MySqlResultReader::Next() {
        if (finished) {
            return NULL;
        }

        int status = mysql_stmt_fetch(stmt);

        if (status == 1 || status == MYSQL_NO_DATA) {
            finished = true;
            return NULL;
        }

        unsigned int fieldCount = resultMetadata->field_count;

        FieldsDictionary* result = new FieldsDictionary();
        for (int i = 0; i < fieldCount; ++i) {
            MYSQL_BIND& f = fieldBuffers[i];
            std::string fieldName = fieldsMetadata[i].name;

            bool isVariableLength = (f.buffer_length == 0 && f.length_value > 0);
            bool isConstantLength = (f.buffer != NULL && f.buffer_length > 0);

            if (f.is_null_value) {
                result->Add(fieldName, FieldValue::CreateNull());
            }
            else if (isVariableLength) {
                //if the field being fetched has variable length data let's use the variable length buffer
                f.buffer = varLengthBufs[i].Acquire(f.length_value);
                f.buffer_length = f.length_value;

                if (mysql_stmt_fetch_column(stmt, fieldBuffers, i, 0)) {
                    throw std::exception("mysql_stmt_fetch_column failed");
                }

                if (f.is_null_value) {
                    result->Add(fieldName, FieldValue::CreateNull());
                }
                else {
                    result->Add(fieldName, FieldValue::Create(f.buffer, f.buffer_length));
                }
            }
            else if (isConstantLength) {
                result->Add(fieldName, FieldValue::Create(f.buffer, f.buffer_length));
            }
            else {
                std::string msg = "cannot read value of the column ";
                msg.append(fieldName);
                throw std::exception(msg.c_str());
            }
        }

        return result;
    }
}
}
}