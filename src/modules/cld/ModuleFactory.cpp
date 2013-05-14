#include "modules/cld/ModuleFactory.h"
#include "modules/cld/Feature.h"
#include "modules/cld/FeatureDeserializer.h"
#include "modules/cld/FeatureExtractor.h"
#include "core/RDBMSIndexStorage.h"
#include "core/LinearSearchIM.h"


using namespace ImgDetective::Core;

namespace ImgDetective {
namespace Modules {
namespace CLD {

    Core::IIndexManager* ModuleFactory::GetIndexManager(const Core::Db::DbWrapper& dbWrapper) {
        Core::IFeatureDeserializer* fd = new CLD::CLDFD();
        RDBMSIndexStorage* ixStorage = new RDBMSIndexStorage(fd, dbWrapper, 1000, "CLDs");
        Core::LinearSearchIM* ixMgr = new Core::LinearSearchIM(ixStorage, CLD_FTYPE_ID);

        return ixMgr;
    }

    Core::IFeatureExtractor* ModuleFactory::GetFeatureExtractor() {
        return new CLD::CLDFE();
    }

}
}
}