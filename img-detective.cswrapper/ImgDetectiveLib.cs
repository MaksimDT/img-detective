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
                var rawImg = new RawImg()
                {
                    content = unmanagedBuffer,
                    contentSize = (UInt32)imgContent.Length,
                    fileExtension = fileExtension
                };
                var result = new UploadImgResult();
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

        [StructLayout(LayoutKind.Sequential)]
        public struct UploadImgResult
        {
            public Int32 opStatus;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct RawImg
        {
            public UInt32 contentSize;
            public IntPtr content;
            [MarshalAs(UnmanagedType.LPStr)]
            public string fileExtension;
        }

        [DllImport("img-detective.facade.dll")]
        private static extern void UploadImg(RawImg img, out UploadImgResult result);
    }
}
