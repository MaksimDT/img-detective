#pragma once

#include "common/CommonInternal.h"
#include <boost/filesystem.hpp>
#include <string>
#pragma warning(push, 0)
#include <Magick++.h>
#pragma warning(pop)

namespace ImgDetective {
namespace Core {

	//internal representation of image
	CONCRETE SEALED class ImgInfo : NONCOPYABLE {
	public:
		static ImgInfo* Create(const REF RawImg& shortInfo);
        static ImgInfo* Create(repoid_t repoId, const boost::filesystem::path& filePath, const boost::filesystem::path& relativeFilePath);

        Magick::Image& GetMagickImage();
        const Magick::Image& GetMagickImage() const;
        imgid_t GetId() const;
        void SetId(imgid_t imgId);
        boost::filesystem::path GetPath() const;
        void SetPath(const boost::filesystem::path& path);
        repoid_t GetRepoId() const;
        void SetRepoId(repoid_t repoId);

        std::string GetFileExtension() const;
	private:
		CTOR ImgInfo(const REF Magick::Image& magickImg, const boost::filesystem::path& filePath, repoid_t repoId);
        void SetFileExtension(const std::string& ext);
		
		EXCLUSIVE Magick::Image magickImg;
        imgid_t imgId;

        boost::filesystem::path filePath;
        repoid_t repoId;

        //in case if we don't have the full filePath, we will use this extension to save the image in the filesystem.
        //starts with a dot symbol
        std::string fileExtension;
	};

}
}