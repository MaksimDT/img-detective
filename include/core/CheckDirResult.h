#pragma once

namespace ImgDetective {
namespace Core {

    struct CheckDirResult {
        enum Enum {
            Unknown = 0,
            AvailableForIndex,
            AlreadyIndexed,
            NotAbsolute,
            NotExists,
            SubdirIndexed,    //subdirectory of the specified directory is indexed. Cannot index the specified directory
            IsNotDir
        };
    };

}
}