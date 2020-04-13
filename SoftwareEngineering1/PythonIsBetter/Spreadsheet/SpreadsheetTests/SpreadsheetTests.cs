using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using SS;
using Formulas;
using System.Collections.Generic;
using System.IO;
using System.Text.RegularExpressions;

namespace SpreadsheetTests
{
    [TestClass]
    public class SpreadsheetTests
    {
        [TestMethod]
        [ExpectedException(typeof(InvalidNameException))]
        public void InvalidName1()
        {
            AbstractSpreadsheet s = new Spreadsheet();
            var x = s.SetContentsOfCell("a", "1.0");
        }

        [TestMethod]
        [ExpectedException(typeof(InvalidNameException))]
        public void InvalidName2()
        {
            AbstractSpreadsheet s = new Spreadsheet();
            var x = s.SetContentsOfCell("a", "test String");
        }

        [TestMethod]
        [ExpectedException(typeof(ArgumentNullException))]
        public void NullArgument()
        {
            AbstractSpreadsheet s = new Spreadsheet();
            string y = null;
            var x = s.SetContentsOfCell("a1", y);
        }

        [TestMethod]
        [ExpectedException(typeof(InvalidNameException))]
        public void InvalidName3()
        {
            AbstractSpreadsheet s = new Spreadsheet();
            var x = s.SetContentsOfCell("a", "=x1 + y1");
        }

        [TestMethod]
        [ExpectedException(typeof(InvalidNameException))]
        public void InvalidName4()
        {
            AbstractSpreadsheet s = new Spreadsheet();
            var x = s.SetContentsOfCell("a0", "1.0");
        }

        [TestMethod]
        [ExpectedException(typeof(InvalidNameException))]
        public void InvalidName5()
        {
            AbstractSpreadsheet s = new Spreadsheet();
            var x = s.SetContentsOfCell("a0", "test String");
        }

        [TestMethod]
        [ExpectedException(typeof(InvalidNameException))]
        public void InvalidName6()
        {
            AbstractSpreadsheet s = new Spreadsheet();
            var x = s.SetContentsOfCell("a0", "=x1 + y1");
        }


        [TestMethod]
        [ExpectedException(typeof(InvalidNameException))]
        public void InvalidName7()
        {
            AbstractSpreadsheet s = new Spreadsheet();
            var x = s.SetContentsOfCell("6", "1.0");
        }

        [TestMethod]
        [ExpectedException(typeof(InvalidNameException))]
        public void InvalidName8()
        {
            AbstractSpreadsheet s = new Spreadsheet();
            var x = s.SetContentsOfCell("6", "test String");
        }

        [TestMethod]
        [ExpectedException(typeof(InvalidNameException))]
        public void InvalidName9()
        {
            AbstractSpreadsheet s = new Spreadsheet();
<<<<<<< HEAD
            var x = s.SetContentsOfCell("9", "=x1 + y1");
=======
            s.SetContentsOfCell("9", "=x1 + y1");
>>>>>>> PS6_Work
        }

        [TestMethod]
        public void InvalidFormulaVariable()
        {
            // Exception should come from invalid var x in the formula
            AbstractSpreadsheet s = new Spreadsheet();
<<<<<<< HEAD
            var x = s.SetContentsOfCell("a9", "=x + 1");
=======
            s.SetContentsOfCell("a9", "=x + 1");
            Assert.IsInstanceOfType(s.GetCellValue("a9"), typeof(FormulaError));
        }

        [TestMethod]
        [ExpectedException(typeof(InvalidNameException))]
        public void InvalidFormulaVariable2()
        {
            // Exception should come from invalid var x in the formula
            AbstractSpreadsheet s = new Spreadsheet();
            s.SetContentsOfCell("9", "=x + 1");
            Assert.IsInstanceOfType(s.GetCellValue("9"), typeof(FormulaError));
>>>>>>> PS6_Work
        }

