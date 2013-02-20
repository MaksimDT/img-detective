#include "FeatureRepository.h"
#include "ContractUtils.h"

#include <vector>
using namespace std;

namespace ImgDetective {
namespace Core {

	FeatureRepository::FeatureRepository(IndexManager::col_p_t indexManagers) {
		Utils::Contract::AssertNotEmpty(indexManagers);
		this->indexManagers = indexManagers;
	}

	imgid_col_t FeatureRepository::GetSimilarImgs(Feature::col_p_t exampleFeatSet, ImgQuery initialQuery) const {
		//user input validation must be done in other modules
		Utils::Contract::AssertNotEmpty(exampleFeatSet);
		Utils::Contract::Assert(initialQuery.pageSize > 0);
		Utils::Contract::Assert(initialQuery.pageNumber > 0);

		std::vector<IndexSeekResult*> indexResults;
		indexResults.reserve(exampleFeatSet.size());

		Feature::col_p_t::iterator it;
		for (it = exampleFeatSet.begin(); it != exampleFeatSet.end(); ++it) {
			Feature::type_id_t featTypeId = (*it)->GetTypeId();
			IndexManager* im = GetIndexManager(featTypeId);

			if (im != NULL) {
				IndexSeekResult* seekResult = im->Search(*it, initialQuery);
				seekResult->ApplyWeightCoeff(GetWeightCoeff(featTypeId));
				indexResults.push_back(seekResult);
			}
		}

		return FuseResults(indexResults);
	}

}
}