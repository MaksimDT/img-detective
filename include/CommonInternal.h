#pragma once

#include "ImgDetectiveApi.h"

#include <vector>

/*
Contains definitions for internal parts of dll, not its interface. Interface is pure C, while this file may contain C++ definitions.
*/

//input parameter
#define IN
//output parameter
#define OUT
//denotes the C++ reference
#define REF
//abstract class (that cannot be instantiated)
#define ABSTRACT
//concrete class (that CAN be instantiated)
#define CONCRETE
//sealed class (that cannot be inherited from)
#define SEALED
//interface class (that contains only pure virtual methods)
#define INTERFACE
//nested class or struct (that is declared inside other class)
#define NESTED
//noncopyable class (instance of which cannot be copied)
#define NONCOPYABLE
//class' constructor
#define CTOR
//class' copy constructor
#define COPYCTOR
/*denotes the exclusive ownership of object. The idea is to enforce disposing
of all class' resources that it exclusively owns in a class' destructor*/
#define EXCLUSIVE

namespace ImgDetective {
namespace Core {

	typedef std::vector<imgid_t> imgid_col_t;
	typedef std::vector<char> blob_t;
	typedef std::vector<char>* blob_p_t;

	void ConvertToSearchResult(imgid_col_t imgIds, REF SearchResult& result);
}
}