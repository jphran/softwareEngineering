using System;
using System.Diagnostics;

namespace Factors
{
    class Driver
    {
        public static void Main(string[] args)
        {
            MaxFactorCount counter;
            //counter = new MaxFactorCount();
            //counter = new MaxFactorCountPrint();
            counter = new MaxFactorCountPrintSync();
            Stopwatch sw = new Stopwatch();
            sw.Start();
            int number = counter.FindMaxFactors(120000, 7);
            sw.Stop();
            Console.WriteLine("Time = " + sw.ElapsedMilliseconds + " msecs");
            Console.WriteLine(number + " has " + MaxFactorCount.CountFactors(number) + " factors");
            Console.ReadLine();
        }
    }
}
