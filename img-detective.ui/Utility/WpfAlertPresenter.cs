using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace img_detective.ui.Utility
{
    public class WpfAlertPresenter : IAlertPresenter
    {
        public void ShowAlert(string message)
        {
            MessageBox.Show(message);
        }
    }
}
