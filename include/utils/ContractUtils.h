#pragma once

#include <assert.h>
#include <stdexcept>
#include <string>

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
			bool isNotNull = (ptr != NULL);

			#ifdef _DEBUG
				assert(isNotNull);
			#else
				if (isNotNull) {
					throw AssertFailedException(msg != NULL ? msg : "");
				}
			#endif
		}

		template <typename T>
		static void AssertIsNull(T* ptr, char* msg = NULL) {
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
				assert(!isEmpty);
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

        static void Ensure(bool condition, char* msg = NULL) {
            Assert(condition, msg);
        }

		static void AssertNotEmpty(const std::string& str, char* msg = NULL) {
			bool isEmpty = (str.length() == 0);

			#ifdef _DEBUG
				assert(isEmpty);
			#else
				if (isEmpty) {
					throw AssertFailedException(msg != NULL ? msg : "");
				}
			#endif
		}
	};

}
}