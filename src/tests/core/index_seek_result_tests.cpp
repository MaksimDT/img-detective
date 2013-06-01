#define BOOST_TEST_MODULE ImgDetectiveUnitTests
#include <boost/test/unit_test.hpp>
#include "core/IndexSeekResult.h"
#include "core/IndexResultsCombiner.h"
#include "utils/MemoryUtils.h"

#include <vector>

using namespace ImgDetective;
using namespace ImgDetective::Core;
using namespace std;

BOOST_AUTO_TEST_SUITE(index_seek_result_tests)

BOOST_AUTO_TEST_CASE(index_results_merge_tests) {
    IndexSeekResult* ix1 = new IndexSeekResult(1);
    IndexSeekResult* ix2 = new IndexSeekResult(2);
    IndexSeekResult* ix3 = new IndexSeekResult(3);

    ix1->Insert(1, FeatureDistance(0.3));
    ix1->Insert(2, FeatureDistance(0.6));
    ix1->Insert(3, FeatureDistance(0.4));
    ix1->Insert(4, FeatureDistance(0.2));
    ix1->Insert(5, FeatureDistance(0.1));
    ix1->Insert(6, FeatureDistance(0.5));

    ix2->Insert(2, FeatureDistance(0.3));
    ix2->Insert(3, FeatureDistance(0.2));
    ix2->Insert(4, FeatureDistance(0.1));

    ix3->Insert(1, FeatureDistance(0.2));
    ix3->Insert(3, FeatureDistance(0.1));
    ix3->Insert(5, FeatureDistance(0.3));

    vector<IndexSeekResult*> ixs;
    ixs.push_back(ix1);
    ixs.push_back(ix2);
    ixs.push_back(ix3);

    IndexResultsCombiner ixResultsCombiner;
    ImgQuery initialQuery;
    initialQuery.exampleContent = NULL;
    initialQuery.exampleContentSize = 0;
    initialQuery.tolerance = 0.5;
    
    SearchResultInternal* resultInternal = ixResultsCombiner.CombineIndexResults(ixs, initialQuery);
    SearchResult result = resultInternal->ConvertToSearchResult(
        [](size_t bytes) {
            return (void*)(new char[bytes]);
        },
        [](void* memChunk) {
            delete [] memChunk;
        });

    BOOST_ASSERT(result.arraySize == 6);
    BOOST_ASSERT(result.items != NULL);
    BOOST_ASSERT(result.itemsPositions != NULL);

    BOOST_CHECK(result.itemsPositions[0] == 5.0);
    BOOST_CHECK(result.itemsPositions[1] == 9.0);
    BOOST_CHECK(result.itemsPositions[2] == 7.0);
    BOOST_CHECK(result.itemsPositions[3] == 3.0);
    BOOST_CHECK(result.itemsPositions[4] == 4.0);
    BOOST_CHECK(result.itemsPositions[5] == 5.0);

    Utils::Memory::SafeDelete(resultInternal);
    Utils::Memory::SafeDeleteCollectionOfPointers(ixs);
    delete [] result.items;
    delete [] result.itemsPositions;
}

BOOST_AUTO_TEST_SUITE_END();