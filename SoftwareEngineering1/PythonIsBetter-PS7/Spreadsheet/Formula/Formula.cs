// Skeleton written by Joe Zachary for CS 3500, January 2019
// Filled in by Adam Gaia u0344827

using System;
using System.Text;
using System.Collections.Generic;
using System.Text.RegularExpressions;
using static Formulas.TokenType;

namespace Formulas
{
    /// <summary>
    /// Represents formulas written in standard infix notation using standard precedence
    /// rules.  Provides a means to evaluate Formulas.  Formulas can be composed of
    /// non-negative floating-point numbers, variables, left and right parentheses, and
    /// the four binary operator symbols +, -, *, and /.  (The unary operators + and -
    /// are not allowed.)
    /// </summary>
    public class Formula
    {

        public struct Token
        {
            public TokenType Type;
            public string Text;

            public Token(TokenType _type, string _text)
            {
                this.Type = _type;
                this.Text = _text;
            }
        }


        // List used to store tokens
        private List<Token> tokensList;

        // List of unique vars
        private HashSet<string> varList;

        // String representation for the toString() method
        private StringBuilder stringRepresentation;


        /// <summary>
        /// Creates a Formula from a string that consists of a standard infix expression composed
        /// from non-negative floating-point numbers (using C#-like syntax for double/int literals), 
        /// variable symbols (a letter followed by zero or more letters and/or digits), left and right
        /// parentheses, and the four binary operator symbols +, -, *, and /.  White space is
        /// permitted between tokens, but is not required.
        /// 
        /// Examples of a valid parameter to this constructor are:
        ///     "2.5e9 + x5 / 17"
        ///     "(5 * 2) + 8"
        ///     "x*y-2+35/9"
        ///     
        /// Examples of invalid parameters are:
        ///     "_"
        ///     "-5.3"
        ///     "2 5 + 3"
        /// 
        /// If the formula is syntacticaly invalid, throws a FormulaFormatException with an 
        /// explanatory Message.
        /// 
        /// Three paramater constructor. Extra inputs are a Normalizer and Validator.
        /// The purpose of a Normalizer is to convert variables into a canonical form.
        /// The purpose of a Validator is to impose extra restrictions on the validity of a variable, 
        /// beyond the ones already built into the Formula definition. 
        /// 
        /// </summary>
        public Formula(String formula)
        {
            // Check for null input
            if (formula.Equals(null))
            {
                throw new ArgumentNullException("Error, input null formula string");
            }


            // One paramater constructor calls the 3 paramater constructor with an
            // identity Normalizer method and a validator method that always 
            // returns true. 
            Normalizer N = s => s;
            Validator V = s => true;
            this.ConstructorHelper(formula, N, V);

        }

        /// <summary>
        /// Three paramater constructor allows N and V to be set
        /// </summary>
        /// <param name="f"></param>
        /// <param name="N"></param>
        /// <param name="V"></param>
        public Formula(string f, Normalizer N, Validator V)
        {
            // Check for null input
            if (f.Equals(null) || N.Equals(null) || V.Equals(null))
            {
                throw new ArgumentNullException("Error, null param");
            }


            this.ConstructorHelper(f, N, V);
        }


