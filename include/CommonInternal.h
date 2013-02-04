#pragma once

#include "ImgDetectiveApi.h"

#include <vector>

/*
Contains definitions for internal parts of dll, not its interface. Interface is pure C, while this file may contain C++ definitions.
*/

#define IN
#define OUT

typedef std::vector<imgid_t> imgid_col_t;