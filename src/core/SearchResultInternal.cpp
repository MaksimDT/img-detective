#include "core/SearchResultInternal.h"

#include <vector>

using namespace std;

namespace ImgDetective {
namespace Core {

    SearchResultInternal::SearchResultInternal() {}

    SearchResultInternal::SearchResultInternal(size_t expectedSize) {
        this->nodes.reserve(expectedSize);
    }

    void SearchResultInternal::AddNode(const SearchResultInternal::Node& node) {
        this->nodes.push_back(node);
    }

    SearchResult SearchResultInternal::ConvertToSearchResult(void* (*arrayMemAllocator)(size_t), void (*arrayMemDeallocator)(void*)) const {
        SearchResult converted;
        memset(&converted, 0, sizeof(SearchResult));

        size_t arraySize = this->nodes.size();
        converted.arraySize = arraySize;

        try {
            converted.items = (imgid_t*)arrayMemAllocator(arraySize * sizeof(imgid_t));
            converted.itemsRelevance = (double*)arrayMemAllocator(arraySize * sizeof(double));

            for (size_t i = 0; i < arraySize; ++i) {
                converted.items[i] = this->nodes[i].imgId;
                converted.itemsRelevance[i] = this->nodes[i].relevance;
            }

            return converted;
        }
        catch (...) {
            if (converted.items != NULL) {
                arrayMemDeallocator(converted.items);
                converted.items = NULL;
            }
            if (converted.itemsRelevance != NULL) {
                arrayMemDeallocator(converted.itemsRelevance);
                converted.itemsRelevance = NULL;
            }

            throw;
        }
    }
}
}