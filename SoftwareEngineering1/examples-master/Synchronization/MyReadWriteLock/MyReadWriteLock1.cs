using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;

namespace MySynchronization
{
    public class MyReadWriteLock1 : IMyReadWriteLock
    {
        private object sync;
        private int readerCount;

        public MyReadWriteLock1()
        {
            sync = new object();
            readerCount = 0;
        }


        public void EnterReadLock()
        {
            lock (sync)
            {
                Interlocked.Increment(ref readerCount);
            }
        }


        public void EnterWriteLock()
        {
            Monitor.Enter(sync);
            SpinWait.SpinUntil(() => readerCount == 0);
        }

        public void ExitReadLock()
        {
            Interlocked.Decrement(ref readerCount);
        }

        public void ExitWriteLock()
        {
            Monitor.Exit(sync);
        }
    }
}