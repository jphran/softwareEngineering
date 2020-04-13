using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;

namespace ManualResetEventDemo
{
    public class MREDemo
    {
        private static ManualResetEvent mre = new ManualResetEvent(false);

        public static void Main ()
        {
            Task t1 = Task.Run(() => Task1Method());
            Task t2 = Task.Run(() => Task2Method());
            t1.Wait();
            t2.Wait();
        }

        private static void Task1Method()
        {
            Console.WriteLine("Task 1 is running");
            Console.WriteLine("Task 1 is waiting for mre to be set");
            mre.WaitOne();
            Console.WriteLine("Task 1 is finished");
        }

        private static void Task2Method()
        {
            Console.WriteLine("Task 2 is running");
            Console.WriteLine("Task 2: Press enter to set the mre");
            Console.ReadLine();
            mre.Set();
            Console.WriteLine("Task 2 is finished");
        }
    }
}
