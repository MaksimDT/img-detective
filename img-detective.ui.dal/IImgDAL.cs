using img_detective.ui.model;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace img_detective.ui.dal
{
    public interface IImgDAL
    {
        IEnumerable<image> GetImgs(int pageNum, int pageSize);
    }
}
