#include "testutils/TestUtils.h"
#include "utils/FileSystemUtils.h"
#include "core/MySqlDbWrapper.h"
#include <boost/filesystem.hpp>

using namespace ImgDetective::Core;
using namespace ImgDetective::Utils;

ImgInfo* TestUtils::ReadImgFromFile(const std::string& path) {
    boost::filesystem::path boostPath = path;
    boost::filesystem::path fileExtension = boostPath.extension();
    std::string fileExtensionStr = fileExtension.string();

    blob_p_t blob = FileSystem::ReadAllBytes(path);

    RawImg shortInfo;
    memset(&shortInfo, 0, sizeof(shortInfo));
    shortInfo.content = BlobToCharArray(blob);
    shortInfo.contentSize = blob->size();
    shortInfo.fileExtension = fileExtensionStr.c_str();

    ImgInfo* img = ImgInfo::Create(shortInfo);

    SafeFreeBlob(blob);

    return img;
}

Db::DbWrapper* TestUtils::CreateDbWrapper() {
    return CreateMySqlDbWrapper();
}

Db::DbWrapper* TestUtils::CreateMySqlDbWrapper() {
    Db::MySqlConnectionSettings conSettings;
    conSettings.dbName = "img_detective_test";
    conSettings.host = "localhost";
    conSettings.login = "root";
    conSettings.password = "";
    conSettings.port = 3306;

    Db::MySqlDbWrapper* dbWrapper = new Db::MySqlDbWrapper(conSettings);

    return dbWrapper;
}