using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace MaxFactorGUI
{
    public partial class Form1 : Form
    {
        private CancellationTokenSource tokenSource;

        public Form1()
        {
            InitializeComponent();
        }

        private void highLimit_TextChanged(object sender, EventArgs e)
        {
            int highBound;
            if (Int32.TryParse(highLimit.Text, out highBound))
            {
                startButton1.Enabled = true;
                startButton2.Enabled = true;
                factorCount.Text = "";
            }
            else
            {
                startButton1.Enabled = false;
                startButton2.Enabled = false;
                factorCount.Text = "";
            }
        }

        private void startButton1_Click(object sender, EventArgs e)
        {
            int limit;
            if (Int32.TryParse(highLimit.Text, out limit))
            {
                startButton1.Enabled = false;
                startButton2.Enabled = false;
                highLimit.Enabled = false;
                Factors.MaxFactorCount counter = new Factors.MaxFactorCount();
                int count = counter.FindMaxFactors(limit, 2);
                factorCount.Text = count + " has the most factors";
                startButton1.Enabled = true;
                startButton2.Enabled = true;
                highLimit.Enabled = true;
            }
        }

        private void startButton2_Click(object sender, EventArgs e)
        {
            int limit;
            if (Int32.TryParse(highLimit.Text, out limit))
            {
                startButton1.Enabled = false;
                startButton2.Enabled = false;
                highLimit.Enabled = false;
                cancelButton.Enabled = true;

                tokenSource = new CancellationTokenSource();
                Task.Run(() => DoFactorCount(limit, 2, tokenSource.Token));                
            }
        }

        private void DoFactorCount(int limit, int nTasks, CancellationToken token)
        {
            FactorsToken.MaxFactorCount counter = new FactorsToken.MaxFactorCount();
            try
            {
                int count = counter.FindMaxFactors(limit, nTasks, token);
                //ResetAfterCancel(count.ToString());
                factorCount.Invoke((Action)(() => ResetAfterCancel(count.ToString())));
            }
            catch (OperationCanceledException)
            {
                factorCount.Invoke((Action) (() => ResetAfterCancel("Canceled")));
                //ResetAfterCancel("Canceled");
            }
        }

        private void ResetAfterCancel (string message)
        {
            factorCount.Text = message;
            startButton1.Enabled = true;
            startButton2.Enabled = true;
            highLimit.Enabled = true;
            cancelButton.Enabled = false;
        }

        private void cancelButton_Click(object sender, EventArgs e)
        {
            tokenSource.Cancel();
        }
    }
}
