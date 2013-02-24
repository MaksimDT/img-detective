#include "common/CommonInternal.h"
#include "utils/ContractUtils.h"

namespace ImgDetective  {
namespace Core {

	void ConvertToSearchResult(const REF imgid_col_t& imgIds, REF SearchResult& result) {
		Utils::Contract::AssertIsNull(result.items);

		result.arraySize = imgIds.size();
		if (imgIds.size() != 0) {
			result.items = new imgid_t[imgIds.size()];
			
			size_t resultIndex = 0;
			imgid_col_t::const_iterator it;
			for (it = imgIds.cbegin(); it != imgIds.cend(); ++it) {
				result.items[resultIndex] = *it;
				++resultIndex;
			}
		}
	}

}
}