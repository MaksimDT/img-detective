using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

namespace img_detective.ui.Utility
{
    public class Command : ICommand
    {
        #region Fields

        private Action<object> execute;

        private Predicate<object> canExecute;

        #endregion

        #region Constructors

        public Command(Action<object> initExecute, Predicate<object> initCanExecute)
        {
            if (initExecute == null)
            {
                throw new ArgumentNullException("initExecute");
            }

            execute = initExecute;
            canExecute = initCanExecute;
        }

        public Command(Action<object> initExecute)
            : this(initExecute, null) { }

        #endregion

        #region ICommand members

        public bool CanExecute(object parameter)
        {
            return canExecute == null ? true : canExecute(parameter);
        }

        public event EventHandler CanExecuteChanged
        {
            add
            {
                CommandManager.RequerySuggested += value;
            }
            remove
            {
                CommandManager.RequerySuggested -= value;
            }
        }

        public void Execute(object parameter)
        {
            execute(parameter);
        }

        #endregion
    }
}
