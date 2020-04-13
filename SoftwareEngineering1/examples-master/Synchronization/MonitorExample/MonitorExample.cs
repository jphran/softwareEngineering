using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;

namespace MonitorExample
{
    public class MonitorExample
    {
        private static int n = 0;
        private const int LIMIT = 100000000;
        private static readonly object sync = new object();

        public static void Main(string[] args)
        {
            Task t1 = Task.Run(() => Increment());
            Task t2 = Task.Run(() => Decrement());
            while (!t1.IsCompleted || !t2.IsCompleted)
            {
                Console.WriteLine(n);
                Thread.Sleep(250);
            }
            Console.WriteLine(n);
        }

        public static void Increment()
        {
            Console.WriteLine("Increment Starting");
            for (int i = 0; i < LIMIT; i++)
            {
                Monitor.Enter(sync);
                try
                {
                    n++;
                }
                finally
                {
                    Monitor.Exit(sync);
                }
            }
            Console.WriteLine("Increment Done");
        }

        public static void Decrement()
        {
            Console.WriteLine("Decrement Starting");
            for (int i = 0; i < LIMIT; i++)
            {
                Monitor.Enter(sync);
                try
                {
                    n--;
                }
                finally
                {
                    Monitor.Exit(sync);
                }
            }
            Console.WriteLine("Decrement Done");
        }
    }
}
