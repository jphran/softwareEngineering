// Written by Joe Zachary for CS 3500, January 2017.
// Edited by Justin Francis, jan 2019

using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using Formulas;

namespace FormulaTestCases
{
    /// <summary>
    /// These test cases are in no sense comprehensive!  They are intended to show you how
    /// client code can make use of the Formula class, and to show you how to create your
    /// own (which we strongly recommend).  To run them, pull down the Test menu and do
    /// Run > All Tests.
    /// </summary>
    [TestClass]
    public class UnitTests
    {
        /// <summary>
        /// This tests that a syntactically incorrect parameter to Formula results
        /// in a FormulaFormatException.
        /// </summary>
        [TestMethod]
        [ExpectedException(typeof(FormulaFormatException))]
        public void Construct1()
        {
            Formula f = new Formula("_");
        }

        /// <summary>
        /// This is another syntax error
        /// </summary>
        [TestMethod]
        [ExpectedException(typeof(FormulaFormatException))]
        public void Construct2()
        {
            Formula f = new Formula("2++3");
        }

        /// <summary>
        /// Another syntax error.
        /// </summary>
        [TestMethod]
        [ExpectedException(typeof(FormulaFormatException))]
        public void Construct3()
        {
            Formula f = new Formula("2 3");
        }


        /// <summary>
        /// Another syntax error.
        /// </summary>
        [TestMethod]
        [ExpectedException(typeof(FormulaFormatException))]
        public void Construct4()
        {
            Formula f = new Formula("");
        }

        /// <summary>
        /// Another syntax error.
        /// </summary>
        [TestMethod]
        [ExpectedException(typeof(FormulaFormatException))]
        public void Construct17()
        {
            Formula f = new Formula("~");
        }

        /// <summary>
        /// Another syntax error.
        /// </summary>
        [TestMethod]
        [ExpectedException(typeof(FormulaFormatException))]
        public void Construct5()
        {
            Formula f = new Formula("  ");
        }

        /// <summary>
        /// Another syntax error.
        /// </summary>
        [TestMethod]
        [ExpectedException(typeof(FormulaFormatException))]
        public void Construct6()
        {
            Formula f = new Formula(")");
        }


        /// <summary>
        /// Another syntax error.
        /// </summary>
        [TestMethod]
        [ExpectedException(typeof(FormulaFormatException))]
        public void Construct7()
        {
            Formula f = new Formula("*");
        }

        /// <summary>
        /// Another syntax error.
        /// </summary>
        [TestMethod]
        [ExpectedException(typeof(FormulaFormatException))]
        public void Construct8()
        {
            Formula f = new Formula("())");
        }

        /// <summary>
        /// Another syntax error.
        /// </summary>
        [TestMethod]
        [ExpectedException(typeof(FormulaFormatException))]
        public void Construct9()
        {
            Formula f = new Formula("(()");
        }

        /// <summary>
        /// Another syntax error.
        /// </summary>
        [TestMethod]
        [ExpectedException(typeof(FormulaFormatException))]
        public void Construct10()
        {
            Formula f = new Formula("(5+78 ) +");
        }

        /// <summary>
        /// Another syntax error.
        /// </summary>
        [TestMethod]
        [ExpectedException(typeof(FormulaFormatException))]
        public void Construct11()
        {
            Formula f = new Formula("(5+78 ) (");
        }

        /// <summary>
        /// Another syntax error.
        /// </summary>
        [TestMethod]
        [ExpectedException(typeof(FormulaFormatException))]
        public void Construct12()
        {
            Formula f = new Formula("()");
        }
        /// <summary>
        /// Another syntax error.
        /// </summary>
        [TestMethod]
        [ExpectedException(typeof(FormulaFormatException))]
        public void Construct13()
        {
            Formula f = new Formula("5.0 5.0");
        }

        /// <summary>
        /// Another syntax error.
        /// </summary>
        [TestMethod]
        [ExpectedException(typeof(FormulaFormatException))]
        public void Construct14()
        {
            Formula f = new Formula("4.0 x");
        }

        /// <summary>
        /// Another syntax error.
        /// </summary>
        [TestMethod]
        [ExpectedException(typeof(FormulaFormatException))]
        public void Construct15()
        {
            Formula f = new Formula("(x) 5.0");
        }

        /// <summary>
        /// Makes sure that "2+3" evaluates to 5.  Since the Formula
        /// contains no variables, the delegate passed in as the
        /// parameter doesn't matter.  We are passing in one that
        /// maps all variables to zero.
        /// </summary>
        [TestMethod]
        public void Evaluate1()
        {
            Formula f = new Formula("2+3");
            Assert.AreEqual(f.Evaluate(v => 0), 5.0, 1e-6);
        }

        /// <summary>
        /// The Formula consists of a single variable (x5).  The value of
        /// the Formula depends on the value of x5, which is determined by
        /// the delegate passed to Evaluate.  Since this delegate maps all
        /// variables to 22.5, the return value should be 22.5.
        /// </summary>
        [TestMethod]
        public void Evaluate2()
        {
            Formula f = new Formula("x5");
            Assert.AreEqual(f.Evaluate(v => 22.5), 22.5, 1e-6);
        }

