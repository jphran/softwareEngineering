using SS;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace SpreadsheetGUI
{
    public interface ISpreadSheetView
    {
        event Action<FormClosingEventArgs> CloseEvent;

        event Action<string> SaveAs;

        event Action<string> GetCellValue;

        event Action<string> GetCellContents;

        event Action<string, string> SetCellContents;

        event Action<string> FileChosenEvent;

        event Action<string> CountEvent;

        event Action NewEvent;

        event Action HelpMenu;

        event Action OpenSpreadsheet;

        event Action OpenNewSpreadsheet;

        event Action<string> SaveSpreadsheet;

        string[] CurrentCellValue { set; }

        string FormulaEditBox { set;}

        void DoClose();

        void OpenNewFromFile(AbstractSpreadsheet ss, string pathOpenedFrom);

        void OpenNew();

        string Title { get; set; }

        string Message { set; }

        object[] InteractionMsgBox { set; }

        string PreviousSaveName { get; set; }

    }
}
