using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace img_detective.cswrapper.structs
{
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

    [StructLayout(LayoutKind.Sequential)]
    public struct CanIndexDirectoryResult
    {
        public enum ResultEnum
        {
            Unknown = 0,
            AvailableForIndex = 1,
            AlreadyIndexed = 2,
            NotAbsolute = 3,
            NotExists = 4,
            SubdirIndexed = 5,
            IsNotDir = 6
        }

        public ResultEnum resultCode;
        public Int32 opStatus;
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct IndexDirectoryResult
    {
        public Int32 opStatus;
    }

    [StructLayout(LayoutKind.Sequential)]
    struct ImgQuery
    {
        public UInt32 exampleContentSize;
        public IntPtr exampleContent;
        public double tolerance;
    }

    [StructLayout(LayoutKind.Sequential)]
    struct SearchResult
    {
        public Int32 opStatus;
        public UInt32 arraySize;
        public IntPtr items;
        public IntPtr itemsPositions;
    }
}
