using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using System.Collections.Generic;
using SS;
using Formulas;
using System.Text.RegularExpressions;
using System.IO;
using System.Xml;
using System.Threading.Tasks;

namespace PS6GradingTests
{
    [TestClass]
    public class PS6GradingTests
    {
        // Accepts all strings
        private const string ALL = "^.*$";

        // Verifies cells and their values, which must alternate.
        public void VV(AbstractSpreadsheet sheet, params object[] constraints)
        {
            for (int i = 0; i < constraints.Length; i += 2)
            {
                if (constraints[i + 1] is double)
                {
                    Assert.AreEqual((double)constraints[i + 1], (double)sheet.GetCellValue((string)constraints[i]), 1e-9);
                }
                else
                {
                    Assert.AreEqual(constraints[i + 1], sheet.GetCellValue((string)constraints[i]));
                }
            }
        }

        // For setting a spreadsheet cell.
        public IEnumerable<string> Set(AbstractSpreadsheet sheet, string name, string contents)
        {
            List<string> result = new List<string>(sheet.SetContentsOfCell(name, contents));
            return result;
        }

        // Tests 1-argument constructor
        [TestMethod, Timeout(1000)]
        public void IsValidTest1()
        {
            AbstractSpreadsheet s = new Spreadsheet();
            s.SetContentsOfCell("A1", "x");
        }

        [TestMethod, Timeout(1000)]
        [ExpectedException(typeof(InvalidNameException))]
        public void IsValidTest2()
        {
            AbstractSpreadsheet ss = new Spreadsheet(new Regex("^[b-zB-Z]+\\d+$"));
            ss.SetContentsOfCell("A1", "x");
        }

        [TestMethod, Timeout(1000)]
        public void IsValidTest3()
        {
            AbstractSpreadsheet s = new Spreadsheet();
            s.SetContentsOfCell("B1", "= A1 + C1");
        }

        [TestMethod, Timeout(1000)]
        [ExpectedException(typeof(FormulaFormatException))]
        public void IsValidTest4()
        {
            AbstractSpreadsheet ss = new Spreadsheet(new Regex("^[b-zB-Z]+\\d+$"));
            ss.SetContentsOfCell("B1", "= A1 + C1");
        }

        // Tests normalization
        [TestMethod, Timeout(1000)]
        public void NormalizeTest1()
        {
            AbstractSpreadsheet s = new Spreadsheet();
            s.SetContentsOfCell("B1", "hello");
            Assert.AreEqual("hello", s.GetCellContents("b1"));
        }

        [TestMethod, Timeout(1000)]
        public void NormalizeTest2()
        {
            AbstractSpreadsheet s = new Spreadsheet();
            s.SetContentsOfCell("a1", "5");
            s.SetContentsOfCell("A1", "6");
            s.SetContentsOfCell("B1", "= a1");
            Assert.AreEqual(6.0, (double)s.GetCellValue("B1"), 1e-9);
        }

        [TestMethod, Timeout(1000)]
        public void NormalizeTest3()
        {
            AbstractSpreadsheet ss = new Spreadsheet();
            ss.SetContentsOfCell("a1", "5");
            ss.SetContentsOfCell("A1", "6");
            ss.SetContentsOfCell("B1", "= a1");
            Assert.AreEqual(6.0, (double)ss.GetCellValue("B1"), 1e-9);
        }

        // Simple tests
        [TestMethod, Timeout(1000)]
        public void EmptySheet()
        {
            AbstractSpreadsheet ss = new Spreadsheet();
            VV(ss, "A1", "");
        }

        [TestMethod, Timeout(1000)]
        public void OneString()
        {
            AbstractSpreadsheet ss = new Spreadsheet();
            OneString(ss);
        }

        public void OneString(AbstractSpreadsheet ss)
        {
            Set(ss, "B1", "hello");
            VV(ss, "B1", "hello");
        }

        [TestMethod, Timeout(1000)]
        public void OneNumber()
        {
            AbstractSpreadsheet ss = new Spreadsheet();
            OneNumber(ss);
        }

        public void OneNumber(AbstractSpreadsheet ss)
        {
            Set(ss, "C1", "17.5");
            VV(ss, "C1", 17.5);
        }

        [TestMethod, Timeout(1000)]
        public void OneFormula()
        {
            AbstractSpreadsheet ss = new Spreadsheet();
            OneFormula(ss);
        }

        public void OneFormula(AbstractSpreadsheet ss)
        {
            Set(ss, "A1", "4.1");
            Set(ss, "B1", "5.2");
            Set(ss, "C1", "= A1+B1");
            VV(ss, "A1", 4.1, "B1", 5.2, "C1", 9.3);
        }

        [TestMethod, Timeout(1000)]
        public void Changed1()
        {
            AbstractSpreadsheet ss = new Spreadsheet();
            Assert.IsFalse(ss.Changed);
        }