        /// <summary>
        /// Helper to do the constructing for the constructors
        /// </summary>
        /// <param name="f"></param>
        /// <param name="N"></param>
        /// <param name="V"></param>
        private void ConstructorHelper(string f, Normalizer N, Validator V)
        {

            this.varList = new HashSet<string>();
            this.stringRepresentation = new StringBuilder();


            IEnumerable<Token> tokens = GetTokens(f);

            // Create a list from the IEnumerable - while creating list, check if any tokens are invalid
            tokensList = new List<Token>();
            foreach (Token token in tokens)
            {

                if (token.Type.Equals(Invalid))
                {
                    throw new FormulaFormatException("Formula contains invalid token: " + token);
                }

                // If token is a variable
                if (token.Type.Equals(Var))
                {
                    // Prform variable validication check
                    checkVar(token.Text, N, V);
                    // If checks pass (no exception thrown), create new token, 
                    // with N(x) as the text value
                    string updatedVar = N(token.Text);
                    Token normToken = new Token(token.Type, updatedVar);
                    this.tokensList.Add(normToken);

                    // add the variable to the list of vars
                    this.varList.Add(updatedVar);

                    // upadate string representation
                    this.stringRepresentation.Append(updatedVar);

                }
                else // Token isnt a var, add it like normal
                {
                    this.tokensList.Add(token);

                    // upadate string representation
                    this.stringRepresentation.Append(token.Text);
                }
                
            }

            // Throw exception if empty formula
            if (tokensList.Count == 0)
            {
                throw new FormulaFormatException("Empty formula");
            }

            // Throw exception if the first token of a formula is not a number, a variable, or an opening parenthesis
            if (tokensList[0].Type.Equals(Oper) || tokensList[0].Type.Equals(RParen))
            {
                throw new FormulaFormatException("First token must be a number, a variable, or an opening parenthesis");
            }

            // Loop through each token
            int rightParenCount = 0;
            int leftParenCount = 0;
            int currentIdx = 0;
            foreach (Token token in tokens)
            {
                String currentToken = token.Text;
                TokenType currentTokenType = token.Type;

                // Count parentheses
                if (currentTokenType.Equals(RParen))
                {
                    rightParenCount++;
                }
                else if (currentTokenType.Equals(LParen))
                {
                    leftParenCount++;
                }

                // Throw exception if more closing parenth than opening
                if (rightParenCount > leftParenCount)
                {
                    throw new FormulaFormatException("Extra closing parenthesis");
                }

                // Make sure next token is valid after current token
                int size = currentIdx + 1;
                if (size < tokensList.Count)
                {
                    int nextIndex = size;
                    TokenType nextTokenType = tokensList[nextIndex].Type;

                    // Any token that immediately follows an opening parenthesis or an operator must be either a number, a variable, 
                    // or an opening parenthesis
                    if (currentTokenType.Equals(LParen) || currentTokenType.Equals(Oper))
                    {
                        if (!nextTokenType.Equals(Number) && !nextTokenType.Equals(Var) && !nextTokenType.Equals(LParen))
                        {
                            throw new FormulaFormatException("Error, token following a " + currentTokenType + " must be either a number, var, or opening parenthesis");
                        }

                    }
                    // Any token that immediately follows a number, a variable, or a closing parenthesis must be either an operator 
                    // or a closing parenthesis
                    //else if (currentTokenType.Equals(Number) || currentTokenType.Equals(Var) || currentTokenType.Equals(RParen))
                    else
                    {
                        if (!nextTokenType.Equals(Oper) && !nextTokenType.Equals(RParen))
                        {
                            throw new FormulaFormatException("Error, token following a " + currentTokenType + " must be either an operator or closing parenthesis");
                        }
                    }
                }

                currentIdx++;
            } // end foreach token

            // throw exception if total num of open parenth > total num closing parenth
            if (leftParenCount != rightParenCount)
            {
                throw new FormulaFormatException("Unequal number of opening and closing parenthesis");
            }

            // Throw exception if the last token of a formula is not a  number, a variable, or a closing parenthesis
            int endIdx = tokensList.Count - 1;
            if (tokensList[endIdx].Type.Equals(LParen) || tokensList[endIdx].Type.Equals(Oper))
            {
                throw new FormulaFormatException("Last token must be a number, a variable, or an opening parenthesis");
            }

        }

        /// <summary>
        /// Helper method to check if N(x) and V(N(x)) are vaild
        /// </summary>
        /// <param name="x"></param>
        /// <param name="N"></param>
        /// <param name="V"></param>
        private void checkVar(string x, Normalizer N, Validator V)
        {
            // Check that N(x) is valid
            string norm = N(x);
            String varPattern = @"[a-zA-Z][0-9a-zA-Z]*";
            Regex r = new Regex(varPattern);
            if (!r.IsMatch(norm))
            {
                throw new FormulaFormatException("N(x) is not valid");
            }

            // Check that V(N(x)) is valid
            bool validat = V(norm);
            if (!validat)
            {
                throw new FormulaFormatException("V(N(x)) is not valid");
            }
        }


