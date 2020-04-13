using SS;
using System;
using System.Drawing;
using System.Windows.Forms;
using Microsoft.VisualBasic;

namespace SpreadsheetGUI
{
    public partial class SpreadSheetWindow : Form, ISpreadSheetView
    {
        private string previousSaveName;
        private string title;

        // Events
        public event Action<string> FileChosenEvent;
        public event Action<string> CountEvent;
        public event Action<FormClosingEventArgs> CloseEvent;
        public event Action NewEvent;
        public event Action<string, string> SetCellContents;
        public event Action<string> GetCellValue;
        public event Action<string> GetCellContents;
        public event Action HelpMenu;
        public event Action OpenSpreadsheet;
        public event Action OpenNewSpreadsheet;
        public event Action<string> SaveSpreadsheet;
        public event Action<string> SaveAs;

        public string PreviousSaveName { get => this.previousSaveName; set => this.previousSaveName = value; }

        /// <summary>
        /// Title of window
        /// </summary>
        public string Title {
            get => title;
            set
            {
                title = value;
                this.Text = title;
            }
        }

        /// <summary>
        /// Error message
        /// </summary>
        public string Message { set => MessageBox.Show(value); }

        public string[] CurrentCellValue
        {
            set
            {
                // Turn string representation of cell name into int representation. A1 -> (0, 0)
                Int32.TryParse(value[1], out int row);
                Char.TryParse(value[0], out char col);
                int intCol = col - 'A';
                spreadsheetPanel.SetValue(intCol, row - 1, value[2]);
            }
            
        }
        
        public string[] UpdateCellDisplay
        {
            set
            {
                // Turn string representation of cell name into int representation. A1 -> (0, 0)
                Int32.TryParse(value[1], out int row);
                Char.TryParse(value[0], out char col);
                int intCol = col - 'A';
                spreadsheetPanel.SetValue(intCol, row - 1, value[2]);
            }
        }


        public string FormulaEditBox { get => FormulaEditor.Text; set => FormulaEditor.Text = value; }

        /// <summary>
        /// Used to create a window for saving
        /// </summary>
        public object[] InteractionMsgBox
        {
            set
            {
                MsgBoxResult results = Interaction.MsgBox(value[0], (MsgBoxStyle)value[1], value[2]);

                if (results.Equals(MsgBoxResult.No) && !(value[3] is string))
                {
                    FormClosingEventArgs e = (FormClosingEventArgs)value[3];
                    e.Cancel = true;
                }
                else if (results.Equals(MsgBoxResult.Yes) && (value[3] is string))
                {
                    this.PreviousSaveName = (string)value[3];
                    this.Title = previousSaveName;
                    SaveSpreadsheet((string)value[3]);
                }
            }
        }

        public SpreadSheetWindow()
        {
            InitializeComponent();
        }

        /// <summary>
        /// Arrow keys, = key, and enter key
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void spreadsheetPanel_KeyDown(object sender, KeyEventArgs e)
        {
            FormulaEditor.Clear();
            

            int col = 0;
            int row = 0;

            // Left arrow key
            if (e.KeyCode == Keys.Left)
            {
                spreadsheetPanel.GetSelection(out col, out row);
                spreadsheetPanel.SetSelection(col - 1, row);
            }
            // Right arrow key
            else if (e.KeyCode == Keys.Right)
            {
                spreadsheetPanel.GetSelection(out col, out row);
                spreadsheetPanel.SetSelection(col + 1, row);
            }
            // Up arrow key
            else if (e.KeyCode == Keys.Up)
            {
                spreadsheetPanel.GetSelection(out col, out row);
                spreadsheetPanel.SetSelection(col, row - 1);
            }
            // Down arrow key
            else if (e.KeyCode == Keys.Down)
            {
                spreadsheetPanel.GetSelection(out col, out row);
                spreadsheetPanel.SetSelection(col, row + 1);
            }
            // Pressing enter will toggle the cell editor
            else if (e.KeyCode == Keys.Enter)
            {
                FormulaEditor.Select();
            }
            // Pressing = key will toggle the cell editor for a formula
            else if (e.KeyCode == Keys.Oemplus)
            {
                FormulaEditor.Text = "=";
                FormulaEditor.Select();
                System.Windows.Forms.SendKeys.Send("=");
            }
        }
       
        // Terminate window
        public void DoClose()
        {
            Close();
        }

        public void OpenNew()
        {
            // for testing
        }

        public void OpenNewFromFile(AbstractSpreadsheet ss, string path)
        {
            // Used for testing
        }

        private void FormulaEditor_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                spreadsheetPanel.GetSelection(out int col, out int row);

                string contents = FormulaEditor.Text;
                SetCellContents(I2S_NameConvert(col, row), contents);

                // Deselect editor box
                spreadsheetPanel.Select();

            }
        }


        /// <summary>
        /// Helper method used to convert integer representation of cell location (x, y) to string representation LetterNumber
        /// </summary>
        /// <param name="col"></param>
        /// <param name="row"></param>
        /// <returns></returns>
        private string I2S_NameConvert(int col, int row)
        {
            row++;
            char colChar = (char)Convert.ToChar(col);
            colChar = (char)(colChar + 'A');
            return colChar.ToString() + row.ToString();
        }

        /// <summary>
        /// Any time a new cell is selected with arrow keys, display cell's contents in formula editor box
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void spreadsheetPanel_KeyUp(object sender, KeyEventArgs e)
        {
            spreadsheetPanel.GetSelection(out int col, out int row);
            GetCellContents(I2S_NameConvert(col, row));
            FormulaEditor.Text = FormulaEditBox;

        }

        private void openHelpWindowToolStripMenuItem_Click(object sender, EventArgs e)
        {
            HelpMenu();
        }

        private void saveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SaveSpreadsheet(this.Title); // defalut save name = title window
        }

        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenSpreadsheet();
        }

        private void openNewToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenNewSpreadsheet();
        }

        /// <summary>
        /// Display image. Incase image is missing from current directory, display message with important info
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void importantInformationToolStripMenuItem_Click(object sender, EventArgs e)
        {
            try
            {
                using (Form form = new Form())
                {
                    PictureBox pb = new PictureBox();
                    pb.Width = 100000;
                    pb.Height = 100000;
                    Bitmap image = new Bitmap("image.jpg");
                    pb.Dock = DockStyle.Fill;
                    pb.Image = (Image)image;
                    Controls.Add(pb);

                    form.Controls.Add(pb);
                    form.ShowDialog();
                }
            }
            catch
            {
                string message = "Why are assembly programmers always soaking wet? They work below C-level.\n\n" + "To understand what recursion is, first you must understand recursion.\n\n" + "Three programmers walk into a bar. The first programmer holds up two fingers and says \"Three beers\".";
                System.Windows.Forms.MessageBox.Show(message);
            }
            
        }

        /// <summary>
        /// Any time a new cell is selected with mouse, display cell's contents in formula editor box
        /// </summary>
        /// <param name="sender"></param>
        private void spreadsheetPanel_SelectionChanged(SSGui.SpreadsheetPanel sender)
        {
            spreadsheetPanel.GetSelection(out int col, out int row);
            GetCellContents(I2S_NameConvert(col, row));
            FormulaEditor.Select();
        }

        private void saveAsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            string input = Interaction.InputBox("Please specify desired file name", "Save As", this.Text, -1, -1);

            if (input.Length != 0)
            {
                SaveAs(input);
            }
        }

        private void SpreadSheetWindow_FormClosing(object sender, FormClosingEventArgs e)
        {
            this.CloseEvent(e);
        }

        private void closeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            DoClose();
        }
    }
}
