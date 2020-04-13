using SS;
using System.IO;
using System.Windows.Forms;
using System;

namespace SpreadsheetGUI
{
    /// <summary>
    /// Keeps track of how many top-level forms are running, shuts down
    /// the application when there are no more.
    /// </summary>
    class SpreadSheetApplicationContext : ApplicationContext
    {
        // Number of open forms
        private int windowCount = 0;
        private int windowNameCount = 0;

        // Singleton ApplicationContext
        private static SpreadSheetApplicationContext context;

        /// <summary>
        /// Private constructor for singleton pattern
        /// </summary>
        private SpreadSheetApplicationContext()
        {
        }

        /// <summary>
        /// Returns the one DemoApplicationContext.
        /// </summary>
        public static SpreadSheetApplicationContext GetContext()
        {
            if (context == null)
            {
                context = new SpreadSheetApplicationContext();
            }
            return context;
        }

        /// <summary>
        /// Runs a form in this application context
        /// </summary>
        public void RunNew()
        {
            // Create the window and the controller
            SpreadSheetWindow window = new SpreadSheetWindow();
            new Controller(window);

            windowCount++;

            // One more form is running
            while (File.Exists("untitledSpreadsheet" + ++windowNameCount + ".ss"))
            {
                //empty block
            }
            window.Title = "untitledSpreadsheet" + windowNameCount + ".ss";

            window.PreviousSaveName = window.Title;

            // When this form closes, we want to find out
            window.FormClosed += (o, e) => { if (--windowCount <= 0) ExitThread(); };

            // Run the form
            window.Show();
        }



        public void RunNewFromFile(AbstractSpreadsheet ss, string name)
        {
            // Create the window and the controller
            SpreadSheetWindow window = new SpreadSheetWindow();
            
            // One more form is running
            windowCount++;

            window.Title = name;

            window.PreviousSaveName = window.Title;

            // When this form closes, we want to find out
            window.FormClosed += (o, e) => { if (--windowCount <= 0) ExitThread(); };

            // Run the form
            window.Show();

            new Controller(window, ss, name);



        }
    }
}
