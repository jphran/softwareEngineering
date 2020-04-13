namespace MaxFactorGUI
{
    partial class Form1
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
            this.highLimit = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.startButton1 = new System.Windows.Forms.Button();
            this.label2 = new System.Windows.Forms.Label();
            this.factorCount = new System.Windows.Forms.TextBox();
            this.cancelButton = new System.Windows.Forms.Button();
            this.startButton2 = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // highLimit
            // 
            this.highLimit.Location = new System.Drawing.Point(136, 41);
            this.highLimit.Name = "highLimit";
            this.highLimit.Size = new System.Drawing.Size(180, 22);
            this.highLimit.TabIndex = 0;
            this.highLimit.TextChanged += new System.EventHandler(this.highLimit_TextChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(34, 44);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(70, 17);
            this.label1.TabIndex = 1;
            this.label1.Text = "High Limit";
            // 
            // startButton1
            // 
            this.startButton1.Enabled = false;
            this.startButton1.Location = new System.Drawing.Point(37, 151);
            this.startButton1.Name = "startButton1";
            this.startButton1.Size = new System.Drawing.Size(279, 23);
            this.startButton1.TabIndex = 2;
            this.startButton1.Text = "Start";
            this.startButton1.UseVisualStyleBackColor = true;
            this.startButton1.Click += new System.EventHandler(this.startButton1_Click);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(37, 94);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(89, 17);
            this.label2.TabIndex = 3;
            this.label2.Text = "Factor Count";
            // 
            // factorCount
            // 
            this.factorCount.Location = new System.Drawing.Point(136, 88);
            this.factorCount.Name = "factorCount";
            this.factorCount.ReadOnly = true;
            this.factorCount.Size = new System.Drawing.Size(180, 22);
            this.factorCount.TabIndex = 4;
            // 
            // cancelButton
            // 
            this.cancelButton.Enabled = false;
            this.cancelButton.Location = new System.Drawing.Point(37, 225);
            this.cancelButton.Name = "cancelButton";
            this.cancelButton.Size = new System.Drawing.Size(279, 23);
            this.cancelButton.TabIndex = 5;
            this.cancelButton.Text = "Cancel";
            this.cancelButton.UseVisualStyleBackColor = true;
            this.cancelButton.Click += new System.EventHandler(this.cancelButton_Click);
            // 
            // startButton2
            // 
            this.startButton2.Enabled = false;
            this.startButton2.Location = new System.Drawing.Point(37, 186);
            this.startButton2.Name = "startButton2";
            this.startButton2.Size = new System.Drawing.Size(279, 22);
            this.startButton2.TabIndex = 6;
            this.startButton2.Text = "Start";
            this.startButton2.UseVisualStyleBackColor = true;
            this.startButton2.Click += new System.EventHandler(this.startButton2_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(353, 287);
            this.Controls.Add(this.startButton2);
            this.Controls.Add(this.cancelButton);
            this.Controls.Add(this.factorCount);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.startButton1);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.highLimit);
            this.Name = "Form1";
            this.Text = "Max Factor Counter";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox highLimit;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button startButton1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox factorCount;
        private System.Windows.Forms.Button cancelButton;
        private System.Windows.Forms.Button startButton2;
    }
}

