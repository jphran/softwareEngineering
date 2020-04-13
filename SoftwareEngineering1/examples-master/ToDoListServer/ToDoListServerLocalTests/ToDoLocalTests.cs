using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Http;
using System.Text;
using System.Web.Http;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using ToDoListServer;
using ToDoListServer.Controllers;
using ToDoListServer.Models;

namespace ToDoListServer.Tests.Controllers
{
    [TestClass]
    public class ValuesControllerTest
    {
        [TestMethod]
        public void PostRegisterTest()
        {
            ToDoController controller = new ToDoController();
            UserInfo user = new UserInfo() { Name = "Joe", Email = "a@b.c" };
            string result = controller.PostRegister(user);
            Assert.IsNotNull(result);
            Assert.AreEqual(36, result.Length);

            user.Name = "";
            try
            {
                controller.PostRegister(user);
                Assert.Fail("Exception not found");
            }
            catch (HttpResponseException e)
            {
                Assert.AreEqual(HttpStatusCode.Forbidden, e.Response.StatusCode);
            }
        }
    }
}
