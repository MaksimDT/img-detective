#pragma once

/*
Contains definitions that can be used both by internal logic and in system's API
*/

#define OPSTATUS_OK 0
#define OPSTATUS_INTERNAL_ERROR 1
#define IMGID_UNDEFINED 0
#define REPO_ID_UNDEFINED 0
#define IMGID_IS_SIGNED false
typedef long long unsigned int imgid_t;
typedef int opstatus_t;
typedef unsigned long repoid_t;

typedef struct SearchResult {
	opstatus_t opStatus;
	long unsigned int arraySize;
	imgid_t* items;
} SearchResult;

typedef struct UploadImgResult {
	opstatus_t opStatus;
} UploadImgResult;

typedef struct IndexDirectoryResult {
    opstatus_t opStatus;
} IndexDirectoryResult;

typedef struct IndexDirectoryAsyncResult {
    unsigned long int processId;
    opstatus_t opStatus;
} IndexDirectoryAsyncResult;

typedef struct RawImg {
	long unsigned int contentSize;
	char* content;
    const char* fileExtension;
	/*char* mimeType;
	imgid_t imgId;
    const wchar_t* filePath;
    unsigned int filePathLength;
    repoid_t repoId;*/
} RawImg;

typedef struct ImgQuery {
	unsigned int pageSize;
	unsigned int pageNumber;
	unsigned int maxResults;
	RawImg example;
} ImgQuery;