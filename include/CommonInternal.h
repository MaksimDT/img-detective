#pragma once

#include "ImgDetectiveApi.h"

#include <vector>

/*
Contains definitions for internal parts of dll, not its interface. Interface is pure C, while this file may contain C++ definitions.
*/

#define IN
#define OUT
#define REF

namespace ImgDetective {
namespace Core {

	typedef std::vector<imgid_t> imgid_col_t;
	typedef std::vector<char> blob_t;
	typedef std::vector<char>* blob_p_t;

}
}