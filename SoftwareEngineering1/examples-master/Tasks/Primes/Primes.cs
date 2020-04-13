using System;
using System.Diagnostics;

namespace Primes
{
    public static class Primes
    {
        public static void Main()
        {
            Stopwatch sw = new Stopwatch();
            sw.Start();
            int count = CountOddPrimes(14000000);
            sw.Stop();
            Console.WriteLine("Time = " + sw.ElapsedMilliseconds + " msecs");
            Console.WriteLine(count);
            Console.ReadLine();
        }

        /// <summary>
        /// Returns the number of odd primes less or equal to maxPrime
        /// </summary>
        public static int CountOddPrimes (int maxPrime)
        {
            return CountPrimesInInterval(1, maxPrime, 2);
        }

        /// <summary>
        /// Returns the number of primes in the set
        ///   {minPrime, minPrime+delta, minPrime+2*delta, ...}
        /// where the last number in the sequence is less than or equal to maxPrime
        /// </summary>
        public static int CountPrimesInInterval(int minPrime, int maxPrime, int delta)
        {
            Console.WriteLine("Counting primes from " + minPrime + " to " + maxPrime + " stepping by " + delta);
            int count = 0;
            for (int p = minPrime; p <= maxPrime; p += delta)
            {
                if (IsPrime(p)) count++;
            }
            return count;
        }

        /// <summary>
        /// Reports whether p is prime.
        /// </summary>
        public static bool IsPrime (int p)
        {
            if (p < 2) return false;
            for (int f = 2; f*f <= p; f++)
            {
                if (p % f == 0) return false;
            }
            return true;
        }
    }
}
