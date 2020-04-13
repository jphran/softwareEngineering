using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;

namespace InterlockExample
{
    public class InterlockExample
    {
        private static int n = 0;
        private const int LIMIT = 100000000;

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

        public static void Increment ()
        {
            Console.WriteLine("Increment Starting");
            for (int i = 0; i < LIMIT; i++)
            {
                Interlocked.Increment(ref n);
            }
            Console.WriteLine("Increment Done");
        }

        public static void Decrement()
        {
            Console.WriteLine("Decrement Starting");
            for (int i = 0; i < LIMIT; i++)
            {
                Interlocked.Decrement(ref n);
            }
            Console.WriteLine("Decrement Done");
        }
    }
}
