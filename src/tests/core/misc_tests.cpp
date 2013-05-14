#include <boost/test/unit_test.hpp>
#include "core/SearchResultInternal.h"
#include "utils/MemoryUtils.h"
#include "common/Matrix.h"

using namespace ImgDetective;
using namespace ImgDetective::Core;

BOOST_AUTO_TEST_SUITE(searchresultinternal_tests)

BOOST_AUTO_TEST_CASE(convert) {
    const size_t numberOfNodes = 20;

    SearchResultInternal resultInternal(numberOfNodes);

    for (size_t i = 0; i < numberOfNodes; ++i) {
        SearchResultInternal::Node node;
        node.imgId = (i + 1);
        node.relevance = 1.0 / (i + 1);
        resultInternal.AddNode(node);
    }

    SearchResult result = resultInternal.ConvertToSearchResult(
        [] (size_t bytesCount) {
            return (void*)(new char[bytesCount]);
        },
        [] (void* ptr) {
            delete [] ptr;
        });

    BOOST_ASSERT(result.arraySize == numberOfNodes);
    BOOST_ASSERT(result.items != NULL);
    BOOST_ASSERT(result.itemsRelevance != NULL);

    bool ok = true;

    for (size_t i = 0; i < numberOfNodes; ++i) {
        if (result.items[i] != (i + 1) || result.itemsRelevance[i] != 1.0 / (i + 1)) {
            ok = false;
            break;
        }
    }

    BOOST_CHECK(ok);

    Utils::Memory::SafeDeleteArray(result.items);
    Utils::Memory::SafeDeleteArray(result.itemsRelevance);
}

BOOST_AUTO_TEST_CASE(matrix_test) {
    const size_t rows = 10;
    const size_t columns = 20;

    Matrix<double> m(rows, columns);

    for (unsigned int i = 0; i < rows; ++i) {
        for (unsigned int j = 0; j < columns; ++j) {
            m(i, j) = i * j;
        }
    }

    bool ok = true;
    for (unsigned int i = 0; i < rows; ++i) {
        for (unsigned int j = 0; j < columns; ++j) {
            if (m(i, j) != i * j) {
                ok = false;
                goto CYCLE_END;
            }
        }
    }
CYCLE_END:
    BOOST_CHECK(ok);

    BOOST_CHECK_THROW(
        m(10, 15),
        std::exception);

    BOOST_CHECK_THROW(
        m(5, 20),
        std::exception);
}

BOOST_AUTO_TEST_SUITE_END()