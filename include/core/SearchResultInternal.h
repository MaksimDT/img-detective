#pragma once

#include "common/Common.h"
#include <vector>

namespace ImgDetective {
namespace Core {

    class SearchResultInternal {
    public:
        typedef struct Node {
            imgid_t imgId;
            double relevance;
        };

        SearchResultInternal();
        SearchResultInternal(size_t expectedSize);
        void AddNode(const Node& node);

        SearchResult ConvertToSearchResult(void* (*arrayMemAllocator)(size_t), void (*arrayMemDeallocator)(void*)) const;
    private:
        std::vector<Node> nodes;
    };

}
}