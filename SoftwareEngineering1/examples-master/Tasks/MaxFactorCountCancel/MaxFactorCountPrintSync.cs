namespace Factors
{
    public class MaxFactorCountPrintSync: MaxFactorCountPrint
    {
        /// <summary>
        /// Updates the statistics in a critical section
        /// </summary>
        protected override void UpdateStatistics(int bestSoFar, int factorCount)
        {
            lock(this)
            {
                base.UpdateStatistics(bestSoFar, factorCount);
            }
        }
    }
}
