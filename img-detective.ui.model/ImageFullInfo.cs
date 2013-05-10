using System;
using System.Collections.Generic;
using System.Diagnostics.Contracts;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace img_detective.ui.model
{
    public class ImageFullInfo
    {
        private readonly image imgEntity;

        public string FilePath { get; private set; }
        public bool Exists
        {
            get
            {
                return !String.IsNullOrWhiteSpace(FilePath) && File.Exists(FilePath);
            }
        }

        public ImageFullInfo(image imgEntity)
        {
            Contract.Requires(imgEntity != null);
            this.imgEntity = imgEntity;
            ComputeFilePath();
        }

        private void ComputeFilePath()
        {
            if (imgEntity.repository != null)
            {
                string repositoryPath = imgEntity.repository.Path;
                string relFilePath = imgEntity.Path;

                if (!String.IsNullOrWhiteSpace(repositoryPath) && !String.IsNullOrWhiteSpace(relFilePath))
                {
                    FilePath = Path.Combine(
                        repositoryPath.Replace('/', '\\'),
                        relFilePath.Replace('/', '\\'));
                }
                else
                {
                    FilePath = String.Empty;
                }
            }
            else
            {
                FilePath = Path.Combine(
                    @"H:\Институт\Диплом\img-detective\upload",
                    imgEntity.Id + "." + imgEntity.Extension);
            }
        }
    }
}
