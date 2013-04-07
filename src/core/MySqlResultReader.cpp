#include "core/MySqlResultReader.h"
#include "utils/ContractUtils.h"
#include "utils/MemoryUtils.h"

namespace ImgDetective {
namespace Core {
namespace Db {

    CTOR MySqlResultReader::MySqlResultReader(fields_vector_t* fieldBuffers, MYSQL* connection, MYSQL_BIND* rawFieldBuffers, MYSQL_STMT* stmt) 
        : fieldBuffers(fieldBuffers), connection(connection), rawFieldBuffers(rawFieldBuffers), stmt(stmt) {

        Utils::Contract::AssertNotNull(fieldBuffers);
        Utils::Contract::AssertNotNull(connection);
        Utils::Contract::AssertNotNull(rawFieldBuffers);
        Utils::Contract::AssertNotNull(stmt);

        finished = false;

        for (int i = 0; i < fieldBuffers->size(); ++i) {
            DbFieldBuffer& fb = (*fieldBuffers)[i];

            if (fb.IsOfVariableLength()) {
                variableLengthFieldsIndexes.push_back(i);
            }
        }
    }

    MySqlResultReader::~MySqlResultReader() {

    }

    FieldsDictionary* MySqlResultReader::Next() {
        if (!finished) {
            int status = mysql_stmt_fetch(stmt);

            if (status == 1 || status == MYSQL_NO_DATA) {
                finished = true;
                return NULL;
            }

            PrepareVariableLengthBuffers();
            FetchVariableLengthFields();

            FieldsDictionary* result = new FieldsDictionary();
            for (int i = 0; i < fieldBuffers->size(); ++i) {
                FieldValue fv(rawFieldBuffers[i].buffer, rawFieldBuffers[i].length_value);
                std::string fieldName = (*fieldBuffers)[i].GetFieldName();
                result->Add(fieldName, fv);
            }

            return result;
        }
        else {
            return NULL;
        }
    }
}
}
}