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
    }
}
