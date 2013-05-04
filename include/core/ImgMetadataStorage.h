#pragma once

#include "core/ImgInfo.h"
#include "core/FsRepositoryInfo.h"
#include <string>
#include <boost/filesystem.hpp>

namespace ImgDetective {
namespace Core {

    class ImgMetadataStorage : NONCOPYABLE, HAS_VIRTUAL_DESCTUCTOR {
    public:
        virtual void InitImgRecord(REF ImgInfo& imgInfo) const = 0;

        virtual FsRepositoryInfo CreateFsRepositoryRecord(const boost::filesystem::path& dirPath) const = 0;
        virtual FsRepositoryInfo::col_t GetAllRepositories() const = 0;
    };

}
}