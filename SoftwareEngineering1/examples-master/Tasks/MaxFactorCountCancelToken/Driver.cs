using System;
using System.Diagnostics;
using System.Threading;
using System.Threading.Tasks;

namespace FactorsToken
{
    class Driver
    {
        public static void Main(string[] args)
        {
            try
            {
                MaxFactorCount counter = new MaxFactorCountPrintSync();

                // Create a cancellation token
                CancellationTokenSource source = new CancellationTokenSource();

                // Start a task to do cancellation
                Task.Run(() => WaitForStopRequest(source));

                Stopwatch sw = new Stopwatch();
                sw.Start();
                int number = counter.FindMaxFactors(120000, 7, source.Token);
                sw.Stop();
                Console.WriteLine("Time = " + sw.ElapsedMilliseconds + " msecs");
                Console.WriteLine(number + " has " + MaxFactorCount.CountFactors(number) + " factors");
            }
            catch (OperationCanceledException)
            {
                Console.WriteLine("Operation canceled");
            }
            Console.ReadLine();
        }

        /// <summary>
        /// Cancels an ongoing computation if a line is entered
        /// </summary>
        private static void WaitForStopRequest(CancellationTokenSource source)
        {
            Console.ReadLine();
            source.Cancel();
        }
    }
}
