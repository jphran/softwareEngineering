using System;
using System.IO;
using System.Text.RegularExpressions;
using System.Xml;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using SS;

namespace PS6JustinTests
{
    [TestClass]
    public class UnitTest1
    {
        [TestMethod()]
        [ExpectedException(typeof(IOException))]
        public void SaveTest5()
        {
            AbstractSpreadsheet s2 = new Spreadsheet(new StringReader("text.txt"), new Regex(""));
        }

        [TestMethod()]
        [ExpectedException(typeof(SpreadsheetReadException))]
        public void SaveTest6()
        {
            StringWriter sw = new StringWriter();
            using (XmlWriter writer = XmlWriter.Create(sw))
            {
                writer.WriteStartDocument();
                writer.WriteStartElement("testing");
                writer.WriteAttributeString("IsValid", "^.*$");

                writer.WriteEndElement();
                writer.WriteEndDocument();
            }
            AbstractSpreadsheet s2 = new Spreadsheet(new StringReader(sw.ToString()), new Regex(""));
        }

        [TestMethod()]
        [ExpectedException(typeof(SpreadsheetReadException))]
        public void SaveTest7()
        {
            StringWriter sw = new StringWriter();
            using (XmlWriter writer = XmlWriter.Create(sw))
            {
                writer.WriteStartDocument();
                writer.WriteStartElement("spreadsheet");
                writer.WriteAttributeString("IsValid", "[");

                writer.WriteEndElement();
                writer.WriteEndDocument();
            }
            AbstractSpreadsheet s2 = new Spreadsheet(new StringReader(sw.ToString()), new Regex(""));
        }

        [TestMethod()]
        [ExpectedException(typeof(SpreadsheetReadException))]
        public void SaveTest8()
        {
            StringWriter sw = new StringWriter();
            using (XmlWriter writer = XmlWriter.Create(sw))
            {
                writer.WriteStartDocument();
                writer.WriteStartElement("spreadsheet");
                writer.WriteAttributeString("IsValid", "");

                writer.WriteStartElement("cell");
                writer.WriteAttributeString("name", "A1");
                writer.WriteAttributeString("contents", "hello");
                writer.WriteEndElement();

                writer.WriteStartElement("cell");
                writer.WriteAttributeString("name", "A1");
                writer.WriteAttributeString("contents", "5.0");
                writer.WriteEndElement();

                writer.WriteEndElement();
                writer.WriteEndDocument();
            }
            AbstractSpreadsheet s2 = new Spreadsheet(new StringReader(sw.ToString()), new Regex(""));
        }

        [TestMethod()]
        [ExpectedException(typeof(SpreadsheetReadException))]
        public void SaveTest9()
        {
            StringWriter sw = new StringWriter();
            using (XmlWriter writer = XmlWriter.Create(sw))
            {
                writer.WriteStartDocument();
                writer.WriteStartElement("spreadsheet");
                writer.WriteAttributeString("IsValid", "/d");

                writer.WriteStartElement("cell");
                writer.WriteAttributeString("name", "A1A");
                writer.WriteAttributeString("contents", "hello");
                writer.WriteEndElement();

                writer.WriteStartElement("cell");
                writer.WriteAttributeString("name", "A6");
                writer.WriteAttributeString("contents", "5.0");
                writer.WriteEndElement();

                writer.WriteEndElement();
                writer.WriteEndDocument();
            }
            AbstractSpreadsheet s2 = new Spreadsheet(new StringReader(sw.ToString()), new Regex(""));
        }

        [TestMethod()]
        [ExpectedException(typeof(SpreadsheetReadException))]
        public void SaveTest10()
        {
            StringWriter sw = new StringWriter();
            using (XmlWriter writer = XmlWriter.Create(sw))
            {
                writer.WriteStartDocument();
                writer.WriteStartElement("spreadsheet");
                writer.WriteAttributeString("IsValid", "/d*");

                writer.WriteStartElement("cell");
                writer.WriteAttributeString("name", "A1");
                writer.WriteAttributeString("contents", "=!*(##");
                writer.WriteEndElement();

                writer.WriteStartElement("cell");
                writer.WriteAttributeString("name", "A2");
                writer.WriteAttributeString("contents", "5.0");
                writer.WriteEndElement();

                writer.WriteEndElement();
                writer.WriteEndDocument();
            }
            AbstractSpreadsheet s2 = new Spreadsheet(new StringReader(sw.ToString()), new Regex(""));
        }

