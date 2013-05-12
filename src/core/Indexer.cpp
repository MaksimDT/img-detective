#include "core/Indexer.h"
#include "utils/MemoryUtils.h"
#include "utils/FileSystemUtils.h"
#include <algorithm>

namespace ImgDetective {
namespace Core {

    CTOR Indexer::Indexer(REF ImgContentStorage& imgContentStorage, REF ImgMetadataStorage& imgMetadataStorage, REF IFeatureExtractor::col_p_t& featureExtractors, REF IFeatureRepository& featureRepository) 
        : imgContentStorage(imgContentStorage), imgMetadataStorage(imgMetadataStorage), featureExtractors(featureExtractors), featureRepo(featureRepository) {
    }

    UploadImgResult Indexer::UploadImg(ImgInfo& img) const {
        AddToIndex(img);
        this->imgContentStorage.Write(img);

        UploadImgResult result;
        result.opStatus = OPSTATUS_OK;
        return result;
    }

    void Indexer::IndexDirectory(const boost::filesystem::path& dirPath) const {
        CanIndexDirectoryResult::Enum checkResult = CanIndexDirectory(dirPath);
        if (checkResult != CanIndexDirectoryResult::AvailableForIndex) {
            throw std::exception("The specified directory cannot be indexed");
        }

        FsRepositoryInfo newRepo = this->imgMetadataStorage.CreateFsRepositoryRecord(dirPath);

        boost::filesystem::recursive_directory_iterator it(newRepo.GetPath());
        boost::filesystem::recursive_directory_iterator end;

        //recursively visit all files in the directory and its subdirectories
        while (it != end) {
            boost::filesystem::path curPath = *it;
            ++it;
            ImgInfo* curImage = NULL;

            try {
                //not going to symlinks
                if (boost::filesystem::is_directory(curPath) && boost::filesystem::is_symlink(curPath)) {
                    it.no_push();
                }

                //here we have an image file. Lets read it from file system and upload to our index
                if (boost::filesystem::is_regular_file(curPath) && IsOfSupportedFormat(curPath)) {
                    boost::filesystem::path relCurPath = Utils::FileSystem::MakeRelative(newRepo.GetPath(), curPath);
                    curImage = ImgInfo::Create(newRepo.GetId(), curPath, relCurPath);
                    //no need to upload an image since its contents is already in the directory being indexed, let's just add it to index
                    AddToIndex(*curImage);

                    Utils::Memory::SafeDelete(curImage);
                }
            }
            catch (const std::exception& ex) {
                //TODO: logging
                Utils::Memory::SafeDelete(curImage);
            }
        }
    }

    CanIndexDirectoryResult::Enum Indexer::CanIndexDirectory(const boost::filesystem::path& dirPath) const {
        if (!boost::filesystem::exists(dirPath)) {
            return CanIndexDirectoryResult::NotExists;
        }

        if (!boost::filesystem::is_directory(dirPath)) {
            return CanIndexDirectoryResult::IsNotDir;
        }

        if (!dirPath.is_absolute()) {
            return CanIndexDirectoryResult::NotAbsolute;
        }

        FsRepositoryInfo::col_t allRepos = this->imgMetadataStorage.GetAllRepositories();
        FsRepositoryInfo::col_t::const_iterator it;
        
        for (it = allRepos.cbegin(); it != allRepos.cend(); ++it) {
            boost::filesystem::path p = it->GetPath();

            if (dirPath == p || Utils::FileSystem::HasParentChildRelationship(p, dirPath)) {
                return CanIndexDirectoryResult::AlreadyIndexed;
            }

            if (Utils::FileSystem::HasParentChildRelationship(dirPath, p)) {
                return CanIndexDirectoryResult::SubdirIndexed;
            }
        }

        return CanIndexDirectoryResult::AvailableForIndex;
    }

    void Indexer::AddToIndex(ImgInfo& img) const {
        IFeature::col_p_t features;

        try {
            this->imgMetadataStorage.InitImgRecord(img);

            features = this->ExtractFeatures(img);
            this->featureRepo.Save(img.GetId(), features);

            Utils::Memory::SafeDeleteCollectionOfPointers(features);
        }
        catch (...) {
            Utils::Memory::SafeDeleteCollectionOfPointers(features);
            throw;
        }
    }

    IFeature::col_p_t Indexer::ExtractFeatures(const REF ImgInfo& imgInfo) const {
        IFeature::col_p_t features;

        try {
            IFeatureExtractor::col_p_t::const_iterator it;
            for (it = featureExtractors.cbegin(); it != featureExtractors.cend(); ++it) {
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

    bool Indexer::IsOfSupportedFormat(const boost::filesystem::path& filePath) const {
        std::string extension = boost::filesystem::extension(filePath);

        std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

        //TODO: to config
        return extension == ".jpg" || extension == ".jpeg" || extension == ".png" || extension == ".bmp";
    }
}
}