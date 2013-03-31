#pragma once

#include "core/ImgContentStorage.h"
#include <string>
#include <boost/filesystem.hpp>

namespace ImgDetective {
namespace Core {

    CONCRETE class FsImgStorage : public ImgContentStorage {
    public:
        CONCRETE NESTED class LookupSession {
        public:
            LookupSession(const REF boost::filesystem::directory_iterator& it);
            ImgInfo* GetNextFile();
        private:
            const REF boost::filesystem::directory_iterator& it;
        };

        FsImgStorage(const REF std::string& baseDirPath);
        
        virtual std::string Write(ImgShortInfo imgInfo, imgid_t imgId);
    private:
        std::string baseDirPath;
    };

}
}