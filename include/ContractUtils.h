#pragma once

#include <assert.h>
#include <stdexcept>

namespace ImgDetective {
namespace Utils {

	class AssertFailedException : public std::exception {
	public:
		AssertFailedException(char* msg)
			:std::exception(msg) {}
	};

	class Contract {
	public:
		template <typename T>
		static void AssertNotNull(T* ptr, char* msg = NULL) {
			bool isNull = (ptr == NULL);

			#ifdef _DEBUG
				assert(isNull);
			#else
				if (isNull) {
					throw AssertFailedException(msg != NULL ? msg : "");
				}
			#endif
		}

		template <typename TCollection>
		static void AssertNotEmpty(TCollection col, char* msg = NULL) {
			bool isEmpty = col.empty();

			#ifdef _DEBUG
				assert(isEmpty);
			#else
				if (isEmpty) {
					throw AssertFailedException(msg != NULL ? msg : "");
				}
			#endif
		}

		static void Assert(bool condition, char* msg = NULL) {
			#ifdef _DEBUG
				assert(condition);
			#else
				if (condition) {
					throw AssertFailedException(msg != NULL ? msg : "");
				}
			#endif
		}
	};

}
}