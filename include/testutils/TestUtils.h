#pragma once

#include "core/ImgInfo.h"
#include "core/DbWrapper.h"

class TestUtils {
public:
    static ImgDetective::Core::ImgInfo* ReadImgFromFile(const std::string& path);
    static ImgDetective::Core::Db::DbWrapper* CreateDbWrapper();
    static ImgDetective::Core::Db::DbWrapper* CreateMySqlDbWrapper();
};