        [TestMethod, Timeout(1000)]
        public void Changed2()
        {
            AbstractSpreadsheet ss = new Spreadsheet();
            Assert.IsFalse(ss.Changed);
            Set(ss, "C1", "17.5");
            Assert.IsTrue(ss.Changed);
        }

        [TestMethod, Timeout(1000)]
        public void Changed3()
        {
            AbstractSpreadsheet ss = new Spreadsheet();
            Assert.IsFalse(ss.Changed);
            Set(ss, "C1", "17.5");
            Assert.IsTrue(ss.Changed);
            ss.Save(new StringWriter());
            Assert.IsFalse(ss.Changed);
        }

        [TestMethod, Timeout(1000)]
        public void Names1()
        {
            AbstractSpreadsheet ss = new Spreadsheet();
            Assert.IsFalse(ss.GetNamesOfAllNonemptyCells().GetEnumerator().MoveNext());
        }

        [TestMethod, Timeout(1000)]
        public void Names2()
        {
            AbstractSpreadsheet ss = new Spreadsheet();
            ss.SetContentsOfCell("A1", "hello");
            ss.SetContentsOfCell("B1", "55");
            ss.SetContentsOfCell("C1", "=A1+B1");
            HashSet<string> names = new HashSet<string>();
            names.Add("A1");
            names.Add("B1");
            names.Add("C1");
            Assert.IsTrue(names.SetEquals(ss.GetNamesOfAllNonemptyCells()));
        }

        [TestMethod, Timeout(1000)]
        public void Names3()
        {
            AbstractSpreadsheet ss = new Spreadsheet();
            ss.SetContentsOfCell("A1", "hello");
            ss.SetContentsOfCell("B1", "55");
            ss.SetContentsOfCell("C1", "=A1+B1");
            ss.SetContentsOfCell("A1", "");
            HashSet<string> names = new HashSet<string>();
            names.Add("B1");
            names.Add("C1");
            Assert.IsTrue(names.SetEquals(ss.GetNamesOfAllNonemptyCells()));
        }


        [TestMethod, Timeout(1000)]
        public void DivisionByZero1()
        {
            AbstractSpreadsheet ss = new Spreadsheet();
            DivisionByZero1(ss);
        }

        public void DivisionByZero1(AbstractSpreadsheet ss)
        {
            Set(ss, "A1", "4.1");
            Set(ss, "B1", "0.0");
            Set(ss, "C1", "= A1 / B1");
            Assert.IsInstanceOfType(ss.GetCellValue("C1"), typeof(FormulaError));
        }

        [TestMethod, Timeout(1000)]
        public void DivisionByZero2()
        {
            AbstractSpreadsheet ss = new Spreadsheet();
            DivisionByZero2(ss);
        }

        public void DivisionByZero2(AbstractSpreadsheet ss)
        {
            Set(ss, "A1", "5.0");
            Set(ss, "A3", "= A1 / 0.0");
            Assert.IsInstanceOfType(ss.GetCellValue("A3"), typeof(FormulaError));
        }

        [TestMethod, Timeout(1000)]
        public void EmptyArgument()
        {
            AbstractSpreadsheet ss = new Spreadsheet();
            EmptyArgument(ss);
        }

        public void EmptyArgument(AbstractSpreadsheet ss)
        {
            Set(ss, "A1", "4.1");
            Set(ss, "C1", "= A1 + B1");
            Assert.IsInstanceOfType(ss.GetCellValue("C1"), typeof(FormulaError));
        }

        [TestMethod, Timeout(1000)]
        public void StringArgument()
        {
            AbstractSpreadsheet ss = new Spreadsheet();
            StringArgument(ss);
        }

        public void StringArgument(AbstractSpreadsheet ss)
        {
            Set(ss, "A1", "4.1");
            Set(ss, "B1", "hello");
            Set(ss, "C1", "= A1 + B1");
            Assert.IsInstanceOfType(ss.GetCellValue("C1"), typeof(FormulaError));
        }

        [TestMethod, Timeout(1000)]
        public void ErrorArgument()
        {
            AbstractSpreadsheet ss = new Spreadsheet();
            ErrorArgument(ss);
        }

        public void ErrorArgument(AbstractSpreadsheet ss)
        {
            Set(ss, "A1", "4.1");
            Set(ss, "B1", "");
            Set(ss, "C1", "= A1 + B1");
            Set(ss, "D1", "= C1");
            Assert.IsInstanceOfType(ss.GetCellValue("D1"), typeof(FormulaError));
        }

        [TestMethod, Timeout(1000)]
        public void NumberFormula1()
        {
            AbstractSpreadsheet ss = new Spreadsheet();
            NumberFormula1(ss);
        }

        public void NumberFormula1(AbstractSpreadsheet ss)
        {
            Set(ss, "A1", "4.1");
            Set(ss, "C1", "= A1 + 4.2");
            VV(ss, "C1", 8.3);
        }

