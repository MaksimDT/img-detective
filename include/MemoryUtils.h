#pragma once

#include <assert.h>

namespace ImgDetective {
namespace Utils {

	class Memory {
	public:
		template <typename T>
		static void AssertIsNotNull(T* ptr) {
			assert(ptr != NULL);
		}

		template <typename T>
		static void SafeDelete(T* ptr) {
			if (ptr != NULL) {
				delete ptr;
			}
			ptr = NULL;
		}
	};

}
}