#include "core/Indexer.h"
#include "utils/MemoryUtils.h"

namespace ImgDetective {
namespace Core {

    CTOR Indexer::Indexer(REF ImgContentStorage& imgContentStorage, REF ImgMetadataStorage& imgMetadataStorage, REF IFeatureExtractor::col_p_t& featureExtractors, REF FeatureRepository& featureRepository) 
        : imgContentStorage(imgContentStorage), imgMetadataStorage(imgMetadataStorage), featureExtractors(featureExtractors), featureRepo(featureRepo) {
    }

    UploadImgResult Indexer::UploadImg(const REF ImgShortInfo& img) {
        UploadImgResult result;
        IFeature::col_p_t features;
        ImgInfo* parsedImg = NULL;

        try {
            //parse input image
            parsedImg = ImgInfo::Create(img);
            //create record for the new image in the metadata repository to obtain a unique identifier of this image
            imgMetadataStorage.InitImgRecord(REF *parsedImg);
            features = ExtractFeatures(*parsedImg);
            featureRepo.Save(parsedImg->GetId(), features);
            imgContentStorage.Write(REF *parsedImg);
            //save additional info in the metadata storage
            imgMetadataStorage.SaveImgRecord(*parsedImg);

            Utils::Memory::SafeDelete(parsedImg);
            Utils::Memory::SafeDeleteCollectionOfPointers(features);
        }
        catch (...) {
            Utils::Memory::SafeDelete(parsedImg);
            Utils::Memory::SafeDeleteCollectionOfPointers(features);

            throw;
        }

        result.opStatus = OPSTATUS_OK;
        return result;
    }

    IFeature::col_p_t Indexer::ExtractFeatures(const REF ImgInfo& imgInfo) {
        IFeature::col_p_t features;

        try {
            IFeatureExtractor::col_p_t::iterator it;
            for (it = featureExtractors.begin(); it != featureExtractors.end(); ++it) {
                IFeatureExtractor* fe = *it;
                IFeature* extractedFeature = fe->ExtractFrom(imgInfo);
                features.push_back(extractedFeature);
            }
        }
        catch (...) {
            Utils::Memory::SafeDeleteCollectionOfPointers(features);
            throw;
        }

        return features;
    }
}
}