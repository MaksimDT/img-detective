#pragma once

#include "common/Common.h"
#include "core/FeatureRepository.h"
#include "core/ImgContentStorage.h"
#include "core/ImgMetadataStorage.h"
#include "core/FeatureExtractor.h"
#include "core/FsRepositoryInfo.h"
#include "core/CheckDirResult.h"
#include <boost/filesystem.hpp>

namespace ImgDetective {
namespace Core {

    CONCRETE SEALED class Indexer : NONCOPYABLE {
    public:
        CTOR Indexer(
            REF ImgContentStorage& imgContentStorage, 
            REF ImgMetadataStorage& imgMetadataStorage, 
            REF IFeatureExtractor::col_p_t& featureExtractors, 
            REF FeatureRepository& featureRepository);
        ~Indexer();

        UploadImgResult UploadImg(ImgInfo& img) const;

        void IndexDirectory(const boost::filesystem::path& dirPath) const;
        CheckDirResult::Enum CanIndexDirectory(const boost::filesystem::path& dirPath) const;
    private:
        void AddToIndex(ImgInfo& img) const;

        IFeature::col_p_t ExtractFeatures(const ImgInfo& imgInfo) const;
        bool IsOfSupportedFormat(const boost::filesystem::path& filePath) const;

        REF ImgContentStorage& imgContentStorage;
        REF ImgMetadataStorage& imgMetadataStorage;
        IFeatureExtractor::col_p_t featureExtractors;
        REF FeatureRepository& featureRepo;
    };

}
}