        /// <summary>
        /// Evaluates this Formula, using the Lookup delegate to determine the values of variables.  (The
        /// delegate takes a variable name as a parameter and returns its value (if it has one) or throws
        /// an UndefinedVariableException (otherwise).  Uses the standard precedence rules when doing the evaluation.
        /// 
        /// If no undefined variables or divisions by zero are encountered when evaluating 
        /// this Formula, its value is returned.  Otherwise, throws a FormulaEvaluationException  
        /// with an explanatory Message.
        /// </summary>
        public double Evaluate(Lookup lookup)
        {

            // Check for null input
            if (lookup.Equals(null))
            {
                throw new ArgumentNullException("Error, null lookup");
            }


            Stack<double> valueStack = new Stack<double>();
            Stack<string> operatorStack = new Stack<string>();

            // Loop through tokens
            foreach (Token token in tokensList)
            {
                string currentTokenData = token.Text;
                TokenType currentTokenType = token.Type;

                // Check token type
                switch (currentTokenType)
                {
                    case LParen:
                        operatorStack.Push(currentTokenData);
                        break;

                    case RParen:
                        if (operatorStack.Count > 0 && (operatorStack.Peek().Equals("+") || operatorStack.Peek().Equals("-")))
                        {
                            stackHelper(ref valueStack, ref operatorStack);
                        }
                        operatorStack.Pop();
                        if (operatorStack.Count > 0 && (operatorStack.Peek().Equals("*") || operatorStack.Peek().Equals("/")))
                        {
                            string op = operatorStack.Pop();
                            double b = valueStack.Pop();
                            double a = valueStack.Pop();
                            double result = performCalculation(a, b, op);
                            valueStack.Push(result);
                        }
                        break;

                    case Oper:
                        if (currentTokenData.Equals("*") || currentTokenData.Equals("/"))
                        {
                            operatorStack.Push(currentTokenData);
                        }
                        else // currentTokenData is + or -
                        {
                            if (operatorStack.Count > 0 && (operatorStack.Peek().Equals("+") || operatorStack.Peek().Equals("-")))
                            {
                                stackHelper(ref valueStack, ref operatorStack);
                            }
                            operatorStack.Push(currentTokenData);
                        }
                        break;

                    case Var:
                        try
                        {
                            double lookupValue = lookup(currentTokenData);
                            numberOperation(ref valueStack, ref operatorStack, lookupValue);
                        }
                        catch (UndefinedVariableException)
                        {
                            throw new FormulaEvaluationException("Undefined variable, " + currentTokenData);
                        }
                        break;

                    case Number:
                        double value = Convert.ToDouble(currentTokenData);
                        numberOperation(ref valueStack, ref operatorStack, value);
                        break;
                } // end switch

            } // end foreach tooken

            // if oper stack is empty, value stack contains answer
            if (operatorStack.Count == 0)
            {
                return valueStack.Pop();
            }

            // else preform one last calculation and return
            double finalB = valueStack.Pop();
            double finalA = valueStack.Pop();
            string lastOpper = operatorStack.Pop();
            return performCalculation(finalA, finalB, lastOpper);
        }


        /// <summary>
        /// Helper method for Evaluate() to take care of numbers
        /// Eliminates the redundancy of case var and case number types
        /// </summary>
        /// <param name="valStack"></param>
        /// <param name="opStack"></param>
        /// <param name="t"></param>
        private void numberOperation(ref Stack<double> valStack, ref Stack<string> opStack, double t)
        {
            //If * or / is at the top of the operator stack, pop the value stack, 
            //pop the operator stack, and apply the popped operator to t and the popped number. 
            //Push the result onto the value stack.
            //Otherwise, push t onto the value stack
            if (opStack.Count > 0 && (opStack.Peek().Equals("*") || opStack.Peek().Equals("/")))
            {
                string op = opStack.Pop();
                double a = valStack.Pop();
                double result = performCalculation(a, t, op);
                valStack.Push(result);
            }
            else
            {
                valStack.Push(t);
            }
        }

        /// <summary>
        /// Private method to preform operations on the stack
        /// Eliminates redundences with op +- and RParn cases
        /// </summary>
        private void stackHelper(ref Stack<double> valStack, ref Stack<string> opStack)
        {
            double val2 = valStack.Pop();
            double val1 = valStack.Pop();
            string op = opStack.Pop();

            // preform operation on values
            double result = performCalculation(val1, val2, op);

            valStack.Push(result);
        }

        /// <summary>
        /// Private method to preform operations for the Evaluate() method
        /// </summary>
        /// <returns></returns>
        private double performCalculation(double a, double b, string op)
        {

           if (op.Equals("+"))
            {
                return a + b;
            }
           if (op.Equals("-"))
            {
                return a - b;
            }
           if (op.Equals("*"))
            {
                return a * b;
            }
           if (op.Equals("/"))
            {
                if (b == 0)
                {
                    throw new FormulaEvaluationException("Error: division by 0");
                }
                return a / b;
            }

            // Shouldnt get this far
            throw new InvalidOperationException("Unknown operator type in performCalculation()");

        }


        /// <summary>
        /// Returns HashSet of unique variables found in formula
        /// </summary>
        /// <returns></returns>
        public ISet<string> GetVariables()
        {
            return this.varList;
        }

        public override string ToString()
        {
            return this.stringRepresentation.ToString();
        }



