using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using SpreadsheetGUI;

namespace ControllerTester
{
    [TestClass]
    public class UnitTest1
    {

        [TestMethod]
        public void TestMethod1()
        {
            TestingStub stub = new TestingStub();
            Controller controller = new Controller(stub);

            stub.FireSetCellContents("A1", "10");
            Assert.IsTrue(stub.ssContains("A1", "10"));
        }

        [TestMethod]
        //[Timeout(3000)]
        public void FindStackOverFlow()
        {
            TestingStub stub = new TestingStub();
            Controller controller = new Controller(stub);

            stub.FireSetCellContents("A1", "=A10");
            stub.FireSetCellContents("A10", "10");

            Assert.IsTrue(stub.ssContains("A1", "10"));
        }

        [TestMethod]
        //[Timeout(3000)]
        public void TestCloseEvent()
        {
            TestingStub stub = new TestingStub();
            Controller controller = new Controller(stub);

            stub.FireSetCellContents("a1", "0");
            stub.FireCloseEvent();

            Assert.IsTrue((bool)stub.InteractionMsgBox[0]);
        }

        [TestMethod]
        //[Timeout(3000)]
        public void TestSaveAsEvent()
        {
            TestingStub stub = new TestingStub();
            Controller controller = new Controller(stub);

            stub.FireSaveAsEvent("testStubSaveAs");

            Assert.AreEqual("testStubSaveAs.ss", stub.Title);
            Assert.AreEqual("testStubSaveAs.ss", stub.PreviousSaveName);
        }

        [TestMethod]
        //[Timeout(3000)]
        public void TestSaveSpreadsheetEvent()
        {
            TestingStub stub = new TestingStub();
            Controller controller = new Controller(stub);

            stub.FireSaveSpreadsheetEvent();

            Assert.AreEqual("untitledSpreadsheet1.ss", stub.Title);
            Assert.AreEqual("untitledSpreadsheet1.ss", stub.PreviousSaveName);
        }

        [TestMethod]
        //[Timeout(3000)]
        public void TestOpenNewEvent()
        {
            TestingStub stub = new TestingStub();
            Controller controller = new Controller(stub);

           stub.FireOpenNewEvent(); //TODO: uncomment and replace untitlteSS1.ss with file path
            Assert.IsTrue(stub.DidOpenNew);
           //Assert.AreEqual("untitledSpreadsheet1.ss", stub.PathOpenedFrom);
        }

        [TestMethod]
        //[Timeout(3000)]
        public void TestOpenSpreadsheetEvent()
        {
            TestingStub stub = new TestingStub();
            Controller controller = new Controller(stub);

             stub.FireOpenSpreadsheetEvent(); //TODO: uncomment and replace untitlteSS1.ss with file path
             Assert.AreEqual("testingSpreadsheet.ss", stub.PathOpenedFrom);
        }

        [TestMethod]
        //[Timeout(3000)]
        public void TestGetCellContentsEvent()
        {
            TestingStub stub = new TestingStub();
            Controller controller = new Controller(stub);
            stub.FireSetCellContents("A1", "10");
            stub.FireSetCellContents("c4", "hello there");
            stub.FireSetCellContents("b32", "=c4");
            stub.FireSetCellContents("A2", "=A1");

            stub.FireGetCellContentsEvent("a1");
            Assert.AreEqual("10", stub.FormulaEditBox);
            stub.FireGetCellContentsEvent("c4");
            Assert.AreEqual("hello there", stub.FormulaEditBox);
            stub.FireGetCellContentsEvent("b32");
            Assert.AreEqual("=C4", stub.FormulaEditBox);
            stub.FireGetCellContentsEvent("a2");
            Assert.AreEqual("=A1", stub.FormulaEditBox);
        }

        [TestMethod]
        //[Timeout(3000)]
        public void TestCircularDep()
        {
            TestingStub stub = new TestingStub();
            Controller controller = new Controller(stub);

            stub.FireSetCellContents("A1", "=a1");

            Assert.IsTrue(stub.ssContains("A1", "Error, Circular Formula"));
        }

        [TestMethod]
        //[Timeout(3000)]
        public void TestGetValueEvent()
        {
            TestingStub stub = new TestingStub();
            Controller controller = new Controller(stub);
            stub.FireSetCellContents("A1", "touche");
            stub.FireSetCellContents("b2", "");
            
            stub.FireGetCellValue("A1");

            Assert.AreEqual("touche", stub.FormulaEditBox);
        }

        [TestMethod]
        //[Timeout(3000)]
        public void TestHelpMenuEvent()
        {
            TestingStub stub = new TestingStub();
            Controller controller = new Controller(stub);

            stub.FireHelpMenu();
        }
    }
}
