#pragma once

#include "common/Common.h"

#ifdef IMGDETECTIVE_EXPORTS
#define IMG_DETECTIVE_API __declspec(dllexport)
#else
#define IMG_DETECTIVE_API __declspec(dllimport)
#endif

extern "C" IMG_DETECTIVE_API void Search1(ImgQuery query);
extern "C" IMG_DETECTIVE_API void Search(ImgQuery query, SearchResult* result);
extern "C" IMG_DETECTIVE_API void UploadImg(RawImg imgInfo, UploadImgResult* result);
extern "C" IMG_DETECTIVE_API void CanIndexDirectory(wchar_t* dirPath, CanIndexDirectoryResult* result);
extern "C" IMG_DETECTIVE_API void IndexDirectory(wchar_t* dirPath, IndexDirectoryResult* result);