        [TestMethod, Timeout(1000)]
        public void NumberFormula2()
        {
            AbstractSpreadsheet ss = new Spreadsheet();
            NumberFormula2(ss);
        }

        public void NumberFormula2(AbstractSpreadsheet ss)
        {
            Set(ss, "A1", "= 4.6");
            VV(ss, "A1", 4.6);
        }

        // Repeats the simple tests all together
        [TestMethod, Timeout(1000)]
        public void RepeatSimpleTests()
        {
            AbstractSpreadsheet ss = new Spreadsheet();
            Set(ss, "A1", "17.32");
            Set(ss, "B1", "This is a test");
            Set(ss, "C1", "= A1+B1");
            OneString(ss);
            OneNumber(ss);
            OneFormula(ss);
            DivisionByZero1(ss);
            DivisionByZero2(ss);
            StringArgument(ss);
            ErrorArgument(ss);
            NumberFormula1(ss);
            NumberFormula2(ss);
        }

        // Four kinds of formulas
        [TestMethod, Timeout(1000)]
        public void Formulas()
        {
            AbstractSpreadsheet ss = new Spreadsheet();
            Formulas(ss);
        }

        public void Formulas(AbstractSpreadsheet ss)
        {
            Set(ss, "A1", "4.4");
            Set(ss, "B1", "2.2");
            Set(ss, "C1", "= A1 + B1");
            Set(ss, "D1", "= A1 - B1");
            Set(ss, "E1", "= A1 * B1");
            Set(ss, "F1", "= A1 / B1");
            VV(ss, "C1", 6.6, "D1", 2.2, "E1", 4.4 * 2.2, "F1", 2.0);
        }

        [TestMethod, Timeout(1000)]
        public void Formulasa()
        {
            Formulas();
        }

        [TestMethod, Timeout(1000)]
        public void Formulasb()
        {
            Formulas();
        }

        // Are multiple spreadsheets supported?
        [TestMethod, Timeout(1000)]
        public void Multiple1()
        {
            AbstractSpreadsheet s1 = new Spreadsheet();
            AbstractSpreadsheet s2 = new Spreadsheet();
            Set(s1, "X1", "hello");
            Set(s2, "X1", "goodbye");
            VV(s1, "X1", "hello");
            VV(s2, "X1", "goodbye");
        }

        [TestMethod, Timeout(1000)]
        public void Multiple1a()
        {
            Multiple1();
        }

        [TestMethod, Timeout(1000)]
        public void Multiple1b()
        {
            Multiple1();
        }

        [TestMethod, Timeout(1000)]
        public void Multiple2()
        {
            AbstractSpreadsheet s1 = new Spreadsheet();
            AbstractSpreadsheet s2 = new Spreadsheet();
            s1.SetContentsOfCell("A1", "x");
            Assert.IsTrue(s1.Changed);
            Assert.IsFalse(s2.Changed);
        }

        [TestMethod, Timeout(1000)]
        public void Multiple2a()
        {
            Multiple2();
        }

        [TestMethod, Timeout(1000)]
        public void Multiple2b()
        {
            Multiple2();
        }

        // Tests save method and 3-parameter constructor
        [TestMethod, Timeout(1000)]
        public void SaveTest1()
        {
            AbstractSpreadsheet s1 = new Spreadsheet();
            Set(s1, "A1", "hello");
            StringWriter sw = new StringWriter();
            s1.Save(sw);
            s1 = new Spreadsheet(new StringReader(sw.ToString()), new Regex(""));
            Assert.AreEqual("hello", s1.GetCellContents("A1"));
        }

        [TestMethod, Timeout(1000)]
        public void SaveTest2()
        {
            try
            {
                AbstractSpreadsheet ss = new Spreadsheet(new StringReader("Hello world"), new Regex(""));
                Assert.Fail();
            }
            catch (Exception)
            {
            }
        }

        [TestMethod, Timeout(1000)]
        public void SaveTest3()
        {
            StringWriter sw = new StringWriter();
            using (XmlWriter writer = XmlWriter.Create(sw))
            {
                writer.WriteStartDocument();
                writer.WriteStartElement("spreadsheet");
                writer.WriteAttributeString("IsValid", "^.*$");

                writer.WriteStartElement("cell");
                writer.WriteAttributeString("name", "A1");
                writer.WriteAttributeString("contents", "hello");
                writer.WriteEndElement();

                writer.WriteStartElement("cell");
                writer.WriteAttributeString("name", "A2");
                writer.WriteAttributeString("contents", "5.0");
                writer.WriteEndElement();

                writer.WriteStartElement("cell");
                writer.WriteAttributeString("name", "A3");
                writer.WriteAttributeString("contents", "4.0");
                writer.WriteEndElement();

                writer.WriteStartElement("cell");
                writer.WriteAttributeString("name", "A4");
                writer.WriteAttributeString("contents", "= A2 + A3");
                writer.WriteEndElement();

                writer.WriteEndElement();
                writer.WriteEndDocument();
            }
            AbstractSpreadsheet ss = new Spreadsheet(new StringReader(sw.ToString()), new Regex(""));
            VV(ss, "A1", "hello", "A2", 5.0, "A3", 4.0, "A4", 9.0);
        }

