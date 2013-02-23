#pragma once

#include "IFeatureRepository.h"
#include "IndexManager.h"
#include <map>

namespace ImgDetective {
namespace Core {

	CONCRETE SEALED class FeatureRepository : public IFeatureRepository {
	public:
		FeatureRepository(IndexManager::col_p_t indexManagers);
		virtual ~FeatureRepository();

		virtual imgid_col_t GetSimilarImgs(const REF Feature::col_p_t& exampleFeatSet, const REF ImgQuery& initialQuery) const;
	private:
		IndexManager* GetIndexManager(Feature::type_id_t featureTypeId) const;
		double GetWeightCoeff(Feature::type_id_t featureTypeId) const;
		imgid_col_t FuseResults(const REF std::vector<IndexSeekResult*>& indexResults, const REF ImgQuery& initialQuery) const;

		IndexManager::col_p_t indexManagers;
		//weight factors for all the features. Must be considered when forming a result set
		std::map<Feature::type_id_t, double> featWeightCoeffs;
	};

}
}