using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AsyncDemo
{
    public class Program
    {
        // This version blocks while the page is being read
        public static string Version1(string url)
        {
            // Do some stuff

            // Read a file into a string
            string s = ReadPageContents(url);

            // Do some stuff that doesn't involve s

            // Do something involving s

            // Do some more stuff

            // Return the string
            return s;
        }

        // This also blocks, but not immediately
        public static string Version2(string url)
        {
            // Do some stuff

            // Read a file into a string using a different task
            Task<string> readTask = Task<string>.Run(() => ReadPageContents(url));

            // Do some stuff that doesn't involve s

            // Obtain the string from the task
            string s = readTask.Result;

            // Do something involving s

            // Do some more stuff

            // Return the string
            return s;
        }

        // This returns a task and lets the user deal with it.  However, most
        // of the method ends up running on a different thread, which can
        // cause synchronization problems.
        public static Task<string> Version3(string url)
        {
            // Do some stuff

            // Read a file into a string and deal with it using a different task
            Task<string> readTask = Task<string>.Run(() =>
                {
                    string s = ReadPageContents(url);

                    // Do some stuff that doesn't involve s

                    // Do something involving s

                    // Do some more stuff

                    // Return the string
                    return s;
                });

            return readTask;
        }

        // This uses async/await to do what was done in the first example,
        // but in a way that ensures everything runs on the same thread.
        // It's also much simpler.
        public async static Task<string> Version4(string url)
        {
            // Do some stuff

            // Read a file into a string
            Task<string> readTask = ReadPageContentsAsync(url);

            // Do some stuff that doesn't involve s

            // Obtain s by awaiting readTask
            string s = await readTask;

            // Do something involving s

            // Do some more stuff

            // Return the string
            return s;
        }


        public static string ReadPageContents(string url)
        {
            return null;
        }

        public static Task<string> ReadPageContentsAsync(string url)
        {
            return null;
        }
    }
}