        [TestMethod, Timeout(1000)]
        public void SaveTest4()
        {
            AbstractSpreadsheet ss = new Spreadsheet();
            Set(ss, "A1", "hello");
            Set(ss, "A2", "5.0");
            Set(ss, "A3", "4.0");
            Set(ss, "A4", "= A2 + A3");
            StringWriter sw = new StringWriter();
            ss.Save(sw);

            using (XmlReader reader = XmlReader.Create(new StringReader(sw.ToString())))
            {
                int spreadsheetCount = 0;
                int cellCount = 0;

                while (reader.Read())
                {
                    if (reader.IsStartElement())
                    {
                        switch (reader.Name)
                        {
                            case "spreadsheet":
                                Assert.IsTrue(new Regex(reader["IsValid"]).IsMatch("a4"));
                                spreadsheetCount++;
                                break;

                            case "cell":
                                string name = reader["name"];
                                string contents = reader["contents"];
                                if (name.Equals("A1")) { Assert.AreEqual("hello", contents); }
                                else if (name.Equals("A2")) { Assert.AreEqual(5.0, Double.Parse(contents), 1e-9); }
                                else if (name.Equals("A3")) { Assert.AreEqual(4.0, Double.Parse(contents), 1e-9); }
                                else if (name.Equals("A4")) { contents = contents.Replace(" ", ""); Assert.AreEqual("=A2+A3", contents); }
                                else Assert.Fail();
                                cellCount++;
                                break;
                        }
                    }
                }
                Assert.AreEqual(1, spreadsheetCount);
                Assert.AreEqual(4, cellCount);
            }
        }

        // Violates schema: has an attribute "content" instead of "contents"
        [TestMethod, Timeout(1000)]
        [ExpectedException(typeof(SpreadsheetReadException))]
        public void SaveTest5()
        {
            StringWriter sw = new StringWriter();
            using (XmlWriter writer = XmlWriter.Create(sw))
            {
                writer.WriteStartDocument();
                writer.WriteStartElement("spreadsheet");
                writer.WriteAttributeString("IsValid", "^.*$");

                writer.WriteStartElement("cell");
                writer.WriteAttributeString("name", "A1");
                writer.WriteAttributeString("contents", "hello");
                writer.WriteEndElement();

                writer.WriteStartElement("cell");
                writer.WriteAttributeString("name", "A2");
                writer.WriteAttributeString("contents", "5.0");
                writer.WriteEndElement();

                writer.WriteStartElement("cell");
                writer.WriteAttributeString("name", "A3");
                writer.WriteAttributeString("contents", "4.0");
                writer.WriteEndElement();

                writer.WriteStartElement("cell");
                writer.WriteAttributeString("name", "A4");
                writer.WriteAttributeString("content", "= A2 + A3");
                writer.WriteEndElement();

                writer.WriteEndElement();
                writer.WriteEndDocument();
            }
            AbstractSpreadsheet ss = new Spreadsheet(new StringReader(sw.ToString()), new Regex(""));
        }

        // Invalid regular expression
        [TestMethod, Timeout(1000)]
        [ExpectedException(typeof(SpreadsheetReadException))]
        public void SaveTest6()
        {
            StringWriter sw = new StringWriter();
            using (XmlWriter writer = XmlWriter.Create(sw))
            {
                writer.WriteStartDocument();
                writer.WriteStartElement("spreadsheet");
                writer.WriteAttributeString("IsValid", "\\Q");

                writer.WriteStartElement("cell");
                writer.WriteAttributeString("name", "A1");
                writer.WriteAttributeString("contents", "hello");
                writer.WriteEndElement();

                writer.WriteStartElement("cell");
                writer.WriteAttributeString("name", "A2");
                writer.WriteAttributeString("contents", "5.0");
                writer.WriteEndElement();

                writer.WriteStartElement("cell");
                writer.WriteAttributeString("name", "A3");
                writer.WriteAttributeString("contents", "4.0");
                writer.WriteEndElement();

                writer.WriteStartElement("cell");
                writer.WriteAttributeString("name", "A4");
                writer.WriteAttributeString("contents", "= A2 + A3");
                writer.WriteEndElement();

                writer.WriteEndElement();
                writer.WriteEndDocument();
            }
            AbstractSpreadsheet ss = new Spreadsheet(new StringReader(sw.ToString()), new Regex(""));
        }

