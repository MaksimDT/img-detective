#include "core/SearchSystem.h"
#include "utils/MemoryUtils.h"
#include "utils/ContractUtils.h"

using namespace std;

namespace ImgDetective {
namespace Core {

    SearchSystem::SearchSystem(IFeatureRepository& featureRepo, FeatureExtractor::col_p_t featureExtractors)
        : featureRepo(featureRepo) {
		Utils::Contract::AssertNotEmpty(featureExtractors);

		this->featureExtractors = featureExtractors;
	}

	SearchResultInternal* SearchSystem::GetSimilarImgs(ImgInfo& img, ImgQuery query) {
        Feature::col_p_t extractedFeatures;

		try {
            extractedFeatures = ExtractFeatures(img, query);
			SearchResultInternal* result = featureRepo.GetSimilarImgs(extractedFeatures, query);

            Utils::Memory::SafeDeleteCollectionOfPointers(extractedFeatures);
			return result;
		}
		catch (...) {
            Utils::Memory::SafeDeleteCollectionOfPointers(extractedFeatures);
			throw;
		}
	}

	IFeature::col_p_t SearchSystem::ExtractFeatures(ImgInfo& imgInfo, ImgQuery query) {
		FeatureExtractor::col_p_t::iterator it;
		IFeature::col_p_t result;

		for (it = featureExtractors.begin(); it != featureExtractors.end(); ++it) {
			IFeature* f = (*it)->ExtractFrom(imgInfo);
			result.push_back(f);
		}

		return result;
	}

}
}