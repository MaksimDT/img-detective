using img_detective.ui.model;
using img_detective.ui.services;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Diagnostics.Contracts;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

namespace img_detective.ui.VM
{
    public class MainWindowVM : DependencyObject
    {
        private readonly IImgService imgService;

        public ObservableCollection<ImageVM> Images { get; private set; }

        public MainWindowVM(IImgService imgService)
        {
            Contract.Requires(imgService != null);
            this.imgService = imgService;
            Images = new ObservableCollection<ImageVM>();
            LoadAllImages();
        }

        private void LoadAllImages()
        {
            IEnumerable<ImageFullInfo> imgs = imgService.GetImgs(0, 16);
            List<ImageVM> imgVMs = imgs.Select(im => new ImageVM(im)).ToList();

            Images.Clear();
            imgVMs.ForEach(im => Images.Add(im));
        }
    }
}
