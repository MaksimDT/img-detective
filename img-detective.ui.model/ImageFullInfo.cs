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
        public double? Position { get; private set; }

        public ImageFullInfo(image imgEntity)
        {
            Contract.Requires(imgEntity != null);
            this.imgEntity = imgEntity;
            ComputeFilePath();
        }

        public ImageFullInfo(image imgEntity, double position)
            : this(imgEntity)
        {
            this.Position = position;
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
                //TODO: path to config!!!
                FilePath = Path.Combine(
                    @"H:\Институт\Диплом\img-detective\upload",
                    imgEntity.Id + "." + imgEntity.Extension);
            }
        }
    }
}
