#pragma once

/*
Contains definitions that can be used both by internal logic and in system's API
*/

#define OPSTATUS_OK 0
#define OPSTATUS_INTERNAL_ERROR 1
#define IMGID_UNDEFINED 0
#define IMGID_IS_SIGNED false
typedef long long unsigned int imgid_t;
typedef int opstatus_t;

typedef struct SearchResult {
	opstatus_t opStatus;
	long unsigned int arraySize;
	imgid_t* items;
} SearchResult;

typedef struct UploadImgResult {
	opstatus_t opStatus;
} UploadImgResult;

typedef struct ImgShortInfo {
	long unsigned int contentSize;
	char* content;
	char* mimeType;
	imgid_t imgId;
} ImgShortInfo;

typedef struct ImgQuery {
	unsigned int pageSize;
	unsigned int pageNumber;
	unsigned int maxResults;
	ImgShortInfo example;
} ImgQuery;