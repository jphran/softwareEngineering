using System;

namespace Factors
{
    public class MaxFactorCountPrint : MaxFactorCount
    {
        /// <summary>
        /// The number currently known to have the most factors
        /// </summary>
        private int currentMaxFactors = 0;

        /// <summary>
        /// The number of factors that currentMaxFactors has
        /// </summary>
        private int currentMaxCount = 0;

        /// <summary>
        /// If maxFactors and maxCount are the best seen by any thread, update the
        /// record and print out a notification.
        /// </summary>
        protected override void UpdateStatistics(int maxFactors, int maxCount)
        {
            if (maxCount > currentMaxCount)
            {
                currentMaxFactors = maxFactors;
                currentMaxCount = maxCount;
                Console.WriteLine(currentMaxFactors + " has " + currentMaxCount + " factors");
            }
        }
    }
}
