using img_detective.ui.model;
using img_detective.ui.services;
using img_detective.ui.Utility;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Diagnostics.Contracts;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Input;

namespace img_detective.ui.VM
{
    public class MainWindowVM : DependencyObject
    {
        #region fields

        private readonly IImgService imgService;
        private readonly IUploadImageSelector uploadImageSelector;

        #endregion

        #region Dependency properties

        public long FirstImageNumber
        {
            get { return (long)GetValue(FirstImageNumberProperty); }
            set { SetValue(FirstImageNumberProperty, value); }
        }
        public static readonly DependencyProperty FirstImageNumberProperty =
            DependencyProperty.Register("FirstImageNumber", typeof(long), typeof(MainWindowVM), new PropertyMetadata(1L));

        public long LastImageNumber
        {
            get { return (long)GetValue(LastImageNumberProperty); }
            set { SetValue(LastImageNumberProperty, value); }
        }
        public static readonly DependencyProperty LastImageNumberProperty =
            DependencyProperty.Register("LastImageNumber", typeof(long), typeof(MainWindowVM), new PropertyMetadata(1L));

        public int PageNumber
        {
            get { return (int)GetValue(PageNumberProperty); }
            set { SetValue(PageNumberProperty, value); }
        }
        public static readonly DependencyProperty PageNumberProperty =
            DependencyProperty.Register("PageNumber", typeof(int), typeof(MainWindowVM), new PropertyMetadata(1));
        
        public long TotalImagesCount
        {
            get { return (long)GetValue(TotalImagesCountProperty); }
            set { SetValue(TotalImagesCountProperty, value); }
        }
        public static readonly DependencyProperty TotalImagesCountProperty =
            DependencyProperty.Register("TotalImagesCount", typeof(long), typeof(MainWindowVM), new PropertyMetadata(0L));

        public int PageSize
        {
            get { return (int)GetValue(PageSizeProperty); }
            set { SetValue(PageSizeProperty, value); }
        }
        public static readonly DependencyProperty PageSizeProperty =
            DependencyProperty.Register("PageSize", typeof(int), typeof(MainWindowVM), new PropertyMetadata(12));

        #endregion

        #region Properties

        public ObservableCollection<ImageVM> Images { get; private set; }

        #endregion

        #region Commands

        private ICommand _goToNextPage = null;
        public ICommand GoToNextPage
        {
            get
            {
                if (_goToNextPage == null)
                {
                    _goToNextPage = new Command(param =>
                    {
                        ++PageNumber;
                        ReloadImages();
                    });
                }

                return _goToNextPage;
            }
        }

        private ICommand _goToPrevPage = null;
        public ICommand GoToPrevPage
        {
            get
            {
                if (_goToPrevPage == null)
                {
                    _goToPrevPage = new Command(param =>
                    {
                        if (PageNumber > 1)
                        {
                            --PageNumber;
                            ReloadImages();
                        };
                    });
                }

                return _goToPrevPage;
            }
        }

        private ICommand _uploadImage = null;
        public ICommand UploadImage
        {
            get
            {
                if (_uploadImage == null)
                {
                    _uploadImage = new Command(param =>
                    {
                        string selectedFilePath = uploadImageSelector.GetFilePath();

                        if (!String.IsNullOrWhiteSpace(selectedFilePath))
                        {
                            imgService.UploadImage(selectedFilePath);
                            PageNumber = 1;
                            ReloadImages();
                        }
                    });
                }

                return _uploadImage;
            }
        }

        private ICommand _openImage = null;
        public ICommand OpenImage
        {
            get
            {
                if (_openImage == null)
                {
                    _openImage = new Command(param =>
                    {
                        ImageVM imageVm = param as ImageVM;

                        if (imageVm != null && imageVm.FileExists())
                        {
                            System.Diagnostics.Process.Start(imageVm.FilePath);
                        }

                        object p = param;
                    });
                }

                return _openImage;
            }
        }

        #endregion

        #region Ctor

        public MainWindowVM(IImgService imgService, IUploadImageSelector uploadImageSelector)
        {
            Contract.Requires(imgService != null);
            Contract.Requires(uploadImageSelector != null);
            this.imgService = imgService;
            this.uploadImageSelector = uploadImageSelector;
            Images = new ObservableCollection<ImageVM>();
            ReloadImages();
        }

        #endregion

        #region helper methods

        private void ReloadImages()
        {
            IEnumerable<ImageFullInfo> imgs = imgService.GetImgs(PageNumber, PageSize);
            List<ImageVM> imgVMs = imgs.Select(im => new ImageVM(im)).ToList();

            Images.Clear();
            imgVMs.ForEach(im => Images.Add(im));

            TotalImagesCount = imgService.GetImgsCount();
            CalculateNumbersBounds();
        }

        private void CalculateNumbersBounds()
        {
            FirstImageNumber = (PageNumber - 1) * PageSize + 1;
            long calculatedLastImageNumber = PageNumber * PageSize;

            if (calculatedLastImageNumber > TotalImagesCount)
            {
                LastImageNumber = TotalImagesCount;
            }
            else
            {
                LastImageNumber = calculatedLastImageNumber;
            }
        }

        #endregion
    }
}
