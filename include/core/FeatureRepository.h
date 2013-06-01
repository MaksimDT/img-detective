#pragma once

#include "core/IndexManager.h"
#include "core/SearchResultInternal.h"
#include "core/IndexResultsCombiner.h"

#include <map>

namespace ImgDetective {
namespace Core {

    class IFeatureRepository : NONCOPYABLE, HAS_VIRTUAL_DESCTUCTOR {
	public:
		virtual SearchResultInternal* GetSimilarImgs(const REF IFeature::col_p_t& exampleFeatSet, const REF ImgQuery& initialQuery) const = 0;
        virtual void Save(imgid_t imgId, const REF IFeature::col_p_t& featureSet) const = 0;
	};

	CONCRETE SEALED class FeatureRepository : public IFeatureRepository {
	public:
        CTOR FeatureRepository(IIndexManager::col_p_t indexManagers);

        virtual SearchResultInternal* GetSimilarImgs(const REF Feature::col_p_t& exampleFeatSet, const REF ImgQuery& initialQuery) const;
        virtual void Save(imgid_t imgId, const REF IFeature::col_p_t& featureSet) const;
	private:
		IIndexManager* GetIndexManager(Feature::type_id_t featureTypeId) const;
		double GetWeightCoeff(Feature::type_id_t featureTypeId) const;
		SearchResultInternal* FuseResults(const REF std::vector<IndexSeekResult*>& indexResults, const REF ImgQuery& initialQuery) const;
		COPYCTOR FeatureRepository(const REF FeatureRepository& other);

		EXCLUSIVE IndexManager::col_p_t indexManagers;
        IndexResultsCombiner ixResultsCombiner;
		//weight factors for all the features. Must be considered when forming a result set
		std::map<Feature::type_id_t, double> featWeightCoeffs;
	};

}
}