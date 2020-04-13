namespace Factors
{
    public class MaxFactorCountPrintSync: MaxFactorCountPrint
    {
        private readonly object sync = new object();

        /// <summary>
        /// Updates the statistics in a critical section
        /// </summary>
        protected override void UpdateStatistics(int bestSoFar, int factorCount)
        {
            lock(sync)
            {
                base.UpdateStatistics(bestSoFar, factorCount);
            }
        }
    }
}
