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
        node.position = (i + 1);
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
    BOOST_ASSERT(result.itemsPositions != NULL);

    bool ok = true;

    for (size_t i = 0; i < numberOfNodes; ++i) {
        if (result.items[i] != (i + 1) || result.itemsPositions[i] != (i + 1)) {
            ok = false;
            break;
        }
    }

    BOOST_CHECK(ok);

    Utils::Memory::SafeDeleteArray(result.items);
    Utils::Memory::SafeDeleteArray(result.itemsPositions);
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

BOOST_AUTO_TEST_CASE(matrix_zigzag_scanning_test) {
    const size_t N = 8;

    Matrix<int> A(N, N);

    for (unsigned int i = 0; i < N; ++i) {
        for (unsigned int j = 0; j < N; ++j) {
            A(i, j) = i * N + j + 1;
        }
    }

    int expectedResult[] = 
    { 
        1, 
        2, 9, 
        17, 10, 3, 
        4, 11, 18, 25, 
        33, 26, 19, 12, 5, 
        6, 13, 20, 27, 34, 41, 
        49, 42, 35, 28, 21, 14, 7,
        8, 15, 22, 29, 36, 43, 50, 57,
        58, 51, 44, 37, 30, 23, 16,
        24, 31, 38, 45, 52, 59,
        60, 53, 46, 39, 32,
        40, 47, 54, 61,
        62, 55, 48,
        56, 63,
        64
    };

    std::vector<int> scanned;
    scanned.resize(N * N);

    A.ZigzagScan(REF scanned);

    bool ok = true;
    for (unsigned int i = 0; i < N * N; ++i) {
        if (scanned[i] != expectedResult[i]) {
            ok = false;
            break;
        }
    }

    BOOST_CHECK(ok);
}

BOOST_AUTO_TEST_SUITE_END()