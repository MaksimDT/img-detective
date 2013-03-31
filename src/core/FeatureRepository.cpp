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

	FeatureRepository::~FeatureRepository() {
		Utils::Memory::SafeDeleteCollectionOfPointers(indexManagers);
	}

	imgid_col_t FeatureRepository::GetSimilarImgs(const REF Feature::col_p_t& exampleFeatSet, const REF ImgQuery& initialQuery) const {
		//user input validation must be done in other modules
		Utils::Contract::AssertNotEmpty(exampleFeatSet);
		Utils::Contract::Assert(initialQuery.pageSize > 0);
		Utils::Contract::Assert(initialQuery.pageNumber > 0);

		std::vector<IndexSeekResult*> indexResults;
		indexResults.reserve(exampleFeatSet.size());

		IFeature::col_p_t::const_iterator it;
		for (it = exampleFeatSet.begin(); it != exampleFeatSet.end(); ++it) {
			IFeature* pFeature = *it;
			Feature::type_id_t featTypeId = pFeature->GetTypeId();
			IIndexManager* im = GetIndexManager(featTypeId);

			if (im != NULL) {
				IndexSeekResult* seekResult = im->Search(*pFeature, initialQuery);
				seekResult->ApplyWeightCoeff(GetWeightCoeff(featTypeId));
				indexResults.push_back(seekResult);
			}
		}

		return FuseResults(REF indexResults, REF initialQuery);
	}

	IIndexManager* FeatureRepository::GetIndexManager(Feature::type_id_t featureTypeId) const {
		IIndexManager::col_p_t::const_iterator it;

		for (it = indexManagers.cbegin(); it != indexManagers.cend(); ++it) {
			IIndexManager* im = *it;
			if (im->GetFeatureTypeId() == featureTypeId) {
				return im;
			}
		}

		return NULL;
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

	imgid_col_t FeatureRepository::FuseResults(const REF vector<IndexSeekResult*>& indexResults, const REF ImgQuery& initialQuery) const {
		//TODO: implement

		if (indexResults.size() == 0) {
			return imgid_col_t();
		}

		imgid_col_t result;
		IndexSeekResult::entry_col_t::const_iterator it;

		for (it = indexResults[0]->GetEntries().cbegin(); it != indexResults[0]->GetEntries().cend(); ++it) {
			result.push_back(it->imgId);
		}

		return result;
	}

}
}