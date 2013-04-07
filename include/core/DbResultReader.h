#pragma once

#include "common/CommonInternal.h"
#include "core/BindInfo.h"
#include "core/FieldsDictionary.h"

namespace ImgDetective {
namespace Core {
namespace Db {

    class DbResultReader : NONCOPYABLE, HAS_VIRTUAL_DESCTUCTOR {
    public:
        virtual FieldsDictionary* Next() = 0;
    };

}
}
}