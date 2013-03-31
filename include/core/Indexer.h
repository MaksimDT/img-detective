#pragma once

#include "common/Common.h"
#include "core/FeatureRepository.h"
#include "core/ImgContentStorage.h"
#include "core/ImgMetadataStorage.h"
#include "core/FeatureExtractor.h"

namespace ImgDetective {
namespace Core {

    CONCRETE SEALED class Indexer : NONCOPYABLE {
    public:
        CTOR Indexer(
            REF ImgContentStorage& imgContentStorage, 
            REF ImgMetadataStorage& imgMetadataStorage, 
            REF IFeatureExtractor::col_p_t& featureExtractors, 
            REF FeatureRepository& featureRepository);

        UploadImgResult UploadImg(const REF ImgShortInfo& img);
    private:
        IFeature::col_p_t ExtractFeatures(const REF ImgInfo& imgInfo);

        REF ImgContentStorage& imgContentStorage;
        REF ImgMetadataStorage& imgMetadataStorage;
        IFeatureExtractor::col_p_t featureExtractors;
        REF FeatureRepository& featureRepo;
    };

}
}