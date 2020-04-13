using System;
using System.Text;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using Rest;
using System.Dynamic;
using static System.Net.HttpStatusCode;
using static System.Net.Http.HttpMethod;
using System.Diagnostics;

namespace ToDoListServerTests
{
    /// <summary>
    /// Provides a way to start and stop the IIS web server from within the test
    /// cases.  If something prevents the test cases from stopping the web server,
    /// subsequent tests may not work properly until the stray process is killed
    /// manually.
    /// 
    /// Starting the service this way allows code coverage statistics for the service
    /// to be collected.
    /// </summary>
    public static class IISAgent
    {
        // Reference to the running process
        private static Process process = null;

        /// <summary>
        /// Starts IIS
        /// </summary>
        public static void Start(string arguments)
        {
            if (process == null)
            {
                ProcessStartInfo info = new ProcessStartInfo(Properties.Resources.IIS_EXECUTABLE, arguments);
                info.WindowStyle = ProcessWindowStyle.Minimized;
                info.UseShellExecute = false;
                process = Process.Start(info);
            }
        }

        /// <summary>
        ///  Stops IIS
        /// </summary>
        public static void Stop()
        {
            if (process != null)
            {
                process.Kill();
            }
        }
    }

    [TestClass]
    public class ToDoTests
    {
        /// <summary>
        /// This is automatically run prior to all the tests to start the server.
        /// </summary>
        // Remove the comment before the annotation to make it work
        //[ClassInitialize]
        public static void StartIIS(TestContext testContext)
        {
            IISAgent.Start(@"/site:""ToDoListServer"" /apppool:""Clr4IntegratedAppPool"" /config:""..\..\..\.vs\config\applicationhost.config""");
        }

        /// <summary>
        /// This is automatically run when all tests have completed to stop the server
        /// </summary>
        // Remove the comment before the annotation to make it work
        //[ClassCleanup]
        public static void StopIIS()
        {
            IISAgent.Stop();
        }

        private RestClient client = new RestClient("http://localhost:44444/ToDo/");

        [TestMethod]
        public void TestMethod1()
        {
            dynamic user = new ExpandoObject();
            user.Name = "Joe";
            user.Email = "email";
            Response r = client.DoMethodAsync("POST", "RegisterUser", user).Result;
            Assert.AreEqual(OK, r.Status);
            Assert.AreEqual(36, r.Data.Length);

            user.Name = "";
            r = client.DoMethodAsync("POST", "RegisterUser", user).Result;
            Assert.AreEqual(Forbidden, r.Status);
        }

        [TestMethod]
        public void TestMethod2()
        {
            dynamic user = new ExpandoObject();
            user.Name = "Joe";
            user.Email = "email";
            Response r = client.DoMethodAsync("POST", "RegisterUser", user).Result;
            Assert.AreEqual(OK, r.Status);
            string userID = r.Data.ToString();

            dynamic item = new ExpandoObject();
            item.UserID = userID;
            item.Description = "Description 1";
            r = client.DoMethodAsync("POST", "AddItem", item).Result;
            Assert.AreEqual(OK, r.Status);

            item.UserID = null;
            r = client.DoMethodAsync("POST", "AddItem", item).Result;
            Assert.AreEqual(Forbidden, r.Status);

            item.UserID = "missing";
            r = client.DoMethodAsync("POST", "AddItem", item).Result;
            Assert.AreEqual(Forbidden, r.Status);
        }

        [TestMethod]
        public void TestMethod3()
        {
            dynamic user = new ExpandoObject();
            user.Name = "Joe";
            user.Email = "email";
            Response r = client.DoMethodAsync("POST", "RegisterUser", user).Result;
            Assert.AreEqual(OK, r.Status);
            string userID = r.Data.ToString();

            dynamic item = new ExpandoObject();
            item.UserID = userID;
            item.Description = "Description 1";
            item.Completed = false;
            r = client.DoMethodAsync("POST", "AddItem", item).Result;
            Assert.AreEqual(OK, r.Status);
            string itemID = r.Data.ToString();

            r = client.DoMethodAsync("PUT", "MarkCompleted/" + itemID, item).Result;
            Assert.AreEqual(NoContent, r.Status);

            r = client.DoMethodAsync("PUT", "MarkCompleted/" + "missing", item).Result;
            Assert.AreEqual(Forbidden, r.Status);
        }

        [TestMethod]
        public void TestMethod4()
        {
            dynamic user = new ExpandoObject();
            user.Name = "Joe";
            user.Email = "email";
            Response r = client.DoMethodAsync("POST", "RegisterUser", user).Result;
            Assert.AreEqual(OK, r.Status);
            string userID = r.Data.ToString();

            dynamic item = new ExpandoObject();
            item.UserID = userID;
            item.Description = "Description 1";
            item.Completed = false;
            r = client.DoMethodAsync("POST", "AddItem", item).Result;
            Assert.AreEqual(OK, r.Status);
            string itemID = r.Data.ToString();

            r = client.DoMethodAsync("DELETE", "DeleteItem/" + itemID).Result;
            Assert.AreEqual(NoContent, r.Status);

            r = client.DoMethodAsync("DELETE", "DeleteItem/" + itemID).Result;
            Assert.AreEqual(Forbidden, r.Status);

            r = client.DoMethodAsync("DELETE", "DeleteItem/" + "missing").Result;
            Assert.AreEqual(Forbidden, r.Status);
        }

        [TestMethod]
        public void TestMethod5()
        {
            dynamic user = new ExpandoObject();
            user.Name = "Joe";
            user.Email = "email";
            Response r = client.DoMethodAsync("POST", "RegisterUser", user).Result;
            Assert.AreEqual(OK, r.Status);
            string userID = r.Data.ToString();

            dynamic item = new ExpandoObject();
            item.UserID = userID;
            item.Description = "Description 1";
            item.Completed = false;
            r = client.DoMethodAsync("POST", "AddItem", item).Result;
            Assert.AreEqual(OK, r.Status);
            string itemID1 = r.Data.ToString();

            item = new ExpandoObject();
            item.UserID = userID;
            item.Description = "Description 2";
            item.Completed = false;
            r = client.DoMethodAsync("POST", "AddItem", item).Result;
            Assert.AreEqual(OK, r.Status);
            string itemID2 = r.Data.ToString();

            item = new ExpandoObject();
            item.UserID = userID;
            item.Description = "Description 3";
            item.Completed = false;
            r = client.DoMethodAsync("POST", "AddItem", item).Result;
            Assert.AreEqual(OK, r.Status);
            string itemID3 = r.Data.ToString();

            r = client.DoMethodAsync("PUT", "MarkCompleted/" + itemID1, item).Result;
            Assert.AreEqual(NoContent, r.Status);

            r = client.DoMethodAsync("DELETE", "DeleteItem/" + itemID3).Result;
            Assert.AreEqual(NoContent, r.Status);

            r = client.DoMethodAsync("GET", "GetAllItems/false/missing").Result;
            Assert.AreEqual(Forbidden, r.Status);

            r = client.DoMethodAsync("GET", "GetAllItems/false/" + userID).Result;
            Assert.AreEqual(OK, r.Status);
            Assert.AreEqual(2, r.Data.Count);

            r = client.DoMethodAsync("GET", "GetAllItems/true/" + userID).Result;
            Assert.AreEqual(OK, r.Status);
            Assert.AreEqual(1, r.Data.Count);
        }
    }
}