        [TestMethod]
        public void testGetContents()
        {
            AbstractSpreadsheet s = new Spreadsheet();

            // Check a bunch of empty cells
            for (int i = 1; i < 100; i++)
            {
                Assert.AreEqual("", s.GetCellContents("=a" + i));
            }

            // Add number as content to bunch of cells
            for (int i = 1; i < 100; i++)
            {
                double x = i + 0.5;
                string xString = "" + x;

                s.SetContentsOfCell("a" + i, xString);
            }
            // check value of cells with numbers
            for (int i = 1; i < 100; i++)
            {
                double x = i + 0.5;

                Assert.AreEqual(x, s.GetCellContents("a" + i));
            }


            // Add string as content to bunch of cells
            for (int i = 1; i < 100; i++)
            {
                string str = "b" + i;
                s.SetContentsOfCell(str, str);
            }
            // check value of cells with numbers
            for (int i = 1; i < 100; i++)
            {
                string str = "b" + i;
                Assert.AreEqual(str, s.GetCellContents(str));
            }

            // Add formula as content to bunch of cells
            for (int i = 1; i < 100; i++)
            {
                string str = "c" + i;
                s.SetContentsOfCell(str, "1");
            }
            // check value of cells with numbers
            for (int i = 1; i < 100; i++)
            {
                string str = "c" + i;
                Formula f = new Formula("=1");
                Assert.AreEqual("1", s.GetCellContents(str).ToString());
            }

        }


        [TestMethod]
        public void changeVaueNum()
        {
            // Exception should come from invalid var x in the formula
            AbstractSpreadsheet s = new Spreadsheet();

            double x = 0.5;
            for (int i = 0; i < 100; i++)
            {
                string xString = "" + x;

                s.SetContentsOfCell("a1", xString);
                Assert.AreEqual(x, s.GetCellContents("=a1"));

                x = x + 0.3;
            }
        }

        [TestMethod]
        public void changeVaueStr()
        {
            // Exception should come from invalid var x in the formula
            AbstractSpreadsheet s = new Spreadsheet();

            string x = "x";
            for (int i = 0; i < 100; i++)
            {
                s.SetContentsOfCell("a1", x);
                Assert.AreEqual(x, s.GetCellContents("=a1"));

                x = x + "0";
            }
        }

        [TestMethod]
        public void changeVaueFormula()
        {
            // Exception should come from invalid var x in the formula
            AbstractSpreadsheet s = new Spreadsheet();

            string x = "x1";
            Formula f = new Formula(x);
            for (int i = 0; i < 100; i++)
            {
                s.SetContentsOfCell("a1", x);
                Assert.AreEqual(f, s.GetCellContents("=a1"));

                x = x + " + x1";
                f = new Formula(x);
            }
        }

        [TestMethod]
        public void testGetAllNonEmptyCells()
        {
            AbstractSpreadsheet s = new Spreadsheet();
            s.SetContentsOfCell("a1", "test");
            s.SetContentsOfCell("a2", "test");
            s.SetContentsOfCell("a3", "test");
            s.SetContentsOfCell("a4", "test");
            s.SetContentsOfCell("a5", "test");
            s.SetContentsOfCell("a6", "test");

            List<string> x = new List<string>();
            x.AddRange(s.GetNamesOfAllNonemptyCells());

            Assert.IsTrue(x.Contains("a1"));
            Assert.IsTrue(x.Contains("a2"));
            Assert.IsTrue(x.Contains("a3"));
            Assert.IsTrue(x.Contains("a4"));
            Assert.IsTrue(x.Contains("a5"));
            Assert.IsTrue(x.Contains("a6"));
            Assert.AreEqual(6, x.Count);

        }

        [TestMethod]
        public void testGetAllNonEmptyCellsWithManuallySetEmptyCell()
        {
            AbstractSpreadsheet s = new Spreadsheet();
            s.SetContentsOfCell("a1", "test");
            s.SetContentsOfCell("a2", "");
            s.SetContentsOfCell("a3", "x");
            s.SetContentsOfCell("a3", "");
            s.SetContentsOfCell("a4", "test2");

            List<string> x = new List<string>();
            x.AddRange(s.GetNamesOfAllNonemptyCells());

            Assert.IsTrue(x.Contains("a1"));
            Assert.IsTrue(x.Contains("a4"));
            Assert.IsFalse(x.Contains("a3"));
            Assert.IsFalse(x.Contains("a2"));
            Assert.AreEqual(2, x.Count);
        }

<<<<<<< HEAD
        
=======

>>>>>>> PS6_Work
        [TestMethod()]
        [ExpectedException(typeof(CircularException))]
        public void TestCircularDependency()
        {
            AbstractSpreadsheet s = new Spreadsheet();

            s.SetContentsOfCell("f1", "=a1 + a2");
            s.SetContentsOfCell("a1", "=b1 + b2");
            s.SetContentsOfCell("a2", "=c1 + c2");
            s.SetContentsOfCell("b1", "=d1 + d2");
            s.SetContentsOfCell("b2", "=1 + 2");
            s.SetContentsOfCell("c1", "=1 + 2");
            s.SetContentsOfCell("c2", "=1 + 2");
            s.SetContentsOfCell("d1", "=1 + 2");
            s.SetContentsOfCell("d2", "=f1 + 2");
<<<<<<< HEAD
=======
            Console.WriteLine(s.GetCellValue("d2"));
>>>>>>> PS6_Work

        }