        // Two definitions of A1
        [TestMethod, Timeout(1000)]
        [ExpectedException(typeof(SpreadsheetReadException))]
        public void SaveTest7()
        {
            StringWriter sw = new StringWriter();
            using (XmlWriter writer = XmlWriter.Create(sw))
            {
                writer.WriteStartDocument();
                writer.WriteStartElement("spreadsheet");
                writer.WriteAttributeString("IsValid", "^.*$");

                writer.WriteStartElement("cell");
                writer.WriteAttributeString("name", "A1");
                writer.WriteAttributeString("contents", "hello");
                writer.WriteEndElement();

                writer.WriteStartElement("cell");
                writer.WriteAttributeString("name", "A2");
                writer.WriteAttributeString("contents", "5.0");
                writer.WriteEndElement();

                writer.WriteStartElement("cell");
                writer.WriteAttributeString("name", "A3");
                writer.WriteAttributeString("contents", "4.0");
                writer.WriteEndElement();

                writer.WriteStartElement("cell");
                writer.WriteAttributeString("name", "A1");
                writer.WriteAttributeString("contents", "world");
                writer.WriteEndElement();

                writer.WriteEndElement();
                writer.WriteEndDocument();
            }
            AbstractSpreadsheet ss = new Spreadsheet(new StringReader(sw.ToString()), new Regex(""));
        }

        // IsValid doesn't match A4
        [TestMethod, Timeout(1000)]
        [ExpectedException(typeof(SpreadsheetReadException))]
        public void SaveTest8()
        {
            StringWriter sw = new StringWriter();
            using (XmlWriter writer = XmlWriter.Create(sw))
            {
                writer.WriteStartDocument();
                writer.WriteStartElement("spreadsheet");
                writer.WriteAttributeString("IsValid", "^A[1-3]$");

                writer.WriteStartElement("cell");
                writer.WriteAttributeString("name", "A1");
                writer.WriteAttributeString("contents", "hello");
                writer.WriteEndElement();

                writer.WriteStartElement("cell");
                writer.WriteAttributeString("name", "A2");
                writer.WriteAttributeString("contents", "5.0");
                writer.WriteEndElement();

                writer.WriteStartElement("cell");
                writer.WriteAttributeString("name", "A3");
                writer.WriteAttributeString("contents", "4.0");
                writer.WriteEndElement();

                writer.WriteStartElement("cell");
                writer.WriteAttributeString("name", "A4");
                writer.WriteAttributeString("contents", "= A2 + A3");
                writer.WriteEndElement();

                writer.WriteEndElement();
                writer.WriteEndDocument();
            }
            AbstractSpreadsheet ss = new Spreadsheet(new StringReader(sw.ToString()), new Regex(""));
        }

        // The new IsValid doesn't match A4
        [TestMethod, Timeout(1000)]
        [ExpectedException(typeof(SpreadsheetVersionException))]
        public void SaveTest9()
        {
            StringWriter sw = new StringWriter();
            using (XmlWriter writer = XmlWriter.Create(sw))
            {
                writer.WriteStartDocument();
                writer.WriteStartElement("spreadsheet");
                writer.WriteAttributeString("IsValid", "^.*$");

                writer.WriteStartElement("cell");
                writer.WriteAttributeString("name", "A1");
                writer.WriteAttributeString("contents", "hello");
                writer.WriteEndElement();

                writer.WriteStartElement("cell");
                writer.WriteAttributeString("name", "A2");
                writer.WriteAttributeString("contents", "5.0");
                writer.WriteEndElement();

                writer.WriteStartElement("cell");
                writer.WriteAttributeString("name", "A3");
                writer.WriteAttributeString("contents", "4.0");
                writer.WriteEndElement();

                writer.WriteStartElement("cell");
                writer.WriteAttributeString("name", "A4");
                writer.WriteAttributeString("contents", "= A2 + A3");
                writer.WriteEndElement();

                writer.WriteEndElement();
                writer.WriteEndDocument();
            }
            AbstractSpreadsheet ss = new Spreadsheet(new StringReader(sw.ToString()), new Regex("^A[1-3]$"));
        }

        // The new regex should be used in the new sheet
        [TestMethod, Timeout(1000)]
        public void SaveTest10()
        {
            StringWriter sw = new StringWriter();
            using (XmlWriter writer = XmlWriter.Create(sw))
            {
                writer.WriteStartDocument();
                writer.WriteStartElement("spreadsheet");
                writer.WriteAttributeString("IsValid", "^[A-Z]\\d$");

                writer.WriteStartElement("cell");
                writer.WriteAttributeString("name", "A1");
                writer.WriteAttributeString("contents", "hello");
                writer.WriteEndElement();

                writer.WriteStartElement("cell");
                writer.WriteAttributeString("name", "B2");
                writer.WriteAttributeString("contents", "5.0");
                writer.WriteEndElement();

                writer.WriteStartElement("cell");
                writer.WriteAttributeString("name", "C3");
                writer.WriteAttributeString("contents", "4.0");
                writer.WriteEndElement();

                writer.WriteStartElement("cell");
                writer.WriteAttributeString("name", "D4");
                writer.WriteAttributeString("contents", "= B2 + C3");
                writer.WriteEndElement();

                writer.WriteEndElement();
                writer.WriteEndDocument();
            }
            AbstractSpreadsheet ss = new Spreadsheet(new StringReader(sw.ToString()), new Regex("^[A-D]\\d$"));
            ss.SetContentsOfCell("B5", "5");
            ss.SetContentsOfCell("C5", "=B5");
            try
            {
                ss.SetContentsOfCell("E55", "5");
                Assert.Fail();
            }
            catch (InvalidNameException)
            {
            }
        }

