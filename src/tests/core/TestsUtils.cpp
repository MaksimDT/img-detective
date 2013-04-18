#include "tests/core/TestUtils.h"
#include "utils/FileSystemUtils.h"

using namespace ImgDetective::Core;
using namespace ImgDetective::Utils;

ImgInfo* TestUtils::ReadImgFromFile(const std::string& path) {
    blob_p_t blob = FileSystem::ReadAllBytes(path);

    ImgShortInfo shortInfo;
    shortInfo.content = BlobToCharArray(blob);
    shortInfo.contentSize = blob->size();

    ImgInfo* img = ImgInfo::Create(shortInfo);

    SafeFreeBlob(blob);

    return img;
}