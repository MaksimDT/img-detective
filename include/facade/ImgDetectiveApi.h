#pragma once

#include "common/Common.h"

#ifdef IMGDETECTIVE_EXPORTS
#define IMG_DETECTIVE_API __declspec(dllexport)
#else
#define IMG_DETECTIVE_API __declspec(dllimport)
#endif

extern "C" IMG_DETECTIVE_API SearchResult SearchByExample(ImgQuery query);
extern "C" IMG_DETECTIVE_API UploadImgResult UploadImg(ImgShortInfo imgInfo);