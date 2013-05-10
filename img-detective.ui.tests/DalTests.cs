using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using img_detective.ui.dal;
using System.Collections.Generic;
using System.Linq;

namespace img_detective.ui.tests
{
    [TestClass]
    public class DalTests
    {
        [TestMethod]
        public void ImgDal_GetAllImgs()
        {
            const int pageSize = 10;

            ImgDAL dal = new ImgDAL();

            IEnumerable<model.image> images = dal.GetImgs(0, pageSize);

            Assert.IsNotNull(images);
            Assert.IsTrue(images.Count() <= pageSize);
        }
    }
}
