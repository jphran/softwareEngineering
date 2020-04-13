using System;
using System.Collections.Generic;
using System.IO;
using System.Text.RegularExpressions;
using System.Xml;
using System.Xml.Schema;
using Dependencies;
using Formulas;

namespace SS
{
    /// <summary>
    /// An AbstractSpreadsheet object represents the state of a simple spreadsheet.  A 
    /// spreadsheet consists of an infinite number of named cells.
    /// 
    /// A string s is a valid cell name if and only if it consists of one or more letters, 
    /// followed by a non-zero digit, followed by zero or more digits.
    /// 
    /// For example, "A15", "a15", "XY32", and "BC7" are valid cell names.  On the other hand, 
    /// "Z", "X07", and "hello" are not valid cell names.
    /// 
    /// A spreadsheet contains a unique cell corresponding to each possible cell name.  
    /// In addition to a name, each cell has a contents and a value.  The distinction is
    /// important, and it is important that you understand the distinction and use
    /// the right term when writing code, writing comments, and asking questions.
    /// 
    /// The contents of a cell can be (1) a string, (2) a double, or (3) a Formula.  If the
    /// contents is an empty string, we say that the cell is empty.  (By analogy, the contents
    /// of a cell in Excel is what is displayed on the editing line when the cell is selected.)
    /// 
    /// In an empty spreadsheet, the contents of every cell is the empty string.
    ///  
    /// The value of a cell can be (1) a string, (2) a double, or (3) a FormulaError.  
    /// (By analogy, the value of an Excel cell is what is displayed in that cell's position
    /// in the grid.)
    /// 
    /// If a cell's contents is a string, its value is that string.
    /// 
    /// If a cell's contents is a double, its value is that double.
    /// 
    /// If a cell's contents is a Formula, its value is either a double or a FormulaError.
    /// The value of a Formula, of course, can depend on the values of variables.  The value 
    /// of a Formula variable is the value of the spreadsheet cell it names (if that cell's 
    /// value is a double) or is undefined (otherwise).  If a Formula depends on an undefined
    /// variable or on a division by zero, its value is a FormulaError.  Otherwise, its value
    /// is a double, as specified in Formula.Evaluate.
    /// 
    /// Spreadsheets are never allowed to contain a combination of Formulas that establish
    /// a circular dependency.  A circular dependency exists when a cell depends on itself.
    /// For example, suppose that A1 contains B1*2, B1 contains C1*2, and C1 contains A1*2.
    /// A1 depends on B1, which depends on C1, which depends on A1.  That's a circular
    /// dependency.
    /// </summary>
    public class Spreadsheet : AbstractSpreadsheet
    {

        // Store collection of cells as a hashmap (dictionary) for O(1) time complexity
        private Dictionary<string, Cell> cells;

        // use dependency graph to keep track of cell relations
        private DependencyGraph dg;

        // Regex to check validity of cell names
        private Regex isValidRegex;

        // Bool to hold status for this.Changed
        private Boolean changedSinceSaveOrCreate;

        /// <summary>
        /// // True if this spreadsheet has been modified since it was created or saved
        /// (whichever happened most recently); false otherwise.
        /// </summary>
        public override bool Changed
        {
            get => this.changedSinceSaveOrCreate;
            protected set => this.changedSinceSaveOrCreate = value;
        }


