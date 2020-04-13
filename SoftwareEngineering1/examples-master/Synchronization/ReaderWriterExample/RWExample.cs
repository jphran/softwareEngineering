using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;
using MySynchronization;

namespace ReaderWriterExample
{
    public class RWExample
    {
        private const int LIMIT = 10;
        private static List<int> list = new List<int>();
        private static Random random = new Random();
        private static readonly object sync = new object();
        private static ReaderWriterLockSlim listLock = new ReaderWriterLockSlim();
        private static MyReadWriteLock1 myListLock = new MyReadWriteLock1();

        public static void Main(string[] args)
        {
            list.Add(LIMIT);

            var tasks = new List<Task>();

            for (int i = 0; i < 3 * LIMIT; i += 3)
            {
                int id = i;
                tasks.Add(new Task(() => Display(id)));
                tasks.Add(new Task(() => Display(id + 1)));
                tasks.Add(new Task(() => Insert(id + 2)));
            }

            foreach (Task t in tasks)
            {
                t.Start();
            }

            Task.WaitAll(tasks.ToArray());

            Console.WriteLine(String.Join(", ", list));
        }

        public static void Display(int id)
        {
            Delay();
            //Monitor.Enter(sync);
            listLock.EnterReadLock();
            //myListLock.EnterReadLock();

            try
            {
                Console.WriteLine("Begin read " + id);
                Delay();
                Console.WriteLine(String.Join(", ", list));
                Console.WriteLine("End read " + id);
            }
            finally
            {
                //Monitor.Exit(sync);
                listLock.ExitReadLock();
                //myListLock.ExitReadLock();
            }
        }

        public static void Insert(int id)
        {
            Delay();
            //Monitor.Enter(sync);
            listLock.EnterWriteLock();
            //myListLock.EnterWriteLock();

            try
            {
                Console.WriteLine("Begin write " + id);
                Delay();
                list.Insert(0, list[0] - 1);
                list.Add(list[list.Count - 1] + 1);
                Console.WriteLine("End write " + id);
            }
            finally
            {
                //Monitor.Exit(sync);
                listLock.ExitWriteLock();
                //myListLock.ExitWriteLock();
            }
        }

        public static void Delay()
        {
            Thread.Sleep(random.Next(2000));
        }
    }
}