        [TestMethod()]
        [ExpectedException(typeof(CircularException))]
        public void CircularTest2()
        {
            AbstractSpreadsheet s = new Spreadsheet();
            s.SetContentsOfCell("A1", "=A1");
        }

        [TestMethod()]
        [ExpectedException(typeof(CircularException))]
        public void CircularTest3()
        {
            AbstractSpreadsheet s = new Spreadsheet();
            s.SetContentsOfCell("A1", "=A2");
            s.SetContentsOfCell("A2", "=A3");
            s.SetContentsOfCell("A3", "=A4");
            s.SetContentsOfCell("A4", "=A5");
            s.SetContentsOfCell("A5", "=A6");
            s.SetContentsOfCell("A6", "=A1");
        }

        [TestMethod()]
        public void testSetReturn()
        {
            AbstractSpreadsheet s = new Spreadsheet();

            s.SetContentsOfCell("B1", "=A1 * 2");
            s.SetContentsOfCell("C1", "=A1 + B1");
            s.SetContentsOfCell("D1", "=A1 + B1");

            var set = s.SetContentsOfCell("A1", "7");
<<<<<<< HEAD

            foreach (string x in set)
            {
                Console.WriteLine(x);
            }
=======
>>>>>>> PS6_Work

            Assert.IsTrue(set.Contains("A1"));
            Assert.IsTrue(set.Contains("B1"));
            Assert.IsTrue(set.Contains("C1"));
            Assert.IsTrue(set.Contains("D1"));

        }

        [TestMethod()]
        public void testReplaceWithEmptyString()
        {
            AbstractSpreadsheet s = new Spreadsheet();

            Assert.IsTrue(s.GetCellContents("A1").Equals(""));

            var x = s.SetContentsOfCell("A1", "test");
            Assert.IsTrue(x.Contains("A1"));
            Assert.IsTrue(x.Count == 1);
            Assert.IsTrue(s.GetCellContents("A1").Equals("test"));

            var y = s.SetContentsOfCell("A1", "");
            Assert.IsTrue(y.Contains("A1"));
            Assert.IsTrue(y.Count == 1);
            Assert.IsTrue(s.GetCellContents("A1").Equals(""));
        }



        // For setting a spreadsheet cell.
        [TestMethod()]
        public void DivisionByZero1()
        {
            AbstractSpreadsheet ss = new Spreadsheet();
            ss.SetContentsOfCell("A1", "0");
            ss.SetContentsOfCell("A2", "4.3");
            ss.SetContentsOfCell("A3", "=A2 / A1");
            Assert.IsInstanceOfType(ss.GetCellValue("A3"), typeof(FormulaError));

        }


        // New test specifically for PS6
        [TestMethod()]
        public void test1()
        {
            AbstractSpreadsheet ss = new Spreadsheet();

            ss.SetContentsOfCell("A1", "1.0");
            ss.SetContentsOfCell("A2", "1.5");
            ss.SetContentsOfCell("A3", "= A1 + A2");
            Assert.AreEqual(2.5, ss.GetCellValue("A3"));

            ss.SetContentsOfCell("B1", "test");
            Assert.AreEqual("test", ss.GetCellValue("B1"));

            ss.SetContentsOfCell("C1", "1.4");
            Assert.AreEqual(1.4, ss.GetCellValue("C1"));

            Assert.AreEqual("", ss.GetCellValue("E1"));
            ss.SetContentsOfCell("E1", "x");
            Assert.AreEqual("x", ss.GetCellValue("E1"));
        }