        // Fun with formulas
        [TestMethod, Timeout(1000)]
        public void Formula1()
        {
            Formula1(new Spreadsheet());
        }
        public void Formula1(AbstractSpreadsheet ss)
        {
            Set(ss, "A1", "= A2 + A3");
            Set(ss, "A2", "= B1 + B2");
            Assert.IsInstanceOfType(ss.GetCellValue("A1"), typeof(FormulaError));
            Assert.IsInstanceOfType(ss.GetCellValue("A2"), typeof(FormulaError));
            Set(ss, "A3", "5.0");
            Set(ss, "B1", "2.0");
            Set(ss, "B2", "3.0");
            VV(ss, "A1", 10.0, "A2", 5.0);
            Set(ss, "B2", "4.0");
            VV(ss, "A1", 11.0, "A2", 6.0);
        }

        [TestMethod, Timeout(1000)]
        public void Formula2()
        {
            Formula2(new Spreadsheet());
        }
        public void Formula2(AbstractSpreadsheet ss)
        {
            Set(ss, "A1", "= A2 + A3");
            Set(ss, "A2", "= A3");
            Set(ss, "A3", "6.0");
            VV(ss, "A1", 12.0, "A2", 6.0, "A3", 6.0);
            Set(ss, "A3", "5.0");
            VV(ss, "A1", 10.0, "A2", 5.0, "A3", 5.0);
        }

        [TestMethod, Timeout(1000)]
        public void Formula3()
        {
            Formula3(new Spreadsheet());
        }
        public void Formula3(AbstractSpreadsheet ss)
        {
            Set(ss, "A1", "= A3 + A5");
            Set(ss, "A2", "= A5 + A4");
            Set(ss, "A3", "= A5");
            Set(ss, "A4", "= A5");
            Set(ss, "A5", "9.0");
            VV(ss, "A1", 18.0);
            VV(ss, "A2", 18.0);
            Set(ss, "A5", "8.0");
            VV(ss, "A1", 16.0);
            VV(ss, "A2", 16.0);
        }

        [TestMethod, Timeout(1000)]
        public void Formula4()
        {
            AbstractSpreadsheet ss = new Spreadsheet();
            Formula1(ss);
            Formula2(ss);
            Formula3(ss);
        }

        [TestMethod, Timeout(1000)]
        public void Formula4a()
        {
            Formula4();
        }

        [TestMethod, Timeout(1000)]
        public void Formula4b()
        {
            Formula4();
        }

        [TestMethod, Timeout(1000)]
        public void MediumSheet()
        {
            AbstractSpreadsheet ss = new Spreadsheet();
            MediumSheet(ss);
        }

        public void MediumSheet(AbstractSpreadsheet ss)
        {
            Set(ss, "A1", "1.0");
            Set(ss, "A2", "2.0");
            Set(ss, "A3", "3.0");
            Set(ss, "A4", "4.0");
            Set(ss, "B1", "= A1 + A2");
            Set(ss, "B2", "= A3 * A4");
            Set(ss, "C1", "= B1 + B2");
            VV(ss, "A1", 1.0, "A2", 2.0, "A3", 3.0, "A4", 4.0, "B1", 3.0, "B2", 12.0, "C1", 15.0);
            Set(ss, "A1", "2.0");
            VV(ss, "A1", 2.0, "A2", 2.0, "A3", 3.0, "A4", 4.0, "B1", 4.0, "B2", 12.0, "C1", 16.0);
            Set(ss, "B1", "= A1 / A2");
            VV(ss, "A1", 2.0, "A2", 2.0, "A3", 3.0, "A4", 4.0, "B1", 1.0, "B2", 12.0, "C1", 13.0);
        }

        [TestMethod, Timeout(1000)]
        public void MediumSheeta()
        {
            MediumSheet();
        }

        [TestMethod, Timeout(1000)]
        public void MediumSheetb()
        {
            MediumSheet();
        }


        [TestMethod, Timeout(1000)]
        public void MediumSave()
        {
            AbstractSpreadsheet ss = new Spreadsheet();
            MediumSheet(ss);
            StringWriter sw = new StringWriter();
            ss.Save(sw);
            ss = new Spreadsheet(new StringReader(sw.ToString()), new Regex(""));
            VV(ss, "A1", 2.0, "A2", 2.0, "A3", 3.0, "A4", 4.0, "B1", 1.0, "B2", 12.0, "C1", 13.0);
        }