        /// <summary>
        /// Here, the delegate passed to Evaluate always throws a
        /// UndefinedVariableException (meaning that no variables have
        /// values).  The test case checks that the result of
        /// evaluating the Formula is a FormulaEvaluationException.
        /// </summary>
        [TestMethod]
        [ExpectedException(typeof(FormulaEvaluationException))]
        public void Evaluate3()
        {
            Formula f = new Formula("x + y");
            f.Evaluate(v => { throw new UndefinedVariableException(v); });
        }

        /// <summary>
        /// The delegate passed to Evaluate is defined below.  We check
        /// that evaluating the formula returns in 10.
        /// </summary>
        [TestMethod]
        public void Evaluate4()
        {
            Formula f = new Formula("x + y");
            Assert.AreEqual(f.Evaluate(Lookup4), 10.0, 1e-6);
        }

        /// <summary>
        /// This uses one of each kind of token.
        /// </summary>
        [TestMethod]
        public void Evaluate5 ()
        {
            Formula f = new Formula("(x + y) * (z / x) * 1.0");
            Assert.AreEqual(f.Evaluate(Lookup4), 20.0, 1e-6);
        }

        /// <summary>
        /// This uses one of each kind of token.
        /// </summary>
        [TestMethod]
        public void Evaluate6()
        {
            Formula f = new Formula("(x + y) * (z / x) * 5.0");
            Assert.AreEqual(f.Evaluate(Lookup4), 100.0, 1e-6);
        }

        /// <summary>
        /// Divide by zero
        /// </summary>
        [TestMethod]
        [ExpectedException(typeof(FormulaEvaluationException))]
        public void Evaluate7()
        {
            Formula f = new Formula("(x + y) * (z /( 0.0* 17.5)) * 1.0");
            f.Evaluate(Lookup4);
        }

        /// <summary>
        /// Divide by zero
        /// </summary>
        [TestMethod]
        [ExpectedException(typeof(FormulaEvaluationException))]
        public void Evaluate8()
        {
            Formula f = new Formula("10/q");
            f.Evaluate(v => 0.0);
        }


        /// <summary>
        /// negative var val
        /// </summary>
        [TestMethod]
        public void Evaluate9()
        {
            Formula f = new Formula("20 - x");
            Assert.AreEqual(f.Evaluate(v => -25.0), 45.0, 1e-6);
        }

        /// <summary>
        /// negative var val, pos return val
        /// </summary>
        [TestMethod]
        public void Evaluate12()
        {
            Formula f = new Formula("15+ x");
            Assert.AreEqual(f.Evaluate(v => -25.0), -10.0, 1e-6);
        }

        /// <summary>
        /// negative var val, neg return val
        /// </summary>
        [TestMethod]
        public void Evaluate13()
        {
            Formula f = new Formula("( 10-45)- x");
            Assert.AreEqual(f.Evaluate(v => -25.0), -10.0, 1e-6);
        }

        /// <summary>
        /// negative return val
        /// </summary>
        [TestMethod]
        public void Evaluate10()
        {
            Formula f = new Formula("1.0 - 51");
            Assert.AreEqual(f.Evaluate(v => 0), -50.0, 1e-6);
        }

        /// <summary>
        /// subtraction
        /// </summary>
        [TestMethod]
        public void Evaluate11()
        {
            Formula f = new Formula("10-5");
            Assert.AreEqual(f.Evaluate(v => 0), 5.0, 1e-6);
        }



        /// <summary>
        /// test valid formulas
        /// </summary>
        [TestMethod]
        public void Evaluate14()
        {
            Formula f = new Formula("(2.5e9 + x5) / 17");
            Assert.AreEqual(f.Evaluate(v => 0), 147058823.529412, 1e-6);

            Formula g = new Formula("(5 * 2) + 8");
            Assert.AreEqual(g.Evaluate(v => 0), 18.0, 1e-6);

            Formula h = new Formula("x*y-2+35/9");
            Assert.AreEqual(h.Evaluate(Lookup4), 25.88888888888, 1e-6);
        }



        // "_"
        /////     "-5.3"
        /////     "2 5 + 3"

        /// <summary>
        /// test invalid formulas
        /// </summary>
        [TestMethod]
        [ExpectedException(typeof(FormulaFormatException))]
        public void Construct16()
        {
            Formula f = new Formula("_");

            Formula g = new Formula("-5.3");

            Formula h = new Formula("2 5 + 3");
        }

        /// <summary>
        /// A Lookup method that maps x to 4.0, y to 6.0, and z to 8.0.
        /// All other variables result in an UndefinedVariableException.
        /// </summary>
        /// <param name="v"></param>
        /// <returns></returns>
        public double Lookup4(String v)
        {
            switch (v)
            {
                case "x": return 4.0;
                case "y": return 6.0;
                case "z": return 8.0;
                default: throw new UndefinedVariableException(v);
            }
        }
    }
}
