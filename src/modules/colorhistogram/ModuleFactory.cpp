#include "modules/colorhistogram/ModuleFactory.h"
#include "modules/colorhistogram/Feature.h"
#include "modules/colorhistogram/FeatureDeserializer.h"
#include "modules/colorhistogram/FeatureExtractor.h"
#include "core/RDBMSIndexStorage.h"
#include "core/LinearSearchIM.h"


using namespace ImgDetective::Core;

namespace ImgDetective {
namespace Modules {
namespace ColorHistogram {

    Core::IIndexManager* ModuleFactory::GetIndexManager(const Core::Db::DbWrapper& dbWrapper) {
        Core::IFeatureDeserializer* fd = new ColorHistogram::ColorHistogramFD();
        RDBMSIndexStorage* ixStorage = new RDBMSIndexStorage(fd, dbWrapper, 1000, "ColorHistograms");
        Core::LinearSearchIM* ixMgr = new Core::LinearSearchIM(ixStorage, COLOR_HISTOGRAM_FTYPE_ID);

        return ixMgr;
    }

    Core::IFeatureExtractor* ModuleFactory::GetFeatureExtractor() {
        return new ColorHistogram::ColorHistogramFE();
    }

}
}
}