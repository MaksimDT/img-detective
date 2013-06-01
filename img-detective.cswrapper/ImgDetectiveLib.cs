using img_detective.ui.model;
using System;
using System.Collections.Generic;
using System.Diagnostics.Contracts;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace img_detective.cswrapper
{
    public static class ImgDetectiveLib
    {
        public static bool UploadImg(byte[] imgContent, string fileExtension)
        {
            Contract.Assert(imgContent != null);
            Contract.Assert(imgContent.Length > 0);
            Contract.Assert(!String.IsNullOrWhiteSpace(fileExtension));

            IntPtr unmanagedBuffer = IntPtr.Zero;

            try
            {
                unmanagedBuffer = Marshal.AllocCoTaskMem(imgContent.Length);
                Marshal.Copy(imgContent, 0, unmanagedBuffer, imgContent.Length);
                var rawImg = new structs.RawImg()
                {
                    content = unmanagedBuffer,
                    contentSize = (UInt32)imgContent.Length,
                    fileExtension = fileExtension
                };
                var result = new structs.UploadImgResult();
                UploadImg(rawImg, out result);

                return result.opStatus == 0;
            }
            finally
            {
                if (unmanagedBuffer != IntPtr.Zero)
                {
                    Marshal.FreeCoTaskMem(unmanagedBuffer);
                }
            }
        }

        public static structs.CanIndexDirectoryResult.ResultEnum CanIndexDirectory(string dirPath)
        {
            structs.CanIndexDirectoryResult result = new structs.CanIndexDirectoryResult();
            CanIndexDirectory(dirPath, out result);

            if (result.opStatus != 0 || result.resultCode == structs.CanIndexDirectoryResult.ResultEnum.Unknown)
            {
                throw new Exception("Couldn't check the specified directory. Call to the umanaged DLL failed");
            }

            return result.resultCode;
        }

        public static void IndexDirectory(string dirPath)
        {
            structs.IndexDirectoryResult result = new structs.IndexDirectoryResult();
            IndexDirectory(dirPath, out result);

            if (result.opStatus != 0)
            {
                throw new Exception("Couldn't index the specified directory. Call to the unmanaged DLL failed");
            }
        }

        public static ui.model.SearchResult Search(byte[] imgContent)
        {
            Contract.Requires(imgContent != null);
            Contract.Requires(imgContent.Length != 0);

            IntPtr imgContentUnmanagedBuf = IntPtr.Zero;
            var result = new structs.SearchResult()
            {
                items = IntPtr.Zero,
                itemsPositions = IntPtr.Zero
            };

            try
            {
                imgContentUnmanagedBuf = Marshal.AllocCoTaskMem(imgContent.Length);
                Marshal.Copy(imgContent, 0, imgContentUnmanagedBuf, imgContent.Length);

                var query = new structs.ImgQuery()
                {
                    exampleContent = imgContentUnmanagedBuf,
                    exampleContentSize = (UInt32)imgContent.Length,
                    tolerance = 0.5
                };

                Search(query, out result);

                if (result.opStatus != 0)
                {
                    throw new Exception("Call to the unmanaged DLL failed. Search cannot be performed");
                }

                if (result.items == IntPtr.Zero || result.itemsPositions == IntPtr.Zero || result.arraySize == 0)
                {
                    //no images found
                    return new ui.model.SearchResult();
                }

                long[] imgIds = new long[result.arraySize];
                double[] positions = new double[result.arraySize];
                Marshal.Copy(result.items, imgIds, 0, (int)result.arraySize);
                Marshal.Copy(result.itemsPositions, positions, 0, (int)result.arraySize);

                return new ui.model.SearchResult(imgIds, positions);
            }
            finally
            {
                if (imgContentUnmanagedBuf != IntPtr.Zero)
                {
                    Marshal.FreeCoTaskMem(imgContentUnmanagedBuf);
                }

                if (result.items != IntPtr.Zero)
                {
                    Marshal.FreeCoTaskMem(result.items);
                }

                if (result.itemsPositions != IntPtr.Zero)
                {
                    Marshal.FreeCoTaskMem(result.itemsPositions);
                }
            }
        }

        

        [DllImport("img-detective.facade.dll", CallingConvention=CallingConvention.Cdecl)]
        private static extern void UploadImg(structs.RawImg img, out structs.UploadImgResult result);

        [DllImport("img-detective.facade.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void CanIndexDirectory([MarshalAs(UnmanagedType.LPWStr)]string dirPath, out structs.CanIndexDirectoryResult result);

        [DllImport("img-detective.facade.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void IndexDirectory([MarshalAs(UnmanagedType.LPWStr)]string dirPath, out structs.IndexDirectoryResult result);

        [DllImport("img-detective.facade.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void Search(structs.ImgQuery query, out structs.SearchResult result);

        [DllImport("img-detective.facade.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void Search1(structs.ImgQuery query);
    }
}
