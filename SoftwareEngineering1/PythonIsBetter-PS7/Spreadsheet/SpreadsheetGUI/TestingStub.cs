using SS;
using System;
using System.Collections.Generic;
using System.Windows.Forms;


namespace SpreadsheetGUI
{
    /// <summary>
    /// Stub class used to help with testing MVC style
    /// </summary>
    public class TestingStub : ISpreadSheetView
    {
        //private Dictionary<string, string> table;
        private Spreadsheet ss;
        private string currentCellValue;
        private string formulaEditBox;
        private string title = "untitledSpreadsheet1.ss";
        private string message;
        private bool isClosed = false;
        private bool isMessage = false;
        private bool didOpenNew = false;
        private string previousSaveName = "untitledSpreadsheet1.ss";
        private string pathOpenedFrom;

        public bool DidOpenNew { get => didOpenNew; set => didOpenNew = value; }

        public string PathOpenedFrom { get; set; }

        public string[] CurrentCellValue
        {
            set
            {
                ss.SetContentsOfCell(value[0] + value[1], value[2]);
            }
        }
        public string FormulaEditBox { get => formulaEditBox;  set => formulaEditBox = value; }
        public string Title { get => title; set => title = value; }
        public string Message { set => message = value; }
        public object[] InteractionMsgBox { get => new object[1] { isMessage}; set => isMessage = true; }
        public string PreviousSaveName { get => previousSaveName; set => previousSaveName = value; }

        public event Action<string> GetCellValue;
        public event Action<string> GetCellContents;
        public event Action<string, string> SetCellContents;
        public event Action<string> FileChosenEvent;
        public event Action<string> CountEvent;
        public event Action<FormClosingEventArgs> CloseEvent;
        public event Action NewEvent;
        public event Action HelpMenu;
        public event Action<string> SaveAs;
        public event Action OpenSpreadsheet;
        public event Action OpenNewSpreadsheet;
        public event Action<string> SaveSpreadsheet;

        public void DoClose()
        {
            isClosed = true;
        }

        public void OpenNew()
        {
            didOpenNew = true;
        }

        public TestingStub()
        {
           // this.table = new Dictionary<string, string>();
            this.ss = new Spreadsheet();
        }


        public void FireHelpMenu()
        {
            HelpMenu();
        }

        public void FireOpenSpreadsheetEvent()
        {
            OpenSpreadsheet();
        }

        public void FireGetCellValue(string cell)
        {
            GetCellValue(cell);
        }

        public void FireGetCellContentsEvent(string cell)
        {
            GetCellContents(cell);
        }

        public void FireOpenNewEvent()
        {
            OpenNewSpreadsheet();
        }

        public void FireSaveSpreadsheetEvent()
        {
            SaveSpreadsheet(this.Title);
        }


        public void FireSaveAsEvent(string fileName)
        {
            SaveAs(fileName);
        }
        
        public void FireCloseEvent()
        {
            FormClosingEventArgs e = new FormClosingEventArgs(CloseReason.UserClosing, false);
            CloseEvent(e);
        }

        public void FireSetCellContents(string name, string contents)
        {
            SetCellContents(name, contents);
        }

        public bool tableContains(string name, string contents)
        {
            if(ss.GetCellContents(name).Equals(contents))
            {
                return true;
            }
            return false;
            
        }

        public bool ssContains(string name, string contents)
        {
            if (contentsToString(ss.GetCellValue(name)).Equals(contents))
            {
                return true;
            }
            return false;
        }

        private string contentsToString(object contents)
        {
            if(contents is FormulaError)
            {
                return "Formula Err";
            }
            else
            {
                return contents.ToString();
            }
        }

        public void OpenNewFromFile(AbstractSpreadsheet ss, string pathOpenedFrom)
        {
            PathOpenedFrom = pathOpenedFrom;
        }
    }
}
