using Formulas;
using SS;
using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Text.RegularExpressions;
using Microsoft.VisualBasic;
using System.Windows.Forms;

namespace SpreadsheetGUI
{
    public class Controller
    {
        private ISpreadSheetView view;
        private AbstractSpreadsheet model;

        /// <summary>
        /// Single param constructor creates a new controller for passed ISpreadSheetView
        /// </summary>
        /// <param name="_view"></param>
        public Controller(ISpreadSheetView _view)
        {
            this.view = _view;
            var re = new Regex("^[A-Z][1-9][0-9]?$"); //creates new regex limited to (A1-Z99) inclusive
            this.model = new Spreadsheet(re); //creates new spreadhseet model with designated regex

            //register all events to handlers
            view.SetCellContents += HandleSetCellContents;
            view.GetCellValue += HandleGetCellValue;
            view.GetCellContents += HandleGetCellContents;
            view.HelpMenu += HandleHelpMenu;
            view.SaveSpreadsheet += HandleSaveSpreadsheet;
            view.SaveAs += HandleSaveAs;
            view.OpenSpreadsheet += HandleOpenSpreadsheet;
            view.OpenNewSpreadsheet += HandleOpenNewSpreadsheet;
            view.CloseEvent += HandleCloseEvent;
            // Dont forget to update the 2 argument constructor

        }

        /// <summary>
        /// Multiple param constructor creates a new controller for a view opened from a file, handles hooking
        /// and populates the new spreadsheet gui with saved contents
        /// </summary>
        /// <param name="_view"></param>
        /// <param name="ss"></param>
        /// <param name="pathOpenedFrom"></param>
        public Controller(ISpreadSheetView _view, AbstractSpreadsheet ss, string pathOpenedFrom)
        {
            this.view = _view;
            this.model = ss;

            //register all events
            view.SetCellContents += HandleSetCellContents;
            view.GetCellValue += HandleGetCellValue;
            view.GetCellContents += HandleGetCellContents;
            view.HelpMenu += HandleHelpMenu;
            view.OpenSpreadsheet += HandleOpenSpreadsheet;
            view.OpenNewSpreadsheet += HandleOpenNewSpreadsheet;
            view.SaveSpreadsheet += HandleSaveSpreadsheet;
            view.SaveAs += HandleSaveAs;
            view.CloseEvent += HandleCloseEvent;
            // Dont forget to update the 1 argument constructor

            // populate all cells
            foreach (string cellName in this.model.GetNamesOfAllNonemptyCells())
            {
                this.HandleUpdateCellDisplay(cellName);
            }

        }


        /// <summary>
        /// Hooks CloseEvent<FormClosingEventArgs></FormClosingEventArgs>
        /// Checks if model has been modified. If true, displays warning and follows user prompt, else closes view
        /// </summary>
        /// <param name="e"></param>
        private void HandleCloseEvent(FormClosingEventArgs e)
        {
            if (this.model.Changed)
            {
                this.view.InteractionMsgBox = new object[4] { "There are unsaved changes. Exit without saving?", MsgBoxStyle.YesNo, "Unsaved Data Warning", e };
            }
        }

        /// <summary>
        /// Hooks SaveAsEvent<string></string>
        /// Appends given save name with appropriate file type markers and calls HandleSaveSpreadsheet()
        /// </summary>
        /// <param name="desName"></param>
        private void HandleSaveAs(string desName)
        {
            //if name is missing proper annotation
            if (!desName.Substring(desName.Length - 3).Equals(".ss"))
            {
                desName = desName + ".ss"; //append
            }

            HandleSaveSpreadsheet(desName); //save
        }

        /// <summary>
        /// Hooks SaveSpreadsheet<string></string>
        /// Checks if the name already exists in pwd and displays override warning if it does. Follows user prompt
        /// to proceed. If given the ok, proceeds to save file in xml. Else cancels.
        /// </summary>
        /// <param name="desName"></param>
        private void HandleSaveSpreadsheet(string desName)
        {
            //check if file name exists in pwd and makes sure that if it does exist, its not the same file being resaved
            if (File.Exists(desName) && !this.view.PreviousSaveName.Equals(desName))
            {
                //invoke view to display message
                this.view.InteractionMsgBox = new object[4] { "A file with that name already exist. Do you wish to override that file? (The old file will no longer exist)", MsgBoxStyle.YesNo, "Override Warning", desName };
            }

            StringWriter sw = new StringWriter(); //object to write ss to xml
            this.model.Save(sw); //write xml
            //save file
            using (FileStream fs = File.Create(desName))
            {
                byte[] xmlData = new UTF8Encoding(true).GetBytes(sw.ToString());
                fs.Write(xmlData, 0, xmlData.Length);
            }

            view.Title = desName; //update view text 
            this.view.PreviousSaveName = this.view.Title; //update override verification
        }

        /// <summary>
        /// Hooks OpenNewSpreadsheet event
        /// </summary>
        private void HandleOpenNewSpreadsheet()
        {
            SpreadSheetApplicationContext.GetContext().RunNew(); //opens new form from context
        }

