using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using MySynchronization;
using System.Threading;
using System.Threading.Tasks;

namespace ReadWriteLockTests
{
    [TestClass]
    public class RWTests
    {
        /// <summary>
        /// Make sure there can be multiple readers at the same time.
        /// </summary>
        [TestMethod, Timeout(2000)]
        public void TestMethod1()
        {
            int SIZE = 4;

            // These local variables are used by the nested method Reader()
            IMyReadWriteLock rwLock = new MyReadWriteLock1();
            ManualResetEvent mre1 = new ManualResetEvent(false);
            ManualResetEvent mre2 = new ManualResetEvent(false);
            int count = SIZE;

            // Call Reader() on SIZE tasks
            Task[] tasks = new Task[SIZE];
            for (int i = 0; i < SIZE; i++)
            {
                tasks[i] = Task.Run(() => Reader());
            }

            // Wait to be signalled that count has reached zero.  For this to
            // happen, all tasks must be simultaneously in the critical section.
            mre1.WaitOne();

            // Signal the tasks that they can terminate
            mre2.Set();

            // This method is run on each task
            void Reader()
            {
                rwLock.EnterReadLock();
                try
                {
                    if (Interlocked.Decrement(ref count) == 0)
                    {
                        Console.WriteLine(count);
                        mre1.Set();
                    }
                    mre2.WaitOne();
                }
                finally
                {
                    rwLock.ExitReadLock();
                }
            }
        }

        /// <summary>
        /// Makes sure that writers enter the critical section sequentially
        /// </summary>
        [TestMethod, Timeout(2000)]
        public void TestMethod2()
        {
            int SIZE = 4;

            // These variable are shared by the nested Writer() method
            IMyReadWriteLock rwLock = new MyReadWriteLock1();
            AutoResetEvent mre1 = new AutoResetEvent(false);
            AutoResetEvent mre2 = new AutoResetEvent(false);
            int count = SIZE;

            // Call Writer() on SIZE tasks 
            Task[] tasks = new Task[SIZE];
            for (int i = 0; i < SIZE; i++)
            {
                tasks[i] = Task.Run(() => Writer());
            }

            // One task at a time should enter the critical section.  Each time that
            // happens, the task decrements counts, sets mre1, and blocks on mre2.  This
            // loop verifies that the count was decremented and then unblocks the task,
            // which then leaves the critical section allowing the next task in.
            for (int i = SIZE-1; i >= 0; i--)
            {
                mre1.WaitOne();
                Thread.Sleep(100);
                Assert.AreEqual(i, count);
                mre2.Set();
            }

            // Run simultaneously on all tasks
            void Writer()
            {
                rwLock.EnterWriteLock();
                try
                {
                    Interlocked.Decrement(ref count);
                    mre1.Set();
                    mre2.WaitOne();
                }
                finally
                {
                    rwLock.ExitWriteLock();
                }
            }
        }
    }
}
