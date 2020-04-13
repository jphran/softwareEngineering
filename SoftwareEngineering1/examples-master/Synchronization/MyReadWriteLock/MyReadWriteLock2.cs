using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;

namespace MySynchronization
{
    public class MyReadWriteLock2 : IMyReadWriteLock
    {
        private object sync;
        private object countLock;
        private int readerCount;
        private ManualResetEvent resetEvent;

        public MyReadWriteLock2()
        {
            sync = new object();
            countLock = new object();
            readerCount = 0;
            resetEvent = new ManualResetEvent(false);
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
            lock (countLock)
            {
                if (readerCount == 0)
                {
                    resetEvent.Set();
                }
                else
                {
                    resetEvent.Reset();
                }
            }
            resetEvent.WaitOne();
        }

        public void ExitReadLock()
        {
            lock (countLock)
            {
                if (Interlocked.Decrement(ref readerCount) == 0)
                {
                    resetEvent.Set();
                }
            }
        }

        public void ExitWriteLock()
        {
            Monitor.Exit(sync);
        }
    }
}