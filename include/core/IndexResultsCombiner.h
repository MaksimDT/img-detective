#pragma once

#include "core/SearchResultInternal.h"
#include "core/IndexSeekResult.h"

namespace ImgDetective {
namespace Core {

    class IndexResultsCombiner {
    public:
        SearchResultInternal* CombineIndexResults(const std::vector<IndexSeekResult*>& indexResults, const ImgQuery& initialQuery) const;
    private:
        class ImgPositionsArray {
        public:
            typedef struct entry_t {
                imgid_t imgId;
                double position;
                double distance;
            };

            CTOR ImgPositionsArray(unsigned int capacity);
            static ImgPositionsArray* GetFromIndexResults(const IndexSeekResult& ix);
            static SearchResultInternal* Merge(const std::vector<ImgPositionsArray*>& imgPositionArrays);
                
            void SortByDistanceAsc();
            void AssignPositions();
            void SortByImgIdAsc();

        private:
            CTOR ImgPositionsArray();

            std::vector<entry_t> entries;
        };
    };

}
}