        /// <summary>
        /// Creates an empty Spreadsheet whose IsValid regular expression accepts every string.
        /// </summary>
        public Spreadsheet()
        {
            cells = new Dictionary<string, Cell>();
            dg = new DependencyGraph();
            this.Changed = false;
            this.isValidRegex = new Regex("(.*?)"); // Match anything
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="isValid"></param>
        public Spreadsheet(Regex isValid)
        {
            cells = new Dictionary<string, Cell>();
            dg = new DependencyGraph();
            this.Changed = false;
            this.isValidRegex = isValid;
        }


        /// <summary>
        /// Creates a Spreadsheet that is a duplicate of the spreadsheet saved in source.
        ///
        /// See the AbstractSpreadsheet.Save method and Spreadsheet.xsd for the file format 
        /// specification.  
        ///
        /// If there's a problem reading source, throws an IOException.
        ///
        /// Else if the contents of source are not consistent with the schema in Spreadsheet.xsd, 
        /// throws a SpreadsheetReadException.  
        ///
        /// Else if the IsValid string contained in source is not a valid C# regular expression, throws
        /// a SpreadsheetReadException.  (If the exception is not thrown, this regex is referred to
        /// below as oldIsValid.)
        ///
        /// Else if there is a duplicate cell name in the source, throws a SpreadsheetReadException.
        /// (Two cell names are duplicates if they are identical after being converted to upper case.)
        ///
        /// Else if there is an invalid cell name or an invalid formula in the source, throws a 
        /// SpreadsheetReadException.  (Use oldIsValid in place of IsValid in the definition of 
        /// cell name validity.)
        ///
        /// Else if there is an invalid cell name or an invalid formula in the source, throws a
        /// SpreadsheetVersionException.  (Use newIsValid in place of IsValid in the definition of
        /// cell name validity.)
        ///
        /// Else if there's a formula that causes a circular dependency, throws a SpreadsheetReadException. 
        ///
        /// Else, create a Spreadsheet that is a duplicate of the one encoded in source except that
        /// the new Spreadsheet's IsValid regular expression should be newIsValid.
        /// </summary>
        /// <param name="source"></param>
        /// <param name="newIsValid"></param>
        public Spreadsheet(TextReader source, Regex newIsValid)
        {
            // Read file
            string data;
            try
            {
                data = source.ReadToEnd();
                source.Close();
            }
            catch (Exception)
            {
                throw new IOException();
            }

            // Create new spreadsheet parts
            cells = new Dictionary<string, Cell>();
            dg = new DependencyGraph();
            this.isValidRegex = newIsValid;
            Regex oldIsValid = new Regex(""); // Create with empty string for now. This will get reinitialized later

            // Use XMLReader to parse data
            Dictionary<string, string> cellsFromXML = new Dictionary<string, string>();
            using (XmlReader reader = XmlReader.Create(new StringReader(data)))
            {
                while (reader.Read())
                {
                    if (reader.IsStartElement())
                    {
                        int spreadSheetCount = 0;


                        switch (reader.Name)
                        {
                            case "spreadsheet":
                                spreadSheetCount++;
                                try
                                {
                                    oldIsValid = new Regex(reader["IsValid"]);

                                }
                                catch (Exception)
                                {
                                    throw new SpreadsheetReadException("Invalid regex loaded from xml file");
                                }
                                if (spreadSheetCount > 1)
                                {
                                    throw new SpreadsheetReadException("Source XML file does not match Spreadsheet.xsd");
                                }
                                break;

                            case "cell":

                                string cellName = reader["name"];
                                string cellContent = reader["contents"];

                                // Check for duplicate
                                if (cells.ContainsKey(cellName))
                                {
                                    throw new SpreadsheetReadException("Duplicate cell " + cellName + " in xml file");
                                }
                                else
                                {
                                    // Validate with oldIsValid
                                    if (!oldIsValid.IsMatch(cellName)) // Notice: if NOT
                                    {
                                        throw new SpreadsheetReadException("Cell");
                                    }

                                    // Add cell to spreadsheet
                                    // Validate with new isValid happens in this processs
                                    // Validation of contents happens in this process
                                    try
                                    {
                                        this.SetContentsOfCell(cellName, cellContent);
                                    }
                                    catch (InvalidNameException)
                                    {
                                        throw new SpreadsheetVersionException("invalid name");
                                    }
                                    catch (Exception)
                                    {
                                        throw new SpreadsheetReadException("Source XML file does not match Spreadsheet.xsd");
                                    }
                                }

                                break;

                            default:
                                
                                throw new SpreadsheetVersionException("Error adding cell to spreadsheet. Exception: ");
                        }
                    }
                }
            }

            this.Changed = false;
        }


        /// <summary>
        /// Helper method to check if a cell's name is valid.
        /// Throws an InvalidNameException if the name is null or invalid
        /// </summary>
        /// <param name="name"></param>
        /// <returns></returns>
        private bool checkName(string name)
        {
            if (name == null)
            {
                throw new InvalidNameException();
            }
            // Check default format
            string regexPattern = "[a-z|A-Z]+ [1-9] [0-9]*";
            Regex r = new Regex(regexPattern, RegexOptions.IgnorePatternWhitespace);
            if (!r.IsMatch(name)) // Notice: if NOT match
            {
                throw new InvalidNameException();
            }

            // Check provided format with this.isValidRegex
            if (!isValidRegex.IsMatch(name)) // Notice: if NOT match
            {
                throw new InvalidNameException();
            }
            // Both checks pass, safe to return true
            return true;
        }

        /// <summary>
        /// If name is null or invalid, throws an InvalidNameException.
        /// 
        /// Otherwise, returns the contents (as opposed to the value) of the named cell.  The return
        /// value should be either a string, a double, or a Formula.
        /// </summary>
        public override object GetCellContents(string name)
        {

            name = name.ToUpper();

            // use helper method to check name and throw exception if needed.
            checkName(name);

            // Check if cell exists in cells (AKA contents have been set)
            if (this.cells.TryGetValue(name, out Cell cell))
            {

                return cell.content;
            }
            else // cell doesn't exist in cells
            {
                // Contents of this cell havent been set. Return empty string
                return "";
            }

        }


        /// <summary>
        /// Enumerates the names of all the non-empty cells in the spreadsheet.
        /// </summary>
        public override IEnumerable<string> GetNamesOfAllNonemptyCells()
        {
            // Cells dictionary only keeps track of non-empty cells.
            // Thus, we can simply return all keys in cells dictionary
            return this.cells.Keys;
        }


        /// <summary>
        /// If content is null, throws an ArgumentNullException.
        ///
        /// Otherwise, if name is null or invalid, throws an InvalidNameException.
        ///
        /// Otherwise, if content parses as a double, the contents of the named
        /// cell becomes that double.
        ///
        /// Otherwise, if content begins with the character '=', an attempt is made
        /// to parse the remainder of content into a Formula f using the Formula
        /// constructor with s => s.ToUpper() as the normalizer and a validator that
        /// checks that s is a valid cell name as defined in the AbstractSpreadsheet
        /// class comment.  There are then three possibilities:
        ///
        ///   (1) If the remainder of content cannot be parsed into a Formula, a
        ///       Formulas.FormulaFormatException is thrown.
        ///
        ///   (2) Otherwise, if changing the contents of the named cell to be f
        ///       would cause a circular dependency, a CircularException is thrown.
        ///
        ///   (3) Otherwise, the contents of the named cell becomes f.
        ///
        /// Otherwise, the contents of the named cell becomes content.
        ///
        /// If an exception is not thrown, the method returns a set consisting of
        /// name plus the names of all other cells whose value depends, directly
        /// or indirectly, on the named cell.
        ///
        /// For example, if name is A1, B1 contains A1*2, and C1 contains B1+A1, the
        /// set {A1, B1, C1} is returned.
        /// </summary>
        /// <param name="name"></param>
        /// <param name="content"></param>
        /// <returns></returns>
        public override ISet<string> SetContentsOfCell(string name, string content)
        {
            if (content == null)
            {
                throw new ArgumentNullException();
            }

            name = name.ToUpper();

            // Call helper method to check name
            checkName(name);

            ISet<string> returnSet;

            // Determine type of content
            if (double.TryParse(content, out double convertedDouble))
            {
                returnSet = SetCellContents(name, convertedDouble);
            }
            else if (content.Length > 0 && content[0] == '=')
            {
                Formula f = new Formula(content.Substring(1), s => s.ToUpper(), s => this.isValidRegex.IsMatch(s));
                returnSet = SetCellContents(name, f);
            }
            else
            {
                returnSet = SetCellContents(name, content);
            }

            // Update value of all cells that depend on this cell
            foreach (string cellName in returnSet)
            {
                this.updateCellValue(cellName);
            }
            return returnSet;
        }


        /// <summary>
        /// If name is null or invalid, throws an InvalidNameException.
        /// 
        /// Otherwise, the contents of the named cell becomes number.  The method returns a
        /// set consisting of name plus the names of all other cells whose value depends, 
        /// directly or indirectly, on the named cell.
        /// 
        /// For example, if name is A1, B1 contains A1*2, and C1 contains B1+A1, the
        /// set {A1, B1, C1} is returned.
        /// </summary>
        protected override ISet<string> SetCellContents(string name, double number)
        {
            object oldContent;

            // Check if cell exists in cells (AKA contents have been set)
            if (this.cells.TryGetValue(name, out Cell cell))
            {
                oldContent = cell.content;

                // Check if changing to new value
                if (cell.value == null || !number.Equals(cell.value))
                {
                    this.Changed = true;
                }


                // update cell
                cell.content = number;
            }
            else // cell doesn't exist in cells
            {
                oldContent = "";

                this.Changed = true;
                // create new cell and add to cells dict.
                Cell newCell = new Cell(number);
                cells.Add(name, newCell);
            }

            // Create and return set of dependees
            HashSet<string> returnSet = new HashSet<string>();
            try
            {
                returnSet.UnionWith(GetCellsToRecalculate(name));
            }
            catch (CircularException e)
            {
                // Revert
                this.Changed = false;
                this.SetContentsOfCell(name, oldContent.ToString());



                throw e;
            }

            return returnSet;

        }



        /// <summary>
        /// If text is null, throws an ArgumentNullException.
        /// 
        /// Otherwise, if name is null or invalid, throws an InvalidNameException.
        /// 
        /// Otherwise, the contents of the named cell becomes text.  The method returns a
        /// set consisting of name plus the names of all other cells whose value depends, 
        /// directly or indirectly, on the named cell.
        /// 
        /// For example, if name is A1, B1 contains A1*2, and C1 contains B1+A1, the
        /// set {A1, B1, C1} is returned.
        /// </summary>
        protected override ISet<string> SetCellContents(string name, string text)
        {
            name = name.ToUpper();

            object oldContent;

            // Check if cell exists in cells (AKA contents have been set)
            if (this.cells.TryGetValue(name, out Cell cell))
            {
                oldContent = cell.content;

                // Check if changing to new value
                if (cell.value == null || !text.Equals(cell.value))
                {
                    this.Changed = true;
                }

                // update cell. 
                if (text.Equals(""))
                {
                    //If text is empty string, remove from dictionary
                    this.cells.Remove(name);
                }
                else
                {
                    cell.content = text;
                }
            }
            else if (!text.Equals("")) // Cell doesnt exist, and new cell necessary
            {
                oldContent = "";

                this.Changed = true;
                // create new cell and add to cells dict.
                Cell newCell = new Cell(text);
                cells.Add(name, newCell);
            }
            else // cell doesn't exist in cells and no new cell necessary
            {
                oldContent = "";
            }

            // Create and return set of dependees
            HashSet<string> returnSet = new HashSet<string>();
            try
            {
                returnSet.UnionWith(GetCellsToRecalculate(name));
            }
            catch (CircularException e)
            {
                // Revert
                this.Changed = false;
                this.SetContentsOfCell(name, oldContent.ToString());

                throw e;
            }
            return returnSet;

        }



        /// <summary>
        /// Requires that all of the variables in formula are valid cell names.
        /// 
        /// If name is null or invalid, throws an InvalidNameException.
        /// 
        /// Otherwise, if changing the contents of the named cell to be the formula would cause a 
        /// circular dependency, throws a CircularException.
        /// 
        /// Otherwise, the contents of the named cell becomes formula.  The method returns a
        /// Set consisting of name plus the names of all other cells whose value depends,
        /// directly or indirectly, on the named cell.
        /// 
        /// For example, if name is A1, B1 contains A1*2, and C1 contains B1+A1, the
        /// set {A1, B1, C1} is returned.
        /// </summary>
        protected override ISet<string> SetCellContents(string name, Formula formula)
        {
            object oldContent;

            // Update dependencies
            foreach (string var in formula.GetVariables())
            {
                this.dg.AddDependency(var, name);
            }

            // Check if cell exists in cells (AKA contents have been set)
            if (this.cells.TryGetValue(name, out Cell cell))
            {
                oldContent = cell.content;

                // Check if changing to new value
                if (cell.value == null || !formula.Equals(cell.content))
                {
                    this.Changed = true;
                }


                // update cell
                cell.content = formula;
            }
            else // cell doesn't exist in cells
            {
                oldContent = "";

                this.Changed = true;

                // create new cell and add to cells dict.
                Cell newCell = new Cell(formula);
                cells.Add(name, newCell);
            }

            // Create and return set of dependees
            HashSet<string> returnSet = new HashSet<string>();
            try
            {
                returnSet.UnionWith(GetCellsToRecalculate(name));
            }
            catch (CircularException e)
            {
                this.Changed = false;
                // remove dependencies
                foreach (string var in formula.GetVariables())
                {
                    this.dg.RemoveDependency(var, name);
                }

                // Revert
                this.SetContentsOfCell(name, oldContent.ToString());

                throw e;
            }
            
            return returnSet;
        }


        /// <summary>
        /// If name is null, throws an ArgumentNullException.
        /// 
        /// Otherwise, if name isn't a valid cell name, throws an InvalidNameException.
        /// 
        /// Otherwise, returns an enumeration, without duplicates, of the names of all cells whose
        /// values depend directly on the value of the named cell.  In other words, returns
        /// an enumeration, without duplicates, of the names of all cells that contain
        /// formulas containing name.
        /// 
        /// For example, suppose that
        /// A1 contains 3
        /// B1 contains the formula A1 * A1
        /// C1 contains the formula B1 + A1
        /// D1 contains the formula B1 - C1
        /// The direct dependents of A1 are B1 and C1
        /// </summary>
        protected override IEnumerable<string> GetDirectDependents(string name)
        {
            if (name == null)
            {
                throw new ArgumentNullException();
            }

            // Use helper method to check name
            checkName(name);

            return dg.GetDependents(name);
        }

        // TODO remove out this method and comment out test
        /// <summary>
        /// 
        /// </summary>
        /// <param name="name"></param>
        /// <returns></returns>
        public IEnumerable<string> testGetDirectDependants(string name)
        {
            return GetDirectDependents(name);
        }


        /// <summary>
        /// Writes the contents of this spreadsheet to dest using an XML format.
        /// The XML elements should be structured as follows:
        ///
        /// <spreadsheet IsValid="IsValid regex goes here">
        ///   <cell name="cell name goes here" contents="cell contents go here"></cell>
        ///   <cell name="cell name goes here" contents="cell contents go here"></cell>
        ///   <cell name="cell name goes here" contents="cell contents go here"></cell>
        /// </spreadsheet>
        ///
        /// The value of the IsValid attribute should be IsValid.ToString()
        /// 
        /// There should be one cell element for each non-empty cell in the spreadsheet.
        /// If the cell contains a string, the string (without surrounding double quotes) should be written as the contents.
        /// If the cell contains a double d, d.ToString() should be written as the contents.
        /// If the cell contains a Formula f, f.ToString() with "=" prepended should be written as the contents.
        ///
        /// If there are any problems writing to dest, the method should throw an IOException.
        /// </summary>
        /// <param name="dest"></param>
        public override void Save(TextWriter dest)
        {
            try
            {
                dest.WriteLine("<?xml version=\"1.0\" encoding=\"utf - 8\" ?>");

                string regex = this.isValidRegex.ToString();
                dest.WriteLine("<spreadsheet IsValid=\"" + regex + "\">");

                foreach (string cellName in this.GetNamesOfAllNonemptyCells())
                {
                    dest.Write("  <cell name=\"" + cellName + "\" contents=\"");

                    Object contents = this.GetCellContents(cellName);
                    if (contents is String)
                    {
                        dest.Write(contents);

                    }
                    else if (contents is double)
                    {
                        dest.Write(contents.ToString());
                    }
                    else // contents is Formula
                    {
                        dest.Write("=" + contents.ToString());
                    }
                    dest.Write("\"></cell>\n");
                }

                dest.WriteLine("</spreadsheet>");

                dest.Close();

                this.Changed = false;

            }
            catch (Exception)
            {
                throw new IOException();
            }
        }


        /// <summary>
        /// If name is null or invalid, throws an InvalidNameException.
        ///
        /// Otherwise, returns the value (as opposed to the contents) of the named cell.  The return
        /// value should be either a string, a double, or a FormulaError.
        /// </summary>
        /// <param name="name"></param>
        /// <returns></returns>
        public override object GetCellValue(string name)
        {
            // Use helper method to check name
            checkName(name);

            if (this.cells.TryGetValue(name, out Cell cell))
            {
                return cell.value;
            }
            else // contents havent been set
            {
                return "";
            }
        }


        /// <summary>
        /// Helper method to value of a cell by evaluating the content formula
        /// </summary>
        /// <param name="name"></param>
        private void updateCellValue(string name)
        {
            if (this.cells.TryGetValue(name, out Cell cell))
            {
                if (cell.content is double || cell.content is string)
                {
                    cell.updateValue(cell.content);
                    return;
                }

                Formula cellContent = (Formula)cell.content;
                try
                {
                    double newValue = cellContent.Evaluate(s => (double)this.GetCellValue(s));
                    cell.updateValue(newValue);
                }
                catch (Exception e)
                {
                    object newValue =  new FormulaError(e.ToString());
                    cell.updateValue(newValue);
                }
            }

        }

    }


    /// <summary>
    /// Basic unit of a spreadsheet.
    /// </summary>
    class Cell
    {
        public object content;
        public object value;

        public Cell(object _content)
        {
            this.content = _content;
            this.value = null;
        }

        public void updateValue(object newValue)
        {
            this.value = newValue;
        }
    }

}
