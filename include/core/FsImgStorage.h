#pragma once

#include "core/ImgContentStorage.h"
#include <boost/filesystem.hpp>

namespace ImgDetective {
namespace Core {

    CONCRETE class FsImgStorage : public ImgContentStorage {
    public:
        CTOR FsImgStorage(const boost::filesystem::path& uploadDirPath);
        
        virtual void Write(ImgInfo& img) const;
        boost::filesystem::path GetImgPath(imgid_t imgId, const std::string& extension) const;
    private:
        boost::filesystem::path uploadDirPath;
    };

}
}