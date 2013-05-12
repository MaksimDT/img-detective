using img_detective.cswrapper;
using img_detective.ui.dal;
using img_detective.ui.model;
using System;
using System.Collections.Generic;
using System.Diagnostics.Contracts;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace img_detective.ui.services
{
    public class ImgService : IImgService
    {
        private readonly IImgDAL dal;

        public ImgService(IImgDAL dal)
        {
            Contract.Assert(dal != null);
            this.dal = dal;
        }

        public IEnumerable<ImageFullInfo> GetImgs(int pageNumber, int pageSize)
        {
            IEnumerable<image> images = dal.GetImgs(pageNumber, pageSize);
            return images.Select(img => new ImageFullInfo(img)).ToList();
        }

        public IEnumerable<ImageFullInfo> GetImgs(SearchResult searchResult, int pageNumber, int pageSize)
        {
            IEnumerable<image> allImages = dal.GetImgs();

            List<ImageFullInfo> result = new List<ImageFullInfo>();
            result.Capacity = allImages.Count();

            foreach (SearchResult.Node resultNode in searchResult.Nodes)
            {
                image im = allImages.FirstOrDefault(i => i.Id == resultNode.ImageId);

                if (im != null)
                {
                    result.Add(new ImageFullInfo(im, resultNode.Relevance));
                }
            }

            int toSkip = 0;
            if (pageNumber > 1)
            {
                toSkip = (pageNumber - 1) * pageSize;
            }
            return result.OrderByDescending(im => im.Relevance.Value).Skip(toSkip).Take(pageSize).ToList();
        }

        public long GetImgsCount()
        {
            return dal.GetImgsCount();
        }

        public void UploadImage(string filePath)
        {
            byte[] fileContent = File.ReadAllBytes(filePath);
            string fileExtension = Path.GetExtension(filePath).ToLowerInvariant();

            bool success = ImgDetectiveLib.UploadImg(fileContent, fileExtension);
        }

        public bool CanIndexDirectory(string directoryPath, out string message)
        {
            message = null;
            var result = ImgDetectiveLib.CanIndexDirectory(directoryPath);

            switch (result)
            {
                case ImgDetectiveLib.CanIndexDirectoryResult.ResultEnum.AvailableForIndex:
                    return true;
                case ImgDetectiveLib.CanIndexDirectoryResult.ResultEnum.Unknown:
                case ImgDetectiveLib.CanIndexDirectoryResult.ResultEnum.AlreadyIndexed:
                case ImgDetectiveLib.CanIndexDirectoryResult.ResultEnum.NotAbsolute:
                case ImgDetectiveLib.CanIndexDirectoryResult.ResultEnum.NotExists:
                case ImgDetectiveLib.CanIndexDirectoryResult.ResultEnum.SubdirIndexed:
                case ImgDetectiveLib.CanIndexDirectoryResult.ResultEnum.IsNotDir:
                    return false;
                default:
                    throw new InvalidOperationException("Unknown value of the enum CanIndexDirectoryResult.ResultEnum: " + result.ToString());
            }
        }

        public void IndexDirectory(string directoryPath)
        {
            ImgDetectiveLib.IndexDirectory(directoryPath);
        }

        public SearchResult SearchByExample(string filePath)
        {
            byte[] exampleImgContent = File.ReadAllBytes(filePath);

            return ImgDetectiveLib.Search(exampleImgContent);
        }
    }
}
