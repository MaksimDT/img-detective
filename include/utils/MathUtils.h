#pragma once

#include "utils/ContractUtils.h"

namespace ImgDetective {
namespace Utils {

    class Math {
    public:
        static unsigned int IntPow(unsigned int base, unsigned int exponent);
        //returns number of states that chunk of memory with specified size can be in 
        static unsigned int GetNumberOfStates(size_t sizeInBytes);
    };

}
}