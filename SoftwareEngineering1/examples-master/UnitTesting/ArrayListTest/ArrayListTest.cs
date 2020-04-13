// Written by Joe Zachary for CS 3500, January 2017
using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using UnitTestDemo;

namespace ArrayListTest
{
    /// <summary>
    /// Tests for the ArrayList class
    /// </summary>
    [TestClass]
    public class ArrayListTest
    {
        /// <summary>
        /// Tries to index into an empty ArrayList.
        /// Implemented by catching exception.
        /// </summary>
        [TestMethod]
        public void EmptyTest1()
        {
            try
            {
                ArrayList list = new ArrayList();
                list.Get(0);
                Assert.Fail();
            }
            catch (IndexOutOfRangeException e)
            {
                // An exception is expected
            }
        }

        /// <summary>
        /// Tries to index into an empty ArrayList.
        /// Implemented with ExpectedException annotation.
        /// </summary>
        [TestMethod]
        [ExpectedException(typeof(IndexOutOfRangeException))]
        public void EmptyTest2()
        {
            ArrayList list = new ArrayList();
            list.Get(0);
        }

        /// <summary>
        /// Tests a one-element array
        /// </summary>
        [TestMethod]
        public void SmallTest()
        {
            ArrayList list = new ArrayList();
            list.AddLast("10");
            Assert.AreEqual("10", list.Get(0));
            Assert.AreEqual(1, list.GetSize());
            try
            {
                list.Get(1);
                Assert.Fail();
            }
            catch (IndexOutOfRangeException)
            {
                // An exception is expected
            }
        }

        /// <summary>
        /// This shows how to directly test a private method.  I generally
        /// prefer to test private methods indirectly by invoking public
        /// methods, but this technique sometimes has its place.
        /// 
        /// </summary>
        [TestMethod]
        public void PrivateTest()
        {
            // Create an array element with three elements
            ArrayList list = new ArrayList();
            list.AddLast("1");
            list.AddLast("2");
            list.AddLast("3");

            // Invoke the private method Scale
            PrivateObject listAccessor = new PrivateObject(list);
            object[] parameters = { 3 };
            int n = (int) listAccessor.Invoke("Scale", parameters);
            Assert.AreEqual(12, n);
        }
    }
}