        /// <summary>
        /// Given a formula, enumerates the tokens that compose it.  Each token is described by a
        /// Tuple containing the token's text and TokenType.  There are no empty tokens, and no
        /// token contains white space.
        /// </summary>
        private static IEnumerable<Token> GetTokens(String formula)
        {
            // Patterns for individual tokens.
            String lpPattern = @"\(";
            String rpPattern = @"\)";
            String opPattern = @"[\+\-*/]";
            String varPattern = @"[a-zA-Z][0-9a-zA-Z]*";

            // NOTE:  I have added white space to this regex to make it more readable.
            // When the regex is used, it is necessary to include a parameter that says
            // embedded white space should be ignored.  See below for an example of this.
            String doublePattern = @"(?: \d+\.\d* | \d*\.\d+ | \d+ ) (?: e[\+-]?\d+)?";
            String spacePattern = @"\s+";

            // Overall token pattern.  It contains embedded white space that must be ignored when
            // it is used.  See below for an example of this.
            String tokenPattern = String.Format("({0}) | ({1}) | ({2}) | ({3}) | ({4}) | ({5}) | (.)",
                                            spacePattern, lpPattern, rpPattern, opPattern, varPattern, doublePattern);

            // Create a Regex for matching tokens.  Notice the second parameter to Split says 
            // to ignore embedded white space in the pattern.
            Regex r = new Regex(tokenPattern, RegexOptions.IgnorePatternWhitespace);

            // Look for the first match
            Match match = r.Match(formula);

            // Start enumerating tokens
            while (match.Success)
            {
                // Ignore spaces
                if (!match.Groups[1].Success)
                {
                    // Holds the token's type
                    TokenType type;

                    if (match.Groups[2].Success)
                    {
                        type = LParen;
                    }
                    else if (match.Groups[3].Success)
                    {
                        type = RParen;
                    }
                    else if (match.Groups[4].Success)
                    {
                        type = Oper;
                    }
                    else if (match.Groups[5].Success)
                    {
                        type = Var;
                    }
                    else if (match.Groups[6].Success)
                    {
                        type = Number;
                    }
                    else if (match.Groups[7].Success)
                    {
                        type = Invalid;
                    }
                    else
                    {
                        // We shouldn't get here
                        throw new InvalidOperationException("Regular exception failed in GetTokens");
                    }

                    // Yield the token
                    yield return new Token(type, match.Value);
                }

                // Look for the next match
                match = match.NextMatch();
            }
        }
    }

    /// <summary>
    /// Identifies the type of a token.
    /// </summary>
    public enum TokenType
    {
        /// <summary>
        /// Left parenthesis
        /// </summary>
        LParen,

        /// <summary>
        /// Right parenthesis
        /// </summary>
        RParen,

        /// <summary>
        /// Operator symbol
        /// </summary>
        Oper,

        /// <summary>
        /// Variable
        /// </summary>
        Var,

        /// <summary>
        /// Double literal
        /// </summary>
        Number,

        /// <summary>
        /// Invalid token
        /// </summary>
        Invalid
    };

    /// <summary>
    /// A Lookup method is one that maps some strings to double values.  Given a string,
    /// such a function can either return a double (meaning that the string maps to the
    /// double) or throw an UndefinedVariableException (meaning that the string is unmapped 
    /// to a value. Exactly how a Lookup method decides which strings map to doubles and which
    /// don't is up to the implementation of the method.
    /// </summary>
    public delegate double Lookup(string var);

    /// <summary>
    /// 
    /// </summary>
    /// <param name="s"></param>
    /// <returns></returns>
    public delegate string Normalizer(string s);

    /// <summary>
    /// 
    /// </summary>
    /// <param name="s"></param>
    /// <returns></returns>
    public delegate bool Validator(string s);

    /// <summary>
    /// Used to report that a Lookup delegate is unable to determine the value
    /// of a variable.
    /// </summary>
    [Serializable]
    public class UndefinedVariableException : Exception
    {
        /// <summary>
        /// Constructs an UndefinedVariableException containing whose message is the
        /// undefined variable.
        /// </summary>
        /// <param name="variable"></param>
        public UndefinedVariableException(String variable)
            : base(variable)
        {
        }
    }

    /// <summary>
    /// Used to report syntactic errors in the parameter to the Formula constructor.
    /// </summary>
    [Serializable]
    public class FormulaFormatException : Exception
    {
        /// <summary>
        /// Constructs a FormulaFormatException containing the explanatory message.
        /// </summary>
        public FormulaFormatException(String message) : base(message)
        {
        }
    }

    /// <summary>
    /// Used to report errors that occur when evaluating a Formula.
    /// </summary>
    [Serializable]
    public class FormulaEvaluationException : Exception
    {
        /// <summary>
        /// Constructs a FormulaEvaluationException containing the explanatory message.
        /// </summary>
        public FormulaEvaluationException(String message) : base(message)
        {
        }
    }
}
