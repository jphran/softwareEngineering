using System;
using System.Diagnostics;
using System.Threading.Tasks;

namespace Primes
{
    public static class PrimesTwoTasks
    {
        public static void Main()
        {
            Stopwatch sw = new Stopwatch();
            sw.Start();
            int count = CountOddPrimesTwoTasks(14000000);
            sw.Stop();
            Console.WriteLine("Time = " + sw.ElapsedMilliseconds + " msecs");
            Console.WriteLine(count);
            Console.ReadLine();
        }

        /// <summary>
        /// Returns the number of odd primes less than or equal to maxPrime.
        /// </summary>
        public static int CountOddPrimesTwoTasks(int maxPrime)
        {
            Task<int> t1 = Task.Run(() => Primes.CountPrimesInInterval(1, maxPrime, 4));
            Task<int> t2 = Task.Run(() => Primes.CountPrimesInInterval(3, maxPrime, 4));
            return t1.Result + t2.Result;
        }
    }
}

