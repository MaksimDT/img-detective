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
    }
}