        [TestMethod, Timeout(1000)]
        public void MediumSavea()
        {
            MediumSave();
        }

        [TestMethod, Timeout(1000)]
        public void MediumSaveb()
        {
            MediumSave();
        }

        [TestMethod, Timeout(3000)]
        public void LongTestWithTimeout3()
        {
            LongTest();
        }

        [TestMethod, Timeout(6000)]
        public void LongTestWithTimeout6()
        {
            LongTest();
        }

        [TestMethod, Timeout(9000)]
        public void LongTestWithTimeout9()
        {
            LongTest();
        }

        [TestMethod, Timeout(12000)]
        public void LongTestWithTimeout12()
        {
            LongTest();
        }

        [TestMethod, Timeout(15000)]
        public void LongTestWithTimeout15()
        {
            LongTest();
        }

        public void LongTest()
        {
            AbstractSpreadsheet s = new Spreadsheet();
            s.SetContentsOfCell("SUM1", "= A1 + A2");
            int i;
            int depth = 100;
            for (i = 1; i <= depth * 2; i += 2)
            {
                s.SetContentsOfCell("A" + i, "= A" + (i + 2) + " + A" + (i + 3));
                s.SetContentsOfCell("A" + (i + 1), "= A" + (i + 2) + "+ A" + (i + 3));
            }
            s.SetContentsOfCell("A" + i, "1");
            s.SetContentsOfCell("A" + (i + 1), "1");
            Assert.AreEqual(Math.Pow(2, depth + 1), (double)s.GetCellValue("SUM1"), 1e20);
            s.SetContentsOfCell("A" + i, "0");
            Assert.AreEqual(Math.Pow(2, depth), (double)s.GetCellValue("SUM1"), 1e20);
            s.SetContentsOfCell("A" + (i + 1), "0");
            Assert.AreEqual(0.0, (double)s.GetCellValue("SUM1"), 0.1);
        }

        // Regression tests from PS5GradingTests

        [TestMethod, Timeout(1000)]
        public void SetContentsReturn()
        {
            AbstractSpreadsheet s = new Spreadsheet();
            s.SetContentsOfCell("A1", "=A2+A3");
            s.SetContentsOfCell("A2", "6");
            s.SetContentsOfCell("A3", "=A2+A4");
            s.SetContentsOfCell("A4", "=A2+A5");
            HashSet<string> result = new HashSet<string>(s.SetContentsOfCell("A5", "82.5"));
            Assert.IsTrue(result.SetEquals(new string[] { "A5", "A4", "A3", "A1" }));
        }

        // CIRCULAR FORMULA DETECTION
        [TestMethod, Timeout(1000)]
        [ExpectedException(typeof(CircularException))]
        public void Circular1()
        {
            AbstractSpreadsheet s = new Spreadsheet();
            s.SetContentsOfCell("A1", "=A2");
            s.SetContentsOfCell("A2", "=A1");
        }

        [TestMethod, Timeout(1000)]
        [ExpectedException(typeof(CircularException))]
        public void Circular2()
        {
            AbstractSpreadsheet s = new Spreadsheet();
            s.SetContentsOfCell("A1", "=A2+A3");
            s.SetContentsOfCell("A3", "=A4+A5");
            s.SetContentsOfCell("A5", "=A6+A7");
            s.SetContentsOfCell("A7", "=A1+A1");
        }

        [TestMethod, Timeout(1000)]
        [ExpectedException(typeof(CircularException))]
        public void Circular3()
        {
            AbstractSpreadsheet s = new Spreadsheet();
            try
            {
                s.SetContentsOfCell("A1", "=A2+A3");
                s.SetContentsOfCell("A2", "15");
                s.SetContentsOfCell("A3", "30");
                s.SetContentsOfCell("A2", "=A3*A1");
            }
            catch (CircularException e)
            {
                //Assert.AreEqual(15, (double)s.GetCellValue("A2"), 1e-9);
                Assert.AreEqual(15, (double)s.GetCellContents("A2"), 1e-9);
                throw e;
            }
        }

        [TestMethod, Timeout(1000)]
        public void Stress1WithTimeout1()
        {
            Stress1();
        }

        [TestMethod, Timeout(2000)]
        public void Stress1WithTimeout2()
        {
            Stress1();
        }

