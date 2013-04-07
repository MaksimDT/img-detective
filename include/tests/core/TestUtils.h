#pragma once

#include "core/ImgInfo.h"

class TestUtils {
public:
    static ImgDetective::Core::ImgInfo* ReadImgFromFile(const std::string& path);
};