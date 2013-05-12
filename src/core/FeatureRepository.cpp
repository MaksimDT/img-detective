#include "core/FeatureRepository.h"
#include "utils/ContractUtils.h"
#include "utils/MemoryUtils.h"

#include <vector>
using namespace std;

namespace ImgDetective {
namespace Core {

	CTOR FeatureRepository::FeatureRepository(IndexManager::col_p_t indexManagers) {
		Utils::Contract::AssertNotEmpty(indexManagers);
		this->indexManagers = indexManagers;
	}

	SearchResultInternal FeatureRepository::GetSimilarImgs(const REF Feature::col_p_t& exampleFeatSet, const REF ImgQuery& initialQuery) const {
		//user input validation must be done in other modules
		Utils::Contract::AssertNotEmpty(exampleFeatSet);

		std::vector<IndexSeekResult*> indexResults;
		indexResults.reserve(exampleFeatSet.size());

		IFeature::col_p_t::const_iterator it;
		for (it = exampleFeatSet.begin(); it != exampleFeatSet.end(); ++it) {
			IFeature* pFeature = *it;
			Feature::type_id_t featTypeId = pFeature->GetTypeId();
			IIndexManager* im = GetIndexManager(featTypeId);

            if (im == NULL) {
                continue;
            }

            IndexSeekResult* seekResult = NULL;
            try {
                seekResult = im->Search(*pFeature, initialQuery);
            }
            catch (...) {
                //TODO: logging
                Utils::Memory::SafeDelete(seekResult);
                continue;
            }
			
            indexResults.push_back(seekResult);
		}

		SearchResultInternal fusedResult = FuseResults(REF indexResults, REF initialQuery);

        Utils::Memory::SafeDeleteCollectionOfPointers(indexResults);

        return fusedResult;
	}

    void FeatureRepository::Save(imgid_t imgId, const REF IFeature::col_p_t& featureSet) const {
        Utils::Contract::Assert(imgId != IMGID_UNDEFINED);
        
        IFeature::col_p_t::const_iterator featIt;
        for (featIt = featureSet.cbegin(); featIt != featureSet.cend(); ++featIt) {
            IFeature* curFeat = *featIt;
            IIndexManager* ixMgr = GetIndexManager(curFeat->GetTypeId());

            ixMgr->AddFeature(*curFeat, imgId);
        }
    }

	IIndexManager* FeatureRepository::GetIndexManager(Feature::type_id_t featureTypeId) const {
		IIndexManager::col_p_t::const_iterator it;

		for (it = indexManagers.cbegin(); it != indexManagers.cend(); ++it) {
			IIndexManager* im = *it;
			if (im->GetFeatureTypeId() == featureTypeId) {
				return im;
			}
		}

        throw std::exception("couldn't find index manager for the specified feature");
	}

	double FeatureRepository::GetWeightCoeff(Feature::type_id_t featureTypeId) const {
		map<Feature::type_id_t, double>::const_iterator it = featWeightCoeffs.find(featureTypeId);

		if (it != featWeightCoeffs.cend()) {
			return it->second;
		}
		else {
			return 1.0;
		}
	}

	SearchResultInternal FeatureRepository::FuseResults(const REF vector<IndexSeekResult*>& indexResults, const REF ImgQuery& initialQuery) const {
		//TODO: implement

		if (indexResults.size() == 0) {
			return SearchResultInternal();
		}

		SearchResultInternal result;
		IndexSeekResult::entry_col_t::const_iterator it;

		for (it = indexResults[0]->GetEntries().cbegin(); it != indexResults[0]->GetEntries().cend(); ++it) {
            SearchResultInternal::Node node;
            node.imgId = it->imgId;
            node.relevance = it->distance.GetRelevanceValue();
            result.AddNode(node);
		}

		return result;
	}

}
}