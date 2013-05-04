#pragma once

#include "core/IndexManager.h"
#include "core/FeatureExtractor.h"
#include "core/DbWrapper.h"

namespace ImgDetective {
namespace Modules {
namespace ColorHistogram {

    class ModuleFactory {
    public:
        static Core::IIndexManager* GetIndexManager(const Core::Db::DbWrapper& dbWrapper);
        static Core::IFeatureExtractor* GetFeatureExtractor();
    };

}
}
}