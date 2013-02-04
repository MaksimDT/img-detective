#include "ImgDetectiveApi.h"

ImgIdArray SearchByExample(ImgQuery query) {
	ImgIdArray* result = new ImgIdArray();
	result->opStatus = OPSTATUS_INTERNAL_ERROR;
	return *result;
}

opstatus_t UploadImg(ImgShortInfo imgInfo) {
	return OPSTATUS_INTERNAL_ERROR;
}