        public void Stress1()
        {
            AbstractSpreadsheet s = new Spreadsheet();
            s.SetContentsOfCell("A1", "=B1+B2");
            s.SetContentsOfCell("B1", "=C1-C2");
            s.SetContentsOfCell("B2", "=C3*C4");
            s.SetContentsOfCell("C1", "=D1*D2");
            s.SetContentsOfCell("C2", "=D3*D4");
            s.SetContentsOfCell("C3", "=D5*D6");
            s.SetContentsOfCell("C4", "=D7*D8");
            s.SetContentsOfCell("D1", "=E1");
            s.SetContentsOfCell("D2", "=E1");
            s.SetContentsOfCell("D3", "=E1");
            s.SetContentsOfCell("D4", "=E1");
            s.SetContentsOfCell("D5", "=E1");
            s.SetContentsOfCell("D6", "=E1");
            s.SetContentsOfCell("D7", "=E1");
            s.SetContentsOfCell("D8", "=E1");
            ISet<String> cells = s.SetContentsOfCell("E1", "0");
            Assert.IsTrue(cells.SetEquals(new HashSet<string>() { "A1", "B1", "B2", "C1", "C2", "C3", "C4", "D1", "D2", "D3", "D4", "D5", "D6", "D7", "D8", "E1" }));
        }

        [TestMethod, Timeout(10000)]
        public void Stress2WithTimeout10()
        {
            Stress2();
        }

        [TestMethod, Timeout(20000)]
        public void Stress2WithTimeout20()
        {
            Stress2();
        }

        public void Stress2()
        {
            AbstractSpreadsheet s = new Spreadsheet();
            ISet<String> cells = new HashSet<string>();
            for (int i = 1; i < 200; i++)
            {
                cells.Add("A" + i);
                Assert.IsTrue(cells.SetEquals(s.SetContentsOfCell("A" + i, "=A" + (i + 1))));
            }
        }

        [TestMethod, Timeout(10000)]
        public void Stress3WithTimeout10()
        {
            Stress3();
        }

        [TestMethod, Timeout(20000)]
        public void Stress3WithTimeout20()
        {
            Stress3();
        }

        public void Stress3()
        {
            AbstractSpreadsheet s = new Spreadsheet();
            for (int i = 1; i < 200; i++)
            {
                s.SetContentsOfCell("A" + i, "=A" + (i + 1));
            }
            try
            {
                s.SetContentsOfCell("A150", "=A50");
                Assert.Fail();
            }
            catch (CircularException)
            {
            }
        }

        [TestMethod, Timeout(10000)]
        public void Stress4WithTimeout10()
        {
            Stress4();
        }

        [TestMethod, Timeout(20000)]
        public void Stress4WithTimeout20()
        {
            Stress4();
        }

        public void Stress4()
        {
            int SIZE = 200;

            AbstractSpreadsheet s = new Spreadsheet();
            for (int i = 0; i < SIZE; i++)
            {
                s.SetContentsOfCell("A1" + i, "=A1" + (i + 1));
            }

            ISet<string> sss = s.SetContentsOfCell("A1" + (SIZE - 1), "25.0");
            Assert.AreEqual(SIZE, sss.Count);
            for (int i = 0; i < SIZE; i++)
            {
                Assert.IsTrue(sss.Contains("A1" + i));
            }

            sss = s.SetContentsOfCell("A1" + (SIZE / 2 - 1), "25.0");
            Assert.AreEqual(SIZE / 2, sss.Count);
            for (int i = 0; i < SIZE / 2; i++)
            {
                Assert.IsTrue(sss.Contains("A1" + i));
            }
        }

        [TestMethod, Timeout(10000)]
        public void Stress5WithTimeout10()
        {
            Stress5();
        }

        [TestMethod, Timeout(20000)]
        public void Stress5WithTimeout20()
        {
            Stress5();
        }

        public void Stress5()
        {
            int seed = 47;
            int size = 831;
            AbstractSpreadsheet s = new Spreadsheet();
            Random rand = new Random(seed);
            for (int i = 0; i < 1000; i++)
            {
                try
                {
                    switch (rand.Next(3))
                    {
                        case 0:
                            s.SetContentsOfCell(randomName(rand), "3.14");
                            break;
                        case 1:
                            s.SetContentsOfCell(randomName(rand), "hello");
                            break;
                        case 2:
                            s.SetContentsOfCell(randomName(rand), "=" + randomFormula(rand));
                            break;
                    }
                }
                catch (CircularException)
                {
                }
            }
            ISet<string> set = new HashSet<string>(s.GetNamesOfAllNonemptyCells());
            Assert.AreEqual(size, set.Count);
        }

        private String randomName(Random rand)
        {
            return "ABCDEFGHIJKLMNOPQRSTUVWXYZ".Substring(rand.Next(26), 1) + (rand.Next(99) + 1);
        }

        private String randomFormula(Random rand)
        {
            String f = randomName(rand);
            for (int i = 0; i < 10; i++)
            {
                switch (rand.Next(4))
                {
                    case 0:
                        f += "+";
                        break;
                    case 1:
                        f += "-";
                        break;
                    case 2:
                        f += "*";
                        break;
                    case 3:
                        f += "/";
                        break;
                }
                switch (rand.Next(2))
                {
                    case 0:
                        f += 7.2;
                        break;
                    case 1:
                        f += randomName(rand);
                        break;
                }
            }
            return f;
        }

    }
}
