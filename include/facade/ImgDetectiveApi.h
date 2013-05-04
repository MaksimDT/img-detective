#pragma once

#include "common/Common.h"

#ifdef IMGDETECTIVE_EXPORTS
#define IMG_DETECTIVE_API __declspec(dllexport)
#else
#define IMG_DETECTIVE_API __declspec(dllimport)
#endif

extern "C" IMG_DETECTIVE_API SearchResult SearchByExample(ImgQuery query);
extern "C" IMG_DETECTIVE_API UploadImgResult UploadImg(RawImg imgInfo);
extern "C" IMG_DETECTIVE_API IndexDirectoryResult IndexDirectory(wchar_t* dirPath);
extern "C" IMG_DETECTIVE_API IndexDirectoryResult IndexDirectoryStub(wchar_t* dirPath);
//extern "C" IMG_DETECTIVE_API IndexDirectoryAsyncResult IndexDirectoryAsync(wchar_t* dirPath);