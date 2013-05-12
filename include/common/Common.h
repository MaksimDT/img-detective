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
    double* itemsRelevance;
} SearchResult;

typedef struct UploadImgResult {
	opstatus_t opStatus;
} UploadImgResult;

typedef struct CanIndexDirectoryResult {
    enum Enum {
        Unknown = 0,
        AvailableForIndex = 1,
        AlreadyIndexed = 2,
        NotAbsolute = 3,
        NotExists = 4,
        SubdirIndexed = 5,    //subdirectory of the specified directory is indexed. Cannot index the specified directory
        IsNotDir = 6
    };

    Enum resultCode;
    opstatus_t opStatus;
} CanIndexDirectoryResult;

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
} RawImg;

typedef struct ImgQuery {
    long unsigned int exampleContentSize;
    char* exampleContent;
    double tolerance;
} ImgQuery;