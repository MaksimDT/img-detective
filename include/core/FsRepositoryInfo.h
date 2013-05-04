#pragma once

#include "common/CommonInternal.h"
#include <string>
#include <boost/filesystem.hpp>
#include <vector>


namespace ImgDetective {
namespace Core {

    class FsRepositoryInfo {
    public:
        typedef std::vector<FsRepositoryInfo> col_t;

        CTOR FsRepositoryInfo(boost::filesystem::path path, repoid_t id = REPO_ID_UNDEFINED);

        repoid_t GetId() const;
        void SetId(repoid_t id);
        boost::filesystem::path GetPath() const;
        void SetPath(const boost::filesystem::path& path);
    private:
        repoid_t id;
        boost::filesystem::path path;
    };

}
}