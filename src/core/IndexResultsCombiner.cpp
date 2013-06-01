#include "core/IndexResultsCombiner.h"
#include "utils/MemoryUtils.h"

using namespace std;

namespace ImgDetective {
namespace Core {

    #pragma region IndexResultsCombiner::ImgPositionsArray impl

    CTOR IndexResultsCombiner::ImgPositionsArray::ImgPositionsArray(unsigned int capacity) {
        entries.reserve(capacity);
    }

    CTOR IndexResultsCombiner::ImgPositionsArray::ImgPositionsArray()  { }

    IndexResultsCombiner::ImgPositionsArray* IndexResultsCombiner::ImgPositionsArray::GetFromIndexResults(const IndexSeekResult& ix) {
        const IndexSeekResult::entry_col_t& entries = ix.GetEntries();

        ImgPositionsArray* result = new ImgPositionsArray(entries.size());

        IndexSeekResult::entry_col_t::const_iterator it;
        for (it = entries.cbegin(); it != entries.cend(); ++it) {
            ImgPositionsArray::entry_t entry;
            entry.imgId = it->imgId;
            entry.distance = it->distance;
            result->entries.push_back(entry);
        }

        result->SortByDistanceAsc();
        result->AssignPositions();
        result->SortByImgIdAsc();

        return result;
    }

    SearchResultInternal* IndexResultsCombiner::ImgPositionsArray::Merge(const std::vector<ImgPositionsArray*>& imgPositionArrays) {
        //prerequisite: all img position arrays must be sorted by imgid in ascending order

        //get the raw data to speed up the merging process a little bit
        const ImgPositionsArray*const* ixs = imgPositionArrays.data();
        const unsigned int ixCount = imgPositionArrays.size();
        vector<unsigned int> cursors(ixCount, 0);
        vector<bool> endOfIx(ixCount);
        unsigned int scannedIxCount = 0;
        
        unsigned int ixMaxSize = 0;
        for (unsigned int i = 0; i < ixCount; ++i) {
            if (ixs[i]->entries.size() > ixMaxSize) {
                ixMaxSize = ixs[i]->entries.size();
            }
        }

        SearchResultInternal* result = new SearchResultInternal(ixMaxSize);

        while (true) {
            //check for indexes' end
            for (unsigned int i = 0; i < ixCount; ++i) {
                if (!endOfIx[i] && cursors[i] >= ixs[i]->entries.size()) {
                    endOfIx[i] = true;
                    ++scannedIxCount;
                }
            }

            if (scannedIxCount == ixCount) {
                break;
            }

            //get min imgid from active cursors
            imgid_t minImgId = UINT_MAX;
            for (unsigned int i = 0; i < ixCount; ++i) {
                if (!endOfIx[i]) {
                    unsigned int cursorPos = cursors[i];
                    ImgPositionsArray::entry_t cursorVal = ixs[i]->entries[cursorPos];
                    if (cursorVal.imgId < minImgId) {
                        minImgId = cursorVal.imgId;
                    }
                }
            }

            //sum up all positions of the selected imgid
            double positionsSum = 0.0;
            for (unsigned int i = 0; i < ixCount; ++i) {
                if (!endOfIx[i]) {
                    unsigned int cursorPos = cursors[i];
                    ImgPositionsArray::entry_t cursorVal = ixs[i]->entries[cursorPos];
                    if (cursorVal.imgId == minImgId) {
                        positionsSum += cursorVal.position;
                        //advance cursor
                        ++cursors[i];
                    }
                }
            }

            SearchResultInternal::Node resultNode;
            resultNode.imgId = minImgId;
            resultNode.position = positionsSum;

            result->AddNode(resultNode);
        }

        return result;
    }

    void IndexResultsCombiner::ImgPositionsArray::SortByDistanceAsc() {
        sort(entries.begin(), entries.end(), [](const ImgPositionsArray::entry_t& left, const ImgPositionsArray::entry_t& right) {
            return left.distance < right.distance;
        });
    }

    void IndexResultsCombiner::ImgPositionsArray::AssignPositions() {
        vector<ImgPositionsArray::entry_t>::iterator it;
        double pos = 1;
        for (it = entries.begin(); it != entries.end(); ++it) {
            it->position = pos;
            pos += 1.0;
        }
    }

    void IndexResultsCombiner::ImgPositionsArray::SortByImgIdAsc() {
        sort(entries.begin(), entries.end(), [](const ImgPositionsArray::entry_t& left, const ImgPositionsArray::entry_t& right) {
            return left.imgId < right.imgId;
        });
    }

    #pragma endregion

    #pragma region IndexResultsCombiner impl

    SearchResultInternal* IndexResultsCombiner::CombineIndexResults(const std::vector<IndexSeekResult*>& indexResults, const ImgQuery& initialQuery) const {
        std::vector<ImgPositionsArray*> ixPositionArrays(indexResults.size(), NULL);

        try {
            for (unsigned int i = 0; i < indexResults.size(); ++i) {
                ImgPositionsArray* ar = ImgPositionsArray::GetFromIndexResults(*indexResults[i]);
                ixPositionArrays[i] = ar;
            }

            SearchResultInternal* result = ImgPositionsArray::Merge(ixPositionArrays);
            Utils::Memory::SafeDeleteCollectionOfPointers(ixPositionArrays);

            return result;
        }
        catch (...) {
            Utils::Memory::SafeDeleteCollectionOfPointers(ixPositionArrays);
            throw;
        }
    }

    #pragma endregion
}
}