#include "SearchSystem.h"
#include "MemoryUtils.h"

using namespace std;

namespace ImgDetective {
namespace Core {

	imgid_col_t SearchSystem::GetSimilarImgs(ImgQuery query) {
		ImgInfo* imgInfo;

		try {
			//constuct internal representation of example image
			imgInfo = ImgInfo::Create(query.example);

			//find out if the example image already exists in our storage
			bool indexed = SearchForImageInIndex(*imgInfo);

			imgid_col_t result;

			if (indexed) {
				result = featureRepo->FindSimilarImgs(imgInfo->GetInternalId(), query);
			}
			else {
				//extract features from example image
				Feature::col_p_t extractedFeatures = ExtractFeatures(*imgInfo, query);
				result = featureRepo->FindSimilarImgs(extractedFeatures, query);
			}

			Utils::Memory::SafeDelete(imgInfo);
		}
		catch (...) {
			Utils::Memory::SafeDelete(imgInfo);
			throw;
		}
	}

	bool SearchSystem::SearchForImageInIndex(ImgInfo& imgInfo) {
		imgid_t existingId;
		bool indexed = false;
		if (contentsRepo->SearchForImage(imgInfo, OUT existingId, OUT indexed)) {
			if (!imgInfo.HasInternalId()) {
				imgInfo.SetInternalId(existingId);
			}
		}

		return indexed;
	}
}
}