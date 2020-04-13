using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;

namespace MySynchronization
{
    public interface IMyReadWriteLock
    {
        void EnterReadLock();

        void EnterWriteLock();

        void ExitReadLock();

        void ExitWriteLock();
    }
}