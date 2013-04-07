#pragma once

#include <assert.h>
#include "common/CommonInternal.h"
#include "utils/ContractUtils.h"

namespace ImgDetective {
namespace Utils {

	class Memory {
	public:
		template <typename T>
		static void SafeDelete(REF T*& ptr) {
			if (ptr != NULL) {
				delete ptr;
			}
			ptr = NULL;
		}

		template <typename TCollection>
		static void SafeDeleteCollectionOfPointers(REF TCollection& col) {
			TCollection::iterator it;

			for (it = col.begin(); it != col.end(); ++it) {
                if (*it != NULL) {
				    delete *it;
                    *it = NULL;
                }
			}

            col.clear();
		}

        template <typename T>
        static void SafeDeleteArray(REF T*& ptr) {
            if (ptr != NULL) {
                delete[] ptr;
                ptr = NULL;
            }
        }
	};

}
}