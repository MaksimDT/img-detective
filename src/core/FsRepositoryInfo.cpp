#include "core/FsRepositoryInfo.h"
#include <algorithm>

using namespace std;

namespace ImgDetective {
namespace Core {

    CTOR FsRepositoryInfo::FsRepositoryInfo(boost::filesystem::path path, repoid_t id) {
        if (!path.is_absolute()) {
            throw std::exception("only absolute paths are allowed");
        }

        this->SetPath(path);
        this->SetId(id);
    }

    repoid_t FsRepositoryInfo::GetId() const {
        return this->id;
    }

    void FsRepositoryInfo::SetId(repoid_t id) {
        this->id = id;
    }

    boost::filesystem::path FsRepositoryInfo::GetPath() const {
        return path;
    }

    void FsRepositoryInfo::SetPath(const boost::filesystem::path& path) {
        wstring pathWstr = path.wstring();
        replace(pathWstr.begin(), pathWstr.end(), '\\', '/');

        this->path = boost::filesystem::path(pathWstr);
    }
}
}