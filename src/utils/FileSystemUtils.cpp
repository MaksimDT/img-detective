#include "utils/FileSystemUtils.h"
#include "utils/MemoryUtils.h"
#include <fstream>

using namespace std;

namespace ImgDetective {
namespace Utils {

    FileNotFoundException::FileNotFoundException(const std::string& msg)
        :exception(msg.c_str()) {}

    Core::blob_p_t FileSystem::ReadAllBytes(const std::string& path) {
        ifstream file(path, ios::in|ios::binary|ios::ate);
        if (file.is_open())
        {
            Core::blob_p_t contents;
            try {
                ifstream::pos_type size = file.tellg();
                contents = Core::CreateBlobOfSize(size);
                file.seekg(0, ios::beg);
                file.read(contents->data(), size);
                file.close();
            }
            catch (...) {
                Core::SafeFreeBlob(contents);
                throw;
            }

            return contents;
        }
        else {
            string msg = "Cannot open file ";
            msg.append(path);
            throw FileNotFoundException(msg);
        }
    }

}
}