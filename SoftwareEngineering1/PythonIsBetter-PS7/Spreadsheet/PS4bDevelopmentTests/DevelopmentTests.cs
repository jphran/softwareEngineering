using Microsoft.VisualStudio.TestTools.UnitTesting;
using Dependencies;
using System.Collections.Generic;
using System;

namespace PS4DevelopmentTests
{
    [TestClass]
    public class DevelopmentTests
    {
        [TestMethod]
        [ExpectedException(typeof(ArgumentNullException))]
        public void Null1()
        {
            DependencyGraph d = new DependencyGraph();
            d.AddDependency("a", null);
        }

        [TestMethod]
        public void Copy1()
        {
            var d1 = new DependencyGraph();
            var d2 = new DependencyGraph(d1);
            Assert.AreEqual(0, d1.Size);
            Assert.AreEqual(0, d2.Size);
        }

        [TestMethod]
        public void Copy5()
        {
            var d1 = new DependencyGraph();
            d1.AddDependency("a", "b");
            d1.AddDependency("d", "e");
            var d2 = new DependencyGraph(d1);
            d1.AddDependency("a", "c");
            d2.AddDependency("d", "f");
            Assert.AreEqual(2, new List<string>(d1.GetDependents("a")).Count);
            Assert.AreEqual(1, new List<string>(d1.GetDependents("d")).Count);
            Assert.AreEqual(2, new List<string>(d2.GetDependents("d")).Count);
            Assert.AreEqual(1, new List<string>(d2.GetDependents("a")).Count);
        }

        [TestMethod]
        [ExpectedException(typeof(ArgumentNullException))]
        public void NullTest1()
        {
            DependencyGraph d = new DependencyGraph();
            d.AddDependency("a", null);
        }

        [TestMethod]
        [ExpectedException(typeof(ArgumentNullException))]
        public void NullTest2()
        {
            DependencyGraph d = new DependencyGraph();
            d.HasDependents(null);
        }

        [TestMethod]
        [ExpectedException(typeof(ArgumentNullException))]
        public void NullTest3()
        {
            DependencyGraph d = new DependencyGraph();
            d.HasDependees(null);
        }


        [TestMethod]
        [ExpectedException(typeof(ArgumentNullException))]
        public void NullTest4()
        {
            DependencyGraph d = new DependencyGraph();
            d.GetDependents(null);
        }


        [TestMethod]
        [ExpectedException(typeof(ArgumentNullException))]
        public void NullTest5()
        {
            DependencyGraph d = new DependencyGraph();
            d.GetDependees(null);
        }


        [TestMethod]
        [ExpectedException(typeof(ArgumentNullException))]
        public void NullTest6()
        {
            DependencyGraph d = new DependencyGraph();
            d.GetDependees(null);
        }

        [TestMethod]
        [ExpectedException(typeof(ArgumentNullException))]
        public void NullTest7()
        {
            DependencyGraph d = new DependencyGraph();
            d.AddDependency(null, null);
        }

        [TestMethod]
        [ExpectedException(typeof(ArgumentNullException))]
        public void NullTest8()
        {
            DependencyGraph d = new DependencyGraph();
            d.AddDependency("test", null);
        }

        [TestMethod]
        [ExpectedException(typeof(ArgumentNullException))]
        public void NullTest9()
        {
            DependencyGraph d = new DependencyGraph();
            d.AddDependency(null, "test");
        }

        [TestMethod]
        [ExpectedException(typeof(ArgumentNullException))]
        public void NullTest10()
        {
            DependencyGraph d = new DependencyGraph();
            d.RemoveDependency(null, null);
        }

        [TestMethod]
        [ExpectedException(typeof(ArgumentNullException))]
        public void NullTest11()
        {
            DependencyGraph d = new DependencyGraph();
            d.RemoveDependency("test", null);
        }

        [TestMethod]
        [ExpectedException(typeof(ArgumentNullException))]
        public void NullTest12()
        {
            DependencyGraph d = new DependencyGraph();
            d.RemoveDependency(null, "test");
        }

        [TestMethod]
        [ExpectedException(typeof(ArgumentNullException))]
        public void NullTest13()
        {
            DependencyGraph d = new DependencyGraph();
            d.ReplaceDependents(null, null);
        }

        [TestMethod]
        [ExpectedException(typeof(ArgumentNullException))]
        public void NullTest14()
        {
            DependencyGraph d = new DependencyGraph();
            d.ReplaceDependents("test", null);
        }

        [TestMethod]
        [ExpectedException(typeof(ArgumentNullException))]
        public void NullTest15()
        {

            List<string> x = new List<string>();

            DependencyGraph d = new DependencyGraph();
            d.ReplaceDependents(null, x);
        }

        [TestMethod]
        [ExpectedException(typeof(ArgumentNullException))]
        public void NullTest16()
        {
            DependencyGraph d = new DependencyGraph();
            d.ReplaceDependees(null, null);
        }

        [TestMethod]
        [ExpectedException(typeof(ArgumentNullException))]
        public void NullTest17()
        {
            DependencyGraph d = new DependencyGraph();
            d.ReplaceDependees("test", null);
        }

        [TestMethod]
        [ExpectedException(typeof(ArgumentNullException))]
        public void NullTest18()
        {

            List<string> x = new List<string>();

            DependencyGraph d = new DependencyGraph();
            d.ReplaceDependees(null, x);
        }

        [TestMethod]
        [ExpectedException(typeof(ArgumentNullException))]
        public void NullTest19()
        {
            DependencyGraph x = null;
            DependencyGraph y = new DependencyGraph(x);
        }


        [TestMethod]
        public void TestDoesNotContainsDependee()
        {
            DependencyGraph x = new DependencyGraph();
            Assert.IsFalse(x.HasDependees("x"));
        }

    }

}
