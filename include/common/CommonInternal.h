#pragma once

#include "Common.h"

#include <vector>
#include <boost/shared_ptr.hpp>

/*
Contains definitions for internal parts of system, not its API. API is a set of pure C functions, 
while this file may contain C++ definitions.
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

#define BITS_PER_PIXEL sizeof(char)

namespace ImgDetective {
namespace Core {

    typedef char pixel_tone_t;
    typedef char byte_t;
	typedef std::vector<imgid_t> imgid_col_t;
	typedef std::vector<byte_t> blob_t;
	typedef boost::shared_ptr<std::vector<byte_t>> blob_p_t;

	void ConvertToSearchResult(const REF imgid_col_t& imgIds, REF SearchResult& result);
    blob_p_t CreateBlobOfSize(size_t blobSize);
}
}