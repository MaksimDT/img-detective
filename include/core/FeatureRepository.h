#pragma once

#include "core/IndexManager.h"

#include <map>

namespace ImgDetective {
namespace Core {

    INTERFACE class IFeatureRepository : NONCOPYABLE, HAS_VIRTUAL_DESCTUCTOR {
	public:
		virtual imgid_col_t GetSimilarImgs(const REF IFeature::col_p_t& exampleFeatSet, const REF ImgQuery& initialQuery) const = 0;
        virtual void Save(imgid_t imgId, const REF IFeature::col_p_t& featureSet) = 0;
	};

	CONCRETE SEALED class FeatureRepository : public IFeatureRepository {
	public:
        CTOR FeatureRepository(IIndexManager::col_p_t indexManagers);
        virtual ~FeatureRepository();

        virtual imgid_col_t GetSimilarImgs(const REF Feature::col_p_t& exampleFeatSet, const REF ImgQuery& initialQuery) const;
        virtual void Save(imgid_t imgId, const REF IFeature::col_p_t& featureSet);
	private:
		IIndexManager* GetIndexManager(Feature::type_id_t featureTypeId) const;
		double GetWeightCoeff(Feature::type_id_t featureTypeId) const;
		imgid_col_t FuseResults(const REF std::vector<IndexSeekResult*>& indexResults, const REF ImgQuery& initialQuery) const;
		COPYCTOR FeatureRepository(const REF FeatureRepository& other);

		EXCLUSIVE IndexManager::col_p_t indexManagers;
		//weight factors for all the features. Must be considered when forming a result set
		std::map<Feature::type_id_t, double> featWeightCoeffs;
	};

}
}