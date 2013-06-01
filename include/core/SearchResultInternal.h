#pragma once

#include "common/Common.h"
#include "common/CommonInternal.h"
#include <vector>

namespace ImgDetective {
namespace Core {

    class SearchResultInternal : NONCOPYABLE {
    public:
        typedef struct Node {
            imgid_t imgId;
            double position;
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