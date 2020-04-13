using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using System.Net.Sockets;
using Chat;
using System.Threading.Tasks;
using System.Text;
using System.Threading;

namespace ChatServerTester
{
    [TestClass]
    public class ChatTester
    {
        private static System.Text.UTF8Encoding encoding = new System.Text.UTF8Encoding();

        [TestMethod]
        public void SimpleTest()
        {
            // Ceate a chat server listening on port 4000
            SimpleChatServer chat = new SimpleChatServer(4000);

            // Send this string and make sure we get the right thing back
            SimpleTestInstance("Test 1\r\nHello world\r\nThis is a test\r\n", 4000);

            // Run 10 tasks that test the server in parallel.
            Task[] tasks = new Task[10];
            for (int i = 0; i < tasks.Length; i++)
            {
                String testString = "Test " + i + "\r\n";
                tasks[i] = Task.Run(() => SimpleTestInstance(testString, 4000));
            }
            Task.WaitAll(tasks);
        }

        [TestMethod]
        public void VolumeTest()
        {
            // Ceate a chat server listening on port 4001
            SimpleChatServer chat = new SimpleChatServer(4001);

            // Number of strings to use
            int COUNT = 1000;

            // Maximum length of strings to use
            int MAX_LENGTH = 1000;

            // Generate LENGTH strings of random length.
            Random rand = new Random();
            StringBuilder sb = new StringBuilder();
            for (int i = 0; i < COUNT; i++)
            {
                int length = rand.Next(MAX_LENGTH);
                for (int j = 0; j < length; j++)
                {
                    sb.Append(rand.Next(10));
                }
                sb.Append("\n");
            }

            SimpleTestInstance(sb.ToString(), 4001);
        }

        private void SimpleTestInstance(string testString, int port)
        {
            // Open a socket to the server
            TcpClient client = new TcpClient("localhost", port);
            Socket socket = client.Client;

            // This is the string we expect to get back
            String expectedString = "Welcome!\r\n" + testString.ToUpper();

            // Convert the string into an array of bytes and send them all out.
            // Note the use of the synchronous Send here.  We can use it because
            // we don't care if the testing thread is blocked for a while.
            byte[] outgoingBuffer = encoding.GetBytes(testString);
            socket.Send(outgoingBuffer);

            // Read bytes from the socket until we have the number we expect.
            // We are using a blocking synchronous Receive here.
            byte[] incomingBuffer = new byte[encoding.GetByteCount(expectedString)];
            int index = 0;
            while (index < incomingBuffer.Length)
            {
                index += socket.Receive(incomingBuffer, index, incomingBuffer.Length - index, 0);
            }

            // Convert the buffer into a string and make sure it is what was expected
            String result = encoding.GetString(incomingBuffer);
            Assert.AreEqual(expectedString, result);
        }

        private void BetterTestInstance(string testString, int port)
        {
            // Open a socket to the server
            TcpClient client = new TcpClient("localhost", port);
            Socket socket = client.Client;

            // This is the string we expect to get back
            String expectedString = "Welcome!\r\n" + testString.ToUpper();

            // Convert the string into an array of bytes and send them all out.
            // Note the use of the synchronous Send here.  We can use it because
            // we don't care if the testing thread is blocked for a while.
            Task t1 = Task.Run(() =>
            {
                byte[] outgoingBuffer = encoding.GetBytes(testString);
                socket.Send(outgoingBuffer);
            });

            // Read bytes from the socket until we have the number we expect.
            // We are using a blocking synchronous Receive here.
            byte[] incomingBuffer = null;
            Task t2 = Task.Run(() =>
            {
                incomingBuffer = new byte[encoding.GetByteCount(expectedString)];
                int index = 0;
                while (index < incomingBuffer.Length)
                {
                    index += socket.Receive(incomingBuffer, index, incomingBuffer.Length - index, 0);
                }
            });

            // Convert the buffer into a string and make sure it is what was expected
            if (!t1.Wait(2000))
            {
                Assert.Fail();
            }

            if (!t2.Wait(2000))
            {
                Assert.Fail();
            }

            String result = encoding.GetString(incomingBuffer);
            Assert.AreEqual(expectedString, result);
        }

        private void AnotherTestInstance(string testString, int port)
        {
            // Open a socket to the server
            TcpClient client = new TcpClient("localhost", port);
            Socket socket = client.Client;

            // This is the string we expect to get back
            String expectedString = "Welcome!\r\n" + testString.ToUpper();

            // Convert the string into an array of bytes and send them all out,
            // sending a random number of bytes at a time.
            // Note the use of the synchronous Send here.  We can use it because
            // we don't care if the testing thread is blocked for a while.
            Task t1 = Task.Run(() =>
            {
                Random rand = new Random();
                byte[] outgoingBuffer = encoding.GetBytes(testString);
                int index = 0;
                while (index < outgoingBuffer.Length)
                {
                    int size = rand.Next(outgoingBuffer.Length - index - 1) + 1;
                    index += socket.Send(outgoingBuffer, index, size, SocketFlags.None);
                }
            });

            // Read bytes from the socket until we have the number we expect.
            // We are using a blocking synchronous Receive here.
            byte[] incomingBuffer = null;
            Task t2 = Task.Run(() =>
            {
                incomingBuffer = new byte[encoding.GetByteCount(expectedString)];
                int index = 0;
                while (index < incomingBuffer.Length)
                {
                    index += socket.Receive(incomingBuffer, index, incomingBuffer.Length - index, 0);
                }
            });

            // Convert the buffer into a string and make sure it is what was expected
            if (!t1.Wait(2000))
            {
                Assert.Fail();
            }

            if (!t2.Wait(2000))
            {
                Assert.Fail();
            }

            String result = encoding.GetString(incomingBuffer);
            Assert.AreEqual(expectedString, result);
        }
    }
}
