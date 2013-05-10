using img_detective.ui.model;
using System;
using System.Collections.Generic;
using System.Diagnostics.Contracts;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media;
using System.Windows.Media.Imaging;

namespace img_detective.ui.VM
{
    public class ImageVM
    {
        private readonly ImageFullInfo model;
        public ImageSource Source { get; private set; }

        public ImageVM(ImageFullInfo model)
        {
            Contract.Requires(model != null);
            this.model = model;
            InitImageSource();
        }

        private void InitImageSource()
        {
            if (!String.IsNullOrWhiteSpace(model.FilePath) && File.Exists(model.FilePath))
            {
                BitmapImage src = new BitmapImage();
                src.BeginInit();
                src.UriSource = new Uri("file://" + model.FilePath);
                src.EndInit();
                Source = src;
            }
            else
            {
                AssignNotFoundIconToSource();
            }
        }

        private void AssignNotFoundIconToSource()
        {
            object imageNotFoundResource = System.Windows.Application.Current.TryFindResource("ImageNotFound");

            if (imageNotFoundResource != null && imageNotFoundResource is ImageSource)
            {
                Source = imageNotFoundResource as ImageSource;
            }
        }
    }
}
