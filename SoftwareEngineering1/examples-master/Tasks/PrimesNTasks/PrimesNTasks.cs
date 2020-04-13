using System;
using System.Diagnostics;
using System.Threading.Tasks;

namespace Primes
{
    public static class PrimesNTasks
    {
        public static void Main()
        {
            Stopwatch sw = new Stopwatch();
            sw.Start();
            int count = CountOddPrimesNTasks(14000000, 8);
            sw.Stop();
            Console.WriteLine("Time = " + sw.ElapsedMilliseconds + " msecs");
            Console.WriteLine(count);
            Console.ReadLine();
        }

        /// <summary>
        /// Returns the number of odd primes less than or equal to maxPrime, 
        /// using n Tasks.
        /// </summary>
        public static int CountOddPrimesNTasks(int maxPrime, int n)
        {
            Task<int>[] tasks = new Task<int>[n];
            for (int i = 0; i < n; i++)
            {
                // Understand what happens if the minPrime local is not used
                int minPrime = i * 2 + 1;
                int delta = 2 * n;
                tasks[i] = Task.Run(() => Primes.CountPrimesInInterval(minPrime, maxPrime, delta));
                //System.Threading.Thread.Sleep(500);
            }

            int count = 0;
            foreach (Task<int> task in tasks)
            {
                count += task.Result;
            }
            return count;
        }
    }
}