        [TestMethod()]
        public void test2()
        {
            AbstractSpreadsheet ss = new Spreadsheet();

            ss.SetContentsOfCell("a1", "1.0");
            ss.SetContentsOfCell("a2", "1.5");
            ss.SetContentsOfCell("a3", "= a1 + a2");
            Assert.AreEqual(2.5, ss.GetCellValue("A3"));

            ss.SetContentsOfCell("b1", "test");
            Assert.AreEqual("test", ss.GetCellValue("B1"));

            ss.SetContentsOfCell("c1", "1.4");
            Assert.AreEqual(1.4, ss.GetCellValue("C1"));

            ss.SetContentsOfCell("d1", "= 53.0 + 16.3");
            Assert.AreEqual(69.3, ss.GetCellValue("D1"));
            Assert.AreEqual("", ss.GetCellValue("E1"));
            ss.SetContentsOfCell("e1", "x");
            Assert.AreEqual("x", ss.GetCellValue("E1"));
        }

        [TestMethod()]
        public void test3()
        {
            Regex testRegex = new Regex("test[a-z|A-Z]+[1-9][0-9]*");

            AbstractSpreadsheet ss = new Spreadsheet();

            ss.SetContentsOfCell("testA1", "1.0");
            ss.SetContentsOfCell("testA2", "1.5");
            ss.SetContentsOfCell("testA3", "= testA1 + testA2");
            Assert.AreEqual(2.5, ss.GetCellValue("testA3"));

            ss.SetContentsOfCell("testB1", "test");
            Assert.AreEqual("test", ss.GetCellValue("testB1"));

            ss.SetContentsOfCell("testC1", "1.4");
            Assert.AreEqual(1.4, ss.GetCellValue("tesC1"));

            ss.SetContentsOfCell("testD1", "= 53.0 + 16.3");
            Assert.AreEqual(69.3, ss.GetCellValue("testD1"));

            Assert.AreEqual("", ss.GetCellValue("testE1"));
            ss.SetContentsOfCell("testE1", "x");
            Assert.AreEqual("x", ss.GetCellValue("testE1"));

        }

        [TestMethod()]
        public void testChanged()
        {
            AbstractSpreadsheet ss = new Spreadsheet();
            Assert.IsFalse(ss.Changed);

            ss.SetContentsOfCell("A1", "test");
            Assert.IsTrue(ss.Changed);

            StringWriter sw = new StringWriter();
            ss.Save(sw);
            Assert.IsFalse(ss.Changed);

            ss.SetContentsOfCell("A1", "test");
            Assert.IsFalse(ss.Changed);

            ss.SetContentsOfCell("A1", "test2");
            Assert.IsTrue(ss.Changed);
        }

        [TestMethod()]
        public void test4()
        {
            AbstractSpreadsheet ss = new Spreadsheet();
            ss.SetContentsOfCell("A1", "=A2");
            Assert.IsInstanceOfType(ss.GetCellValue("A1"), typeof(FormulaError));
        }

        [TestMethod()]
        public void test5()
        {
            AbstractSpreadsheet ss = new Spreadsheet();
            ss.SetContentsOfCell("A1", "1.0");
            ss.SetContentsOfCell("A2", "1.0");
            ss.SetContentsOfCell("A3", "=A1 + A2");
            Assert.AreEqual(2.0, ss.GetCellValue("A3"));

            ss.SetContentsOfCell("A1", "=A4 + A2");
            Assert.IsInstanceOfType(ss.GetCellValue("A1"), typeof(FormulaError));

            ss.SetContentsOfCell("A4", "1.0");
            Assert.AreEqual(2.0, ss.GetCellValue("A1"));
            Assert.AreEqual(3.0, ss.GetCellValue("A3"));

            ss.SetContentsOfCell("A4", "0.0");
            Assert.AreEqual(2.0, ss.GetCellValue("A3"));
        }


        [TestMethod()]
        public void testSave()
        {
            AbstractSpreadsheet ss = new Spreadsheet();
            ss.SetContentsOfCell("A1", "1.5");
            ss.SetContentsOfCell("A2", "testString");
            ss.SetContentsOfCell("A3", "2.5");
            ss.SetContentsOfCell("A4", "=A1 + A3");

            StringWriter sw = new StringWriter();
            ss.Save(sw);

            Console.WriteLine(sw.ToString());
        }

    }
}
