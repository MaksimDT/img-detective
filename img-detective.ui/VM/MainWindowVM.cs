using img_detective.ui.model;
using img_detective.ui.services;
using img_detective.ui.Utility;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Diagnostics.Contracts;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;

namespace img_detective.ui.VM
{
    public class MainWindowVM : DependencyObject
    {
        #region fields

        private readonly IImgService imgService;
        private readonly IImageFileSelector imageFileSelector;
        private readonly IDirectorySelector directorySelector;
        private readonly IAlertPresenter alertPresenter;
        private SearchResult currentSearchResult;

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
            DependencyProperty.Register("PageSize", typeof(int), typeof(MainWindowVM), new PropertyMetadata(12, OnPageSizeChanged));
        private static void OnPageSizeChanged(DependencyObject d, DependencyPropertyChangedEventArgs args)
        {
            MainWindowVM vm = d as MainWindowVM;
            if (vm != null)
            {
                vm.PageNumber = 1;
                vm.ReloadImages();
            }
        }

        public ImageSource SearchExample
        {
            get { return (ImageSource)GetValue(SearchExampleProperty); }
            set { SetValue(SearchExampleProperty, value); }
        }
        public static readonly DependencyProperty SearchExampleProperty =
            DependencyProperty.Register("SearchExample", typeof(ImageSource), typeof(MainWindowVM), new PropertyMetadata(null));

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
                        string selectedFilePath = imageFileSelector.GetFilePath();

                        if (!String.IsNullOrWhiteSpace(selectedFilePath))
                        {
                            imgService.UploadImage(selectedFilePath);
                            ResetToInitialState();
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

        private ICommand _indexDirectory = null;
        public ICommand IndexDirectory
        {
            get
            {
                if (_indexDirectory == null)
                {
                    _indexDirectory = new Command(param =>
                    {
                        string selectedDirPath = directorySelector.GetDirectoryPath();
                        if (String.IsNullOrWhiteSpace(selectedDirPath))
                        {
                            return;
                        }

                        string checkMessage;
                        bool canIndex = imgService.CanIndexDirectory(selectedDirPath, out checkMessage);

                        if (canIndex)
                        {
                            imgService.IndexDirectory(selectedDirPath);
                            ResetToInitialState();
                        }
                        else
                        {
                            if (!String.IsNullOrWhiteSpace(checkMessage))
                            {
                                alertPresenter.ShowAlert(checkMessage);
                            }
                            else
                            {
                                alertPresenter.ShowAlert("Индексация выбранной директории невозможна");
                            }
                        }
                    });
                }

                return _indexDirectory;
            }
        }

        private ICommand _search = null;
        public ICommand Search
        {
            get
            {
                if (_search == null)
                {
                    _search = new Command(param =>
                    {
                        string selectedFilePath = imageFileSelector.GetFilePath();
                        if (String.IsNullOrWhiteSpace(selectedFilePath))
                        {
                            return;
                        }

                        try {
                            var exampleImgSource = new BitmapImage();
                            exampleImgSource.BeginInit();
                            exampleImgSource.UriSource = new Uri("file://" + selectedFilePath);
                            exampleImgSource.EndInit();

                            SearchExample = exampleImgSource;

                            currentSearchResult = imgService.SearchByExample(selectedFilePath);
                            PageNumber = 1;
                            ReloadImages();
                        }
                        catch (Exception ex) {
                            alertPresenter.ShowAlert("Не удалось произвести поиск (неизвестная ошибка)");
                        }
                    });
                }

                return _search;
            }
        }

        #endregion

        #region Ctor

        public MainWindowVM(IImgService imgService, IImageFileSelector imageFileSelector, IDirectorySelector directorySelector, IAlertPresenter alertPresenter)
        {
            Contract.Requires(imgService != null);
            Contract.Requires(imageFileSelector != null);
            Contract.Requires(directorySelector != null);
            Contract.Requires(alertPresenter != null);

            this.imgService = imgService;
            this.imageFileSelector = imageFileSelector;
            this.directorySelector = directorySelector;
            this.alertPresenter = alertPresenter;

            Images = new ObservableCollection<ImageVM>();
            ReloadImages();
        }

        #endregion

        #region helper methods

        private void ReloadImages()
        {
            IEnumerable<ImageFullInfo> imgs = null;

            if (currentSearchResult != null)
            {
                imgs = imgService.GetImgs(currentSearchResult, PageNumber, PageSize);
            }
            else
            {
                imgs = imgService.GetImgs(PageNumber, PageSize);
            }

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

        private void ResetToInitialState()
        {
            currentSearchResult = null;
            PageNumber = 1;
            SearchExample = null;
            ReloadImages();
        }

        #endregion
    }
}
