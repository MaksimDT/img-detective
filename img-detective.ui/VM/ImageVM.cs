using img_detective.ui.model;
using System;
using System.Collections.Generic;
using System.Diagnostics.Contracts;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Media;
using System.Windows.Media.Imaging;

namespace img_detective.ui.VM
{
    public class ImageVM : DependencyObject
    {
        #region fields

        private readonly ImageFullInfo model;

        #endregion

        #region Properties

        public ImageSource Source { get; private set; }
        public string FilePath
        {
            get
            {
                return model.FilePath;
            }
        }
        public double Relevance
        {
            get
            {
                return model.Relevance ?? 0.0;
            }
        }

        #endregion

        #region dependency properties

        public double Width
        {
            get { return (double)GetValue(WidthProperty); }
            set { SetValue(WidthProperty, value); }
        }
        public static readonly DependencyProperty WidthProperty =
            DependencyProperty.Register("Width", typeof(double), typeof(ImageVM), new PropertyMetadata(0.0, OnWidthChanged));
        private static void OnWidthChanged(DependencyObject d, DependencyPropertyChangedEventArgs args)
        {
            ImageVM vm = d as ImageVM;
            if (vm != null && args.NewValue is double)
            {
                vm.DoubledWidth = (double)(args.NewValue) * 2;
            }
        }

        public double Height
        {
            get { return (double)GetValue(HeightProperty); }
            set { SetValue(HeightProperty, value); }
        }
        public static readonly DependencyProperty HeightProperty =
            DependencyProperty.Register("Height", typeof(double), typeof(ImageVM), new PropertyMetadata(0.0, OnHeightChanged));
        private static void OnHeightChanged(DependencyObject d, DependencyPropertyChangedEventArgs args)
        {
            ImageVM vm = d as ImageVM;
            if (vm != null && args.NewValue is double)
            {
                vm.DoubledHeight = (double)(args.NewValue) * 2;
            }
        }

        public double DoubledWidth
        {
            get { return (double)GetValue(DoubledWidthProperty); }
            set { SetValue(DoubledWidthProperty, value); }
        }
        public static readonly DependencyProperty DoubledWidthProperty =
            DependencyProperty.Register("DoubledWidth", typeof(double), typeof(ImageVM), new PropertyMetadata(0.0));

        public double DoubledHeight
        {
            get { return (double)GetValue(DoubledHeightProperty); }
            set { SetValue(DoubledHeightProperty, value); }
        }

        public static readonly DependencyProperty DoubledHeightProperty =
            DependencyProperty.Register("DoubledHeight", typeof(double), typeof(ImageVM), new PropertyMetadata(0.0));

        #endregion

        #region ctor

        public ImageVM(ImageFullInfo model)
        {
            Contract.Requires(model != null);
            this.model = model;
            InitImageSource();
        }

        #endregion

        #region public methods

        public bool FileExists()
        {
            return !String.IsNullOrWhiteSpace(model.FilePath) && File.Exists(model.FilePath);
        }

        #endregion

        #region helper methods

        private void InitImageSource()
        {
            if (FileExists())
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

        #endregion
    }
}
