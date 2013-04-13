#pragma once

#include "Common.h"

#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>

//classes derived from this one will have a virtual destructor
class IDisposable {
public:
    virtual ~IDisposable() {};
};

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
#define NONCOPYABLE public boost::noncopyable
//class' constructor
#define CTOR
//class' copy constructor
#define COPYCTOR
/*denotes the exclusive ownership of object. The idea is to enforce disposing
of all class' resources that it exclusively owns in a class' destructor*/
#define EXCLUSIVE
/*denotes that the class has a virtual destructor and its 
descendants can be deleted via base class descriptor*/
#define HAS_VIRTUAL_DESCTUCTOR public IDisposable

#define BITS_PER_PIXEL sizeof(char)

namespace ImgDetective {
namespace Core {

    typedef char pixel_tone_t;
    typedef char byte_t;
    typedef std::vector<imgid_t> imgid_col_t;
    typedef std::vector<byte_t> blob_t;
    typedef std::vector<byte_t>* blob_p_t;

    void ConvertToSearchResult(const REF imgid_col_t& imgIds, REF SearchResult& result);

    #pragma region working with BLOBs

    blob_p_t CreateBlobOfSize(size_t blobSize);
    void SafeFreeBlob(REF blob_p_t& blob);
    char* BlobToCharArray(blob_p_t blob);
    void CopyToBlob(void* data, blob_p_t blob);

    #pragma endregion

    bool IsValidImgId(imgid_t imgId);
}
}