        [TestMethod()]
        [ExpectedException(typeof(SpreadsheetReadException))]
        public void SaveTest11()
        {
            StringWriter sw = new StringWriter();
            using (XmlWriter writer = XmlWriter.Create(sw))
            {
                writer.WriteStartDocument();
                writer.WriteStartElement("spreadsheet");
                writer.WriteAttributeString("IsValid", "[0-3]");

                writer.WriteStartElement("cell");
                writer.WriteAttributeString("name", "A9");
                writer.WriteAttributeString("contents", "5689");
                writer.WriteEndElement();

                writer.WriteStartElement("cell");
                writer.WriteAttributeString("name", "A2");
                writer.WriteAttributeString("contents", "=A1");
                writer.WriteEndElement();

                writer.WriteEndElement();
                writer.WriteEndDocument();
            }
            AbstractSpreadsheet s2 = new Spreadsheet(new StringReader(sw.ToString()), new Regex(""));
        }

        [TestMethod()]
        [ExpectedException(typeof(SpreadsheetVersionException))]
        public void SaveTest12()
        {
            StringWriter sw = new StringWriter();
            using (XmlWriter writer = XmlWriter.Create(sw))
            {
                writer.WriteStartDocument();
                writer.WriteStartElement("spreadsheet");
                writer.WriteAttributeString("IsValid", "(?s).*");

                writer.WriteStartElement("cell");
                writer.WriteAttributeString("name", "A9");
                writer.WriteAttributeString("contents", "5689");
                writer.WriteEndElement();

                writer.WriteStartElement("cell");
                writer.WriteAttributeString("name", "A2");
                writer.WriteAttributeString("contents", "=A1");
                writer.WriteEndElement();

                writer.WriteEndElement();
                writer.WriteEndDocument();
            }
            AbstractSpreadsheet s2 = new Spreadsheet(new StringReader(sw.ToString()), new Regex("[0-3]"));
        }

        [TestMethod()]
        [ExpectedException(typeof(SpreadsheetVersionException))]
        public void SaveTest13()
        {
            StringWriter sw = new StringWriter();
            using (XmlWriter writer = XmlWriter.Create(sw))
            {
                writer.WriteStartDocument();
                writer.WriteStartElement("spreadsheet");
                writer.WriteAttributeString("IsValid", "(?s).*");

                writer.WriteStartElement("cell");
                writer.WriteAttributeString("name", "A1");
                writer.WriteAttributeString("contents", "5689");
                writer.WriteEndElement();

                writer.WriteStartElement("cell");
                writer.WriteAttributeString("name", "A2");
                writer.WriteAttributeString("contents", "=A9");
                writer.WriteEndElement();

                writer.WriteEndElement();
                writer.WriteEndDocument();
            }
            AbstractSpreadsheet s2 = new Spreadsheet(new StringReader(sw.ToString()), new Regex("[0-3]"));
        }

        [TestMethod()]
        [ExpectedException(typeof(SpreadsheetReadException))]
        public void SaveTest14()
        {
            StringWriter sw = new StringWriter();
            using (XmlWriter writer = XmlWriter.Create(sw))
            {
                writer.WriteStartDocument();
                writer.WriteStartElement("spreadsheet");
                writer.WriteAttributeString("IsValid", "(?s).*");

                writer.WriteStartElement("cell");
                writer.WriteAttributeString("name", "A1");
                writer.WriteAttributeString("contents", "=A2");
                writer.WriteEndElement();

                writer.WriteStartElement("cell");
                writer.WriteAttributeString("name", "A2");
                writer.WriteAttributeString("contents", "=A1");
                writer.WriteEndElement();

                writer.WriteEndElement();
                writer.WriteEndDocument();
            }
            AbstractSpreadsheet s2 = new Spreadsheet(new StringReader(sw.ToString()), new Regex("(?s).*"));
        }

        [TestMethod()]
        public void SaveTest15()
        {
            AbstractSpreadsheet ss = new Spreadsheet();
            Assert.IsFalse(ss.Changed);
            ss.SetContentsOfCell("A1", "10.0");
            Assert.IsTrue(ss.Changed);
            StringWriter sw = new StringWriter();
            ss.Save(sw);
            Assert.IsFalse(ss.Changed);

        }
    }
}
