namespace ToDoListClient
{
    partial class ToDoListView
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
            this.nameLabel = new System.Windows.Forms.Label();
            this.nameBox = new System.Windows.Forms.TextBox();
            this.emailBox = new System.Windows.Forms.TextBox();
            this.emailLabel = new System.Windows.Forms.Label();
            this.registerButton = new System.Windows.Forms.Button();
            this.taskLabel = new System.Windows.Forms.Label();
            this.taskBox = new System.Windows.Forms.TextBox();
            this.taskButton = new System.Windows.Forms.Button();
            this.allTaskButton = new System.Windows.Forms.CheckBox();
            this.showCompletedTasksButton = new System.Windows.Forms.CheckBox();
            this.taskPanel = new System.Windows.Forms.TableLayoutPanel();
            this.cancelButton = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // nameLabel
            // 
            this.nameLabel.AutoSize = true;
            this.nameLabel.Location = new System.Drawing.Point(19, 29);
            this.nameLabel.Name = "nameLabel";
            this.nameLabel.Size = new System.Drawing.Size(45, 17);
            this.nameLabel.TabIndex = 0;
            this.nameLabel.Text = "Name";
            // 
            // nameBox
            // 
            this.nameBox.Location = new System.Drawing.Point(70, 29);
            this.nameBox.Name = "nameBox";
            this.nameBox.Size = new System.Drawing.Size(150, 22);
            this.nameBox.TabIndex = 1;
            this.nameBox.TextChanged += new System.EventHandler(this.registration_TextChanged);
            // 
            // emailBox
            // 
            this.emailBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.emailBox.Location = new System.Drawing.Point(288, 29);
            this.emailBox.Name = "emailBox";
            this.emailBox.Size = new System.Drawing.Size(206, 22);
            this.emailBox.TabIndex = 2;
            this.emailBox.TextChanged += new System.EventHandler(this.registration_TextChanged);
            // 
            // emailLabel
            // 
            this.emailLabel.AutoSize = true;
            this.emailLabel.Location = new System.Drawing.Point(240, 29);
            this.emailLabel.Name = "emailLabel";
            this.emailLabel.Size = new System.Drawing.Size(42, 17);
            this.emailLabel.TabIndex = 3;
            this.emailLabel.Text = "Email";
            // 
            // registerButton
            // 
            this.registerButton.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.registerButton.Enabled = false;
            this.registerButton.Location = new System.Drawing.Point(15, 66);
            this.registerButton.Name = "registerButton";
            this.registerButton.Size = new System.Drawing.Size(479, 23);
            this.registerButton.TabIndex = 4;
            this.registerButton.Text = "Register";
            this.registerButton.UseVisualStyleBackColor = true;
            this.registerButton.Click += new System.EventHandler(this.registerButton_Click);
            // 
            // taskLabel
            // 
            this.taskLabel.AutoSize = true;
            this.taskLabel.Location = new System.Drawing.Point(12, 125);
            this.taskLabel.Name = "taskLabel";
            this.taskLabel.Size = new System.Drawing.Size(39, 17);
            this.taskLabel.TabIndex = 5;
            this.taskLabel.Text = "Task";
            // 
            // taskBox
            // 
            this.taskBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.taskBox.Location = new System.Drawing.Point(57, 120);
            this.taskBox.Name = "taskBox";
            this.taskBox.Size = new System.Drawing.Size(524, 22);
            this.taskBox.TabIndex = 6;
            this.taskBox.TextChanged += new System.EventHandler(this.taskBox_TextChanged);
            // 
            // taskButton
            // 
            this.taskButton.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.taskButton.Enabled = false;
            this.taskButton.Location = new System.Drawing.Point(12, 163);
            this.taskButton.Name = "taskButton";
            this.taskButton.Size = new System.Drawing.Size(569, 23);
            this.taskButton.TabIndex = 7;
            this.taskButton.Text = "Submit Task";
            this.taskButton.UseVisualStyleBackColor = true;
            this.taskButton.Click += new System.EventHandler(this.taskButton_Click);
            // 
            // allTaskButton
            // 
            this.allTaskButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.allTaskButton.AutoSize = true;
            this.allTaskButton.Enabled = false;
            this.allTaskButton.Location = new System.Drawing.Point(58, 625);
            this.allTaskButton.Name = "allTaskButton";
            this.allTaskButton.Size = new System.Drawing.Size(186, 21);
            this.allTaskButton.TabIndex = 9;
            this.allTaskButton.Text = "Show Tasks for all Users";
            this.allTaskButton.UseVisualStyleBackColor = true;
            this.allTaskButton.CheckedChanged += new System.EventHandler(this.allTaskButton_CheckedChanged);
            // 
            // showCompletedTasksButton
            // 
            this.showCompletedTasksButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.showCompletedTasksButton.AutoSize = true;
            this.showCompletedTasksButton.Enabled = false;
            this.showCompletedTasksButton.Location = new System.Drawing.Point(325, 625);
            this.showCompletedTasksButton.Name = "showCompletedTasksButton";
            this.showCompletedTasksButton.Size = new System.Drawing.Size(210, 21);
            this.showCompletedTasksButton.TabIndex = 10;
            this.showCompletedTasksButton.Text = "Show Only Completed Tasks";
            this.showCompletedTasksButton.UseVisualStyleBackColor = true;
            this.showCompletedTasksButton.CheckedChanged += new System.EventHandler(this.showCompletedTasksButton_CheckedChanged);
            // 
            // taskPanel
            // 
            this.taskPanel.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.taskPanel.AutoScroll = true;
            this.taskPanel.CellBorderStyle = System.Windows.Forms.TableLayoutPanelCellBorderStyle.Inset;
            this.taskPanel.ColumnCount = 3;
            this.taskPanel.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 85.80247F));
            this.taskPanel.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 14.19753F));
            this.taskPanel.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 79F));
            this.taskPanel.Location = new System.Drawing.Point(12, 208);
            this.taskPanel.Name = "taskPanel";
            this.taskPanel.RowCount = 1;
            this.taskPanel.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.taskPanel.Size = new System.Drawing.Size(569, 401);
            this.taskPanel.TabIndex = 11;
            // 
            // cancelButton
            // 
            this.cancelButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.cancelButton.Enabled = false;
            this.cancelButton.Location = new System.Drawing.Point(513, 29);
            this.cancelButton.Name = "cancelButton";
            this.cancelButton.Size = new System.Drawing.Size(68, 62);
            this.cancelButton.TabIndex = 12;
            this.cancelButton.Text = "Cancel";
            this.cancelButton.UseVisualStyleBackColor = true;
            this.cancelButton.Click += new System.EventHandler(this.cancelButton_Click);
            // 
            // ToDoListView
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(593, 655);
            this.Controls.Add(this.cancelButton);
            this.Controls.Add(this.taskPanel);
            this.Controls.Add(this.showCompletedTasksButton);
            this.Controls.Add(this.allTaskButton);
            this.Controls.Add(this.taskButton);
            this.Controls.Add(this.taskBox);
            this.Controls.Add(this.taskLabel);
            this.Controls.Add(this.registerButton);
            this.Controls.Add(this.emailLabel);
            this.Controls.Add(this.emailBox);
            this.Controls.Add(this.nameBox);
            this.Controls.Add(this.nameLabel);
            this.Name = "ToDoListView";
            this.Text = "To Do List";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label nameLabel;
        private System.Windows.Forms.TextBox nameBox;
        private System.Windows.Forms.TextBox emailBox;
        private System.Windows.Forms.Label emailLabel;
        private System.Windows.Forms.Button registerButton;
        private System.Windows.Forms.Label taskLabel;
        private System.Windows.Forms.TextBox taskBox;
        private System.Windows.Forms.Button taskButton;
        private System.Windows.Forms.CheckBox allTaskButton;
        private System.Windows.Forms.CheckBox showCompletedTasksButton;
        private System.Windows.Forms.TableLayoutPanel taskPanel;
        private System.Windows.Forms.Button cancelButton;
    }
}

