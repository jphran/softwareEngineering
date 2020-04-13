using System;
using System.Threading.Tasks;

namespace Factors
{
    public class MaxFactorCount
    {
        /// <summary>
        /// When set to true, the computation threads terminate
        /// </summary>
        private bool cancel = false;

        /// <summary>
        /// Returns the integer between 1 and limit that has the most factors,
        /// using nTasks Tasks.
        /// </summary>
        public int FindMaxFactors(int limit, int nTasks)
        {
            // Use multiple Tasks to solve slices of the problem
            Task<int>[] tasks = new Task<int>[nTasks];
            for (int i = 0; i < nTasks; i++)
            {
                int begin = i + 1;
                tasks[i] = Task.Run(() => FindMaxFactorsInInterval(begin, limit, nTasks));
            }

            // Start a task that waits for the user to stop the computation
            Task.Run(() => WaitForStopRequest());

            // Find and return the best result found by the Tasks
            int maxFactors = 0;
            int maxCount = 0;
            foreach (Task<int> task in tasks)
            {
                int taskMaxFactors = task.Result;
                int taskMaxCount = CountFactors(taskMaxFactors);
                if (taskMaxCount > maxCount)
                {
                    maxFactors = taskMaxFactors;
                    maxCount = taskMaxCount;
                }
            }
            return maxFactors;
        }

        /// <summary>
        /// Cancels an ongoing computation if a line is entered
        /// </summary>
        private void WaitForStopRequest ()
        {
            Console.ReadLine();
            cancel = true;
        }


        /// <summary>
        /// Returns the integer in the set
        /// {lo, lo+delta, lo+2*delta, ...}
        /// where all numbers are less than or equal to hi
        /// that has the most factors
        /// </summary>
        protected int FindMaxFactorsInInterval(int lo, int hi, int delta)
        {
            int maxFactors = 0;
            int maxCount = 0;
            for (int number = lo; number <= hi; number += delta)
            {
                if (cancel) return 0;
                int count = CountFactors(number);
                if (count > maxCount)
                {
                    maxCount = count;
                    maxFactors = number;
                    UpdateStatistics(maxFactors, maxCount);
                }
            }
            return maxFactors;
        }

        /// <summary>
        /// Override this to do something interesting when a new maximum is found
        /// </summary>
        protected virtual void UpdateStatistics (int bestSoFar, int factorCoung)
        {
        }

        /// <summary>
        /// Returns the number of distinct factors of n.
        /// </summary>
        public static int CountFactors(int n)
        {
            int count = 2;
            for (int f = 2; f <= n / 2; f++)
            {
                if (n % f == 0) count++;
            }
            return count;
        }
    }
}