        /// <summary>
        /// Hooks OpenSpreadsheet event
        /// Allows user to select a file from file dialog and opens it in a new spreadsheet. Pop ups displayed if 
        /// exceptions are thrown
        /// </summary>
        private void HandleOpenSpreadsheet()
        {
            //create pop up file explorer
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.DefaultExt = "ss";
            ofd.Filter = "Spreadsheet file (*.ss)|*.ss|All Files (*.*)|*.*"; //create optional filters for .ss or all files

            //if they chose a file open, else cancel openspreadsheet event
            if (ofd.ShowDialog() == DialogResult.OK)
            {
                try
                {
                    string filePath = ofd.FileName;
                    string fileText = File.ReadAllText(filePath); //copy text from file
                    AbstractSpreadsheet ss = new Spreadsheet(new StringReader(fileText), new Regex("^[A-Z][1-9][0-9]?$")); //create ss from designated file
                    string filenName = Path.GetFileName(filePath); //get name for window title
                    SpreadSheetApplicationContext.GetContext().RunNewFromFile(ss, filenName); //create new window from context
                    this.view.OpenNewFromFile(ss, filenName); //for testing ONLY
                }
                catch (Exception e) //if something goes wrong disp err msg
                {
                    string openError = "Error opening file.\n" + e.Message;
                    System.Windows.Forms.MessageBox.Show(openError);
                }
            }
        }


        /// <summary>
        /// Hooks GetCellContents<string></string> event
        /// Pulls desired contents from model, passes it to view
        /// </summary>
        /// <param name="name"></param>
        private void HandleGetCellContents(string name)
        {
            object contents = model.GetCellContents(name); //pull the contents

            if (contents is Formula) //if form, append "=" for proper syntax
            {
                view.FormulaEditBox = "=" + contents.ToString();
            }
            else
            {
                view.FormulaEditBox = contents.ToString();
            }
        }

        /// <summary>
        /// Hooks SetCellContents<string ,string></string> event
        /// Updates the model with the cell name and contents and passes on the contents to the view
        /// </summary>
        /// <param name="name"></param>
        /// <param name="contents"></param>
        private void HandleSetCellContents(string name, string contents)
        {
            string[] args; //return string[]
            ISet<string> cellsToUpdate;

            try
            {
                cellsToUpdate = this.model.SetContentsOfCell(name, contents); //set contents and get list of cels to update
            }
            catch (CircularException)
            {
                args = new string[3] { name.Substring(0, 1), name.Substring(1), "Formula Error" }; //return cell name and contents
                view.CurrentCellValue = args;

                //disp err message
                this.view.Message = "You just created a circular dependancy. Please revise.";

                return;
            }

            object value = this.model.GetCellValue(name); //pull value from model

            string[] valueAndErrorMessage = this.valueToString(value); //change value object to value string and error message if approp

            string valueString = valueAndErrorMessage[0]; //value is first item in arr
            string errMsg = valueAndErrorMessage[1]; //second item
            //if there is an error message, disp on view
            if (errMsg != null)
            {
                this.view.Message = errMsg;
            }

            args = new string[3] { name.Substring(0, 1), name.Substring(1), valueString }; //else return approp string[]

            view.CurrentCellValue = args;

            // After setting contents update all dependees
            foreach (string cellName in cellsToUpdate)
            {
                this.HandleUpdateCellDisplay(cellName);
            }
        }

        /// <summary>
        /// Given a cell value, creates a string representation. 
        /// This method determines if a cell's value is a string, double, or formula error
        /// </summary>
        /// <param name="contents"></param>
        /// <returns></returns>
        private string[] valueToString(object value)
        {
            if (value is FormulaError)
            {
                FormulaError err = (FormulaError)value;
                string reason = err.Reason;
                string msg;

                if (reason.Contains("InvalidCastException"))
                {
                    msg = "You just refrenced a string or empty cell. Please revise.";
                }
                else
                {
                    msg = "Unknown reference.";
                }
                return new string[2] { "Formula Err", msg };
            }
            else
            {
                return new string[2] { value.ToString(), null };
            }
        }

        /// <summary>
        /// Updates the displayed valued of the nammed cell. Used to update dependancies when a dependee changes.
        /// </summary>
        /// <param name="name"></param>
        private void HandleUpdateCellDisplay(string name)
        {
            object value = this.model.GetCellValue(name);
            string[] args;
            string valueString = valueToString(value)[0];
            args = new string[3] { name.Substring(0, 1), name.Substring(1), valueString };
            view.CurrentCellValue = args;
        }

        /// <summary>
        /// Returns the input cell value
        /// </summary>
        /// <param name="name"></param>
        private void HandleGetCellValue(string name)
        {
            object value = model.GetCellValue(name);

            string valueString = valueToString(value)[0];
            view.FormulaEditBox = valueString;

        }

        /// <summary>
        /// Creates a pop up pox containing information on how to use the spreadsheet.
        /// </summary>
        private void HandleHelpMenu()
        {
            String helpMessage = "How to use the spreadsheet:\n\n" +
            "Cell values are changed by selecting the cell and typing a Formula/Number/String in the formula editor box, then pressing enter.\n" +
            "The formula editor is the textbox above the cells. You must press enter while in the formula edit box to confirm the cell contents.\n" +
            "Once enter has been pressed, you may navigate between cells using the arrowkeys. While using the arrow keys to navigate, pressing enter\n" +
            "or clicking on the formula editor box will transition the focus back to the formula editor box. While using the arrowkeys,\n" +
            "pressing the equals key ('=') will transition the focus to the formual editor and automatically insert an equals sign ('=') for\n" +
            "quickly entering in a formula. Additionally you may click on a cell to navigate to it. Clicking on a cell will automatically shift\n" +
            "the focus to the formula editor box.\n" +
            "Clicking File > Save (as) will allow you to save the spreadsheet. Clicking File > Open will open a saved spreadsheet.\n" +
            "Clicking File > New will open a blank spreadsheet in a new window";

            System.Windows.Forms.MessageBox.Show(helpMessage);
        }

    }
}
