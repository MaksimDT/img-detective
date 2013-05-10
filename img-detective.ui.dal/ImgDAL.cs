using img_detective.ui.model;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace img_detective.ui.dal
{
    public class ImgDAL : IImgDAL
    {
        public IEnumerable<image> GetImgs(int pageNum, int pageSize)
        {
            using (var ctx = new ImgMetadataDbCon())
            {
                int toSkip = 0;
                if (pageNum > 1)
                {
                    toSkip = (pageNum - 1) * pageSize;
                }
                return ctx.images.Include("repository").OrderByDescending(im => im.Id).Skip(toSkip).Take(pageSize).ToList();
            }
        }
    }
}
