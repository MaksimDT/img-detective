using System;
using System.Collections.Generic;
using System.Diagnostics.Contracts;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace img_detective.ui.Utility
{
    public class WpfDirectorySelector : IDirectorySelector
    {
        private readonly System.Windows.Window hostWpfWindow;

        public WpfDirectorySelector(System.Windows.Window hostWpfWindow)
        {
            Contract.Requires(hostWpfWindow != null);
            this.hostWpfWindow = hostWpfWindow;
        }

        public string GetDirectoryPath()
        {
            var dlg = new FolderBrowserDialog();
            System.Windows.Forms.DialogResult result = dlg.ShowDialog(hostWpfWindow.GetIWin32Window());

            if (result == DialogResult.OK)
            {
                return dlg.SelectedPath;
            }
            else
            {
                return String.Empty;
            }
        }
    }
}
