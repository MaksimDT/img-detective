#include "modules/ehd/ModuleFactory.h"
#include "modules/ehd/Feature.h"
#include "modules/ehd/FeatureDeserializer.h"
#include "modules/ehd/FeatureExtractor.h"
#include "core/RDBMSIndexStorage.h"
#include "core/LinearSearchIM.h"


using namespace ImgDetective::Core;

namespace ImgDetective {
namespace Modules {
namespace EHD {

    Core::IIndexManager* ModuleFactory::GetIndexManager(const Core::Db::DbWrapper& dbWrapper) {
        Core::IFeatureDeserializer* fd = new EHD::EHDFD();
        RDBMSIndexStorage* ixStorage = new RDBMSIndexStorage(fd, dbWrapper, 1000, "EHDs");
        Core::LinearSearchIM* ixMgr = new Core::LinearSearchIM(ixStorage, EHD_FTYPE_ID);

        return ixMgr;
    }

    Core::IFeatureExtractor* ModuleFactory::GetFeatureExtractor() {
        return new EHD::EHDFE();
    }

}
}
}