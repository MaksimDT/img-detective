#pragma once

#ifndef IMGDETECTIVECORE_EXPORTS
#define IMG_DETECTIVE_API __declspec(dllexport)
#else
#define IMG_DETECTIVE_API __declspec(dllimport)
#endif

#define OPSTATUS_OK 0
#define OPSTATUS_INTERNAL_ERROR 1
#define IMGID_UNDEFINED 0

typedef long unsigned int imgid_t;
typedef int opstatus_t;

typedef struct ImgIdArray {
	opstatus_t opStatus;
	long unsigned int arraySize;
	imgid_t* items;
};

typedef struct ImgShortInfo {
	long unsigned int contentSize;
	unsigned char* content;
	char* mimeType;
	imgid_t imgId;
};

typedef struct ImgQuery {
	ImgShortInfo example;
};

extern "C" IMG_DETECTIVE_API ImgIdArray SearchByExample(ImgQuery query);
extern "C" IMG_DETECTIVE_API opstatus_t UploadImg(ImgShortInfo imgInfo);