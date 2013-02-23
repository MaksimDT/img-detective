#pragma once

#include <assert.h>

namespace ImgDetective {
namespace Utils {

	class Memory {
	public:
		template <typename T>
		static void SafeDelete(T* ptr) {
			if (ptr != NULL) {
				delete ptr;
			}
			ptr = NULL;
		}

		template <typename TCollection>
		static void SafeDeleteCollectionOfPointers(TCollection col) {
			TCollection::iterator it;

			for (it = col.begin(); it != col.end(); ++it) {
				SafeDelete((*it));
			}
		}
	};

}
}