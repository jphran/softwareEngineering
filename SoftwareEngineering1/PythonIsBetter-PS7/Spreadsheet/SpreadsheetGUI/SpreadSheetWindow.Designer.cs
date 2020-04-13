namespace SpreadsheetGUI
{
    partial class SpreadSheetWindow
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.FormulaSymbol = new System.Windows.Forms.RichTextBox();
            this.FormulaEditor = new System.Windows.Forms.TextBox();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fIleToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.openToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.openNewToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveAsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.eDitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.helpToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.openHelpWindowToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.importantInformationToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.spreadsheetPanel = new SSGui.SpreadsheetPanel();
            this.closeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.menuStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // FormulaSymbol
            // 
            this.FormulaSymbol.Enabled = false;
            this.FormulaSymbol.Location = new System.Drawing.Point(12, 47);
            this.FormulaSymbol.Name = "FormulaSymbol";
            this.FormulaSymbol.ReadOnly = true;
            this.FormulaSymbol.ScrollBars = System.Windows.Forms.RichTextBoxScrollBars.None;
            this.FormulaSymbol.Size = new System.Drawing.Size(48, 31);
            this.FormulaSymbol.TabIndex = 1;
            this.FormulaSymbol.Text = "f(x)";
            // 
            // FormulaEditor
            // 
            this.FormulaEditor.AcceptsReturn = true;
            this.FormulaEditor.Location = new System.Drawing.Point(144, 47);
            this.FormulaEditor.Name = "FormulaEditor";
            this.FormulaEditor.Size = new System.Drawing.Size(742, 31);
            this.FormulaEditor.TabIndex = 3;
            this.FormulaEditor.KeyDown += new System.Windows.Forms.KeyEventHandler(this.FormulaEditor_KeyDown);
            // 
            // menuStrip1
            // 
            this.menuStrip1.ImageScalingSize = new System.Drawing.Size(32, 32);
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fIleToolStripMenuItem,
            this.eDitToolStripMenuItem,
            this.helpToolStripMenuItem,
            this.importantInformationToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(2213, 42);
            this.menuStrip1.TabIndex = 4;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fIleToolStripMenuItem
            // 
            this.fIleToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.openToolStripMenuItem,
            this.openNewToolStripMenuItem,
            this.saveToolStripMenuItem,
            this.saveAsToolStripMenuItem,
            this.closeToolStripMenuItem});
            this.fIleToolStripMenuItem.Name = "fIleToolStripMenuItem";
            this.fIleToolStripMenuItem.Size = new System.Drawing.Size(64, 38);
            this.fIleToolStripMenuItem.Text = "File";
            // 
            // openToolStripMenuItem
            // 
            this.openToolStripMenuItem.Name = "openToolStripMenuItem";
            this.openToolStripMenuItem.Size = new System.Drawing.Size(324, 38);
            this.openToolStripMenuItem.Text = "Open";
            this.openToolStripMenuItem.Click += new System.EventHandler(this.openToolStripMenuItem_Click);
            // 
            // openNewToolStripMenuItem
            // 
            this.openNewToolStripMenuItem.Name = "openNewToolStripMenuItem";
            this.openNewToolStripMenuItem.Size = new System.Drawing.Size(324, 38);
            this.openNewToolStripMenuItem.Text = "New";
            this.openNewToolStripMenuItem.Click += new System.EventHandler(this.openNewToolStripMenuItem_Click);
            // 
            // saveToolStripMenuItem
            // 
            this.saveToolStripMenuItem.Name = "saveToolStripMenuItem";
            this.saveToolStripMenuItem.Size = new System.Drawing.Size(324, 38);
            this.saveToolStripMenuItem.Text = "Save";
            this.saveToolStripMenuItem.Click += new System.EventHandler(this.saveToolStripMenuItem_Click);
            // 
            // saveAsToolStripMenuItem
            // 
            this.saveAsToolStripMenuItem.Name = "saveAsToolStripMenuItem";
            this.saveAsToolStripMenuItem.Size = new System.Drawing.Size(324, 38);
            this.saveAsToolStripMenuItem.Text = "Save As";
            this.saveAsToolStripMenuItem.Click += new System.EventHandler(this.saveAsToolStripMenuItem_Click);
            // 
            // eDitToolStripMenuItem
            // 
            this.eDitToolStripMenuItem.Name = "eDitToolStripMenuItem";
            this.eDitToolStripMenuItem.Size = new System.Drawing.Size(67, 36);
            this.eDitToolStripMenuItem.Text = "Edit";
            // 
            // helpToolStripMenuItem
            // 
            this.helpToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.openHelpWindowToolStripMenuItem});
            this.helpToolStripMenuItem.Name = "helpToolStripMenuItem";
            this.helpToolStripMenuItem.Size = new System.Drawing.Size(77, 36);
            this.helpToolStripMenuItem.Text = "Help";
            // 
            // openHelpWindowToolStripMenuItem
            // 
            this.openHelpWindowToolStripMenuItem.Name = "openHelpWindowToolStripMenuItem";
            this.openHelpWindowToolStripMenuItem.Size = new System.Drawing.Size(324, 38);
            this.openHelpWindowToolStripMenuItem.Text = "Open Help Window";
            this.openHelpWindowToolStripMenuItem.Click += new System.EventHandler(this.openHelpWindowToolStripMenuItem_Click);
            // 
            // importantInformationToolStripMenuItem
            // 
            this.importantInformationToolStripMenuItem.Name = "importantInformationToolStripMenuItem";
            this.importantInformationToolStripMenuItem.Size = new System.Drawing.Size(264, 36);
            this.importantInformationToolStripMenuItem.Text = "Important Information";
            this.importantInformationToolStripMenuItem.Click += new System.EventHandler(this.importantInformationToolStripMenuItem_Click);
            // 
            // spreadsheetPanel
            // 
            this.spreadsheetPanel.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.spreadsheetPanel.AutoSize = true;
            this.spreadsheetPanel.Location = new System.Drawing.Point(12, 97);
            this.spreadsheetPanel.Name = "spreadsheetPanel";
            this.spreadsheetPanel.Size = new System.Drawing.Size(1225, 602);
            this.spreadsheetPanel.TabIndex = 0;
            this.spreadsheetPanel.SelectionChanged += new SSGui.SelectionChangedHandler(this.spreadsheetPanel_SelectionChanged);
            this.spreadsheetPanel.KeyDown += new System.Windows.Forms.KeyEventHandler(this.spreadsheetPanel_KeyDown);
            this.spreadsheetPanel.KeyUp += new System.Windows.Forms.KeyEventHandler(this.spreadsheetPanel_KeyUp);
            // 
            // closeToolStripMenuItem
            // 
            this.closeToolStripMenuItem.Name = "closeToolStripMenuItem";
            this.closeToolStripMenuItem.Size = new System.Drawing.Size(324, 38);
            this.closeToolStripMenuItem.Text = "Close";
            this.closeToolStripMenuItem.Click += new System.EventHandler(this.closeToolStripMenuItem_Click);
            // 
            // SpreadSheetWindow
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(12F, 25F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(2213, 769);
            this.Controls.Add(this.FormulaEditor);
            this.Controls.Add(this.FormulaSymbol);
            this.Controls.Add(this.spreadsheetPanel);
            this.Controls.Add(this.menuStrip1);
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "SpreadSheetWindow";
            this.Text = "Form1";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.SpreadSheetWindow_FormClosing);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private SSGui.SpreadsheetPanel spreadsheetPanel;
        private System.Windows.Forms.RichTextBox FormulaSymbol;
        private System.Windows.Forms.TextBox FormulaEditor;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fIleToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem openToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveAsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem eDitToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem helpToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem openHelpWindowToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem openNewToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem importantInformationToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem closeToolStripMenuItem;
    }
}

