using img_detective.cswrapper;
using img_detective.ui.model;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace img_detective.ui.services
{
    public interface IImgService
    {
        IEnumerable<ImageFullInfo> GetImgs(int pageNumber, int pageSize);
        IEnumerable<ImageFullInfo> GetImgs(SearchResult searchResult, int pageNumber, int pageSize);
        long GetImgsCount();
        void UploadImage(string filePath);
        bool CanIndexDirectory(string directoryPath, out string message);
        void IndexDirectory(string directoryPath);
        SearchResult SearchByExample(string filePath);
    }
}
