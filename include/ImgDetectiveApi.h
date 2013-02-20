#pragma once

#ifdef IMGDETECTIVE_EXPORTS
#define IMG_DETECTIVE_API __declspec(dllexport)
#else
#define IMG_DETECTIVE_API __declspec(dllimport)
#endif

#define OPSTATUS_OK 0
#define OPSTATUS_INTERNAL_ERROR 1
#define IMGID_UNDEFINED 0
typedef long unsigned int imgid_t;
typedef int opstatus_t;

typedef struct {
	opstatus_t opStatus;
	long unsigned int arraySize;
	imgid_t* items;
} SearchResult;

typedef struct {
	opstatus_t opStatus;
} UploadImgResult;

typedef struct {
	long unsigned int contentSize;
	unsigned char* content;
	char* mimeType;
	imgid_t imgId;
} ImgShortInfo;

typedef struct {
	unsigned int pageSize;
	unsigned int pageNumber;
	unsigned int maxResults;
	ImgShortInfo example;
} ImgQuery;

extern "C" IMG_DETECTIVE_API SearchResult SearchByExample(ImgQuery query);
extern "C" IMG_DETECTIVE_API UploadImgResult UploadImg(ImgShortInfo imgInfo);