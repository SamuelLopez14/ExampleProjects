// <copyright file="SpreadSheet.cs" company="Samuel Lopez 11417858">
// Copyright (c) Samuel Lopez 11417858. All rights reserved.
// </copyright>

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

namespace CptS321
{
    /// <summary>
    /// The Spreadsheet class for the assignment.
    /// </summary>
    public class SpreadSheet : INotifyPropertyChanged
    {
        /// <summary>
        /// The actual cells of the spreadsheet.
        /// </summary>
        private PrivateCell[,] cells;

        /// <summary>
        /// Stack that holds undo commands.
        /// </summary>
        private Stack<UndoRedoClass> undos;

        /// <summary>
        /// Stack that holds redo commands.
        /// </summary>
        private Stack<UndoRedoClass> redos;

        /// <summary>
        /// The rows of the spreadsheet.
        /// </summary>
        private int rows;

        /// <summary>
        /// The columns of the spreadsheet.
        /// </summary>
        private int columns;

        /// <summary>
        /// Initializes a new instance of the <see cref="SpreadSheet"/> class.
        /// </summary>
        /// <param name="a">num rows.</param>
        /// <param name="b">Number of Columns.</param>
        public SpreadSheet(int a, int b)
        {
            this.rows = a;
            this.columns = b;
            this.cells = new PrivateCell[a, b];
            for (int i = 0; i < a; i++)
            {
                for (int j = 0; j < b; j++)
                {
                    this.cells[i, j] = new PrivateCell(i, j);
                    this.cells[i, j].PropertyChanged += this.CellPropertyChanged;
                }
            }

            this.undos = new Stack<UndoRedoClass>();
            this.redos = new Stack<UndoRedoClass>();
        }

        /// <summary>
        /// The property handler to use in the winform app.
        /// </summary>
        public event PropertyChangedEventHandler PropertyChanged = (sender, e) => { };

        /// <summary>
        /// Used to set the text in a specific cell index.
        /// </summary>
        /// <param name="row">Row Index.</param>
        /// <param name="column">Column Index.</param>
        /// <param name="message">The input string.</param>
        public void SetCell(int row, int column, string message)
        {
            this.cells[row, column].Text = message;
        }

        /// <summary>
        /// This should respond to each individual cells property change event.
        /// </summary>
        /// <param name="sender">The cell that sent.</param>
        /// <param name="e">The event args.</param>
        public void CellPropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            string cellText = (sender as PrivateCell).Text, rowNumber;

            if (e.PropertyName == "color")
            {
                if ((sender as Cell).IsFromUndo == true)
                {
                    (sender as Cell).IsFromUndo = false;
                }
                else
                {
                    this.undos.Push(new UndoRedoClass(sender as Cell, "color", "color", (sender as Cell).OldColor));
                    this.redos = new Stack<UndoRedoClass>();
                }

                this.PropertyChanged(sender, e);
                return;
            }

            if ((sender as Cell).IsFromUndo == true || e.PropertyName == "Dependency Changed")
            {
                (sender as Cell).IsFromUndo = false;
            }
            else
            {
                this.undos.Push(new UndoRedoClass(sender as Cell, "text", (sender as Cell).OldText, (sender as Cell).BGCOLOR));
                this.redos = new Stack<UndoRedoClass>();
            }

            char columnLetter = ' ';
            if (cellText == null || cellText == string.Empty || cellText[0] != '=')
            {
                (sender as PrivateCell).RemoveAllDependencies();
                (sender as PrivateCell).Value = cellText;
                this.PropertyChanged(sender, new PropertyChangedEventArgs("text"));
            }
            else
            {
                if ((cellText[1] >= 'A' && cellText[1] <= 'Z') && !this.IsEquationString(cellText))
                {
                    if (!this.IsCellValid(cellText.Substring(1)))
                    {
                        (sender as PrivateCell).RemoveAllDependencies();
                        (sender as PrivateCell).Value = "!(Bad Input)";
                    }
                    else
                    {
                        columnLetter = cellText[1];
                        rowNumber = cellText.Substring(2);

                        (sender as PrivateCell).RemoveAllDependencies();

                        (sender as PrivateCell).AddDependency(this.cells[Convert.ToInt32(rowNumber) - 1, (int)(columnLetter - 'A')]);
                        if (!this.CheckForCircularReference(sender as Cell, new List<Cell>()))
                        {
                            (sender as PrivateCell).RemoveAllDependencies();
                            (sender as PrivateCell).Value = "!(BadReference)";
                        }
                        else
                        {
                            (sender as PrivateCell).Value = this.cells[Convert.ToInt32(rowNumber) - 1, (int)(columnLetter - 'A')].Value;
                        }
                    }
                }
                else
                {
                    ExpressionTree exp = new ExpressionTree(cellText.Substring(1));
                    if (!this.AreMultipleCellsValid(exp.VariableList))
                    {
                        (sender as PrivateCell).RemoveAllDependencies();
                        (sender as PrivateCell).Value = "!(Bad Input)";
                    }
                    else
                    {
                        exp.SetDictWithVariableList(this.cells);
                        (sender as PrivateCell).RemoveAllDependencies();
                        List<string> cellList = exp.VariableList;
                        foreach (string name in cellList)
                        {
                            (sender as PrivateCell).AddDependency(this.cells[Convert.ToInt32(name.Substring(1)) - 1, (int)(name[0] - 'A')]);
                        }

                        if (!this.CheckForCircularReference(sender as Cell, new List<Cell>()))
                        {
                            (sender as PrivateCell).RemoveAllDependencies();
                            (sender as PrivateCell).Value = "!(BadReference)";
                        }
                        else
                        {
                            (sender as PrivateCell).Value = exp.Evaluate().ToString();
                        }
                    }
                }
            }

            this.PropertyChanged(sender, e);
        }

        /// <summary>
        /// Returns the number of columns.
        /// </summary>
        /// <returns>Number of Columns.</returns>
        public int ColumnCount()
        {
            return this.columns;
        }

        /// <summary>
        /// Checks a string to see if it's a valid cell.
        /// </summary>
        /// <param name="s">String.</param>
        /// <returns>True if valid.</returns>
        public bool IsCellValid(string s)
        {
            int row = 0;
            if (s[0] < 'A' || s[0] > 'Z')
            {
                return false;
            }

            try
            {
                 row = Convert.ToInt32(s.Substring(1));
            }
            catch
            {
                return false;
            }

            if (row > this.rows)
            {
                return false;
            }

            return true;
        }

        /// <summary>
        /// Checks the list of strings to make sure all are possible cells.
        /// </summary>
        /// <param name="variables">List of strings representing cells.</param>
        /// <returns>True if all cells valis.</returns>
        public bool AreMultipleCellsValid(List<string> variables)
        {
            foreach (string cell in variables)
            {
                if (!this.IsCellValid(cell))
                {
                    return false;
                }
            }

            return true;
        }

        /// <summary>
        /// Checks the cell for a circular reference.
        /// </summary>
        /// <param name="cell">Cell to be checked.</param>
        /// <param name="previouscells">List of previous cells.</param>
        /// <returns>True if not circular reference.</returns>
        public bool CheckForCircularReference(Cell cell, List<Cell> previouscells)
        {
            if (cell.Text == null || cell.Text == string.Empty || cell.Text[0] != '=')
            {
                return true;
            }

            List<Cell> innenChecklist = new List<Cell>(previouscells), tempCheckList;
            innenChecklist.Add(cell);
            foreach (Cell dependentCell in cell.Dependencies)
            {
                innenChecklist.Add(dependentCell);
                if (innenChecklist.Count() != innenChecklist.Distinct().Count())
                {
                    return false;
                }

                tempCheckList = new List<Cell>(previouscells);
                tempCheckList.Add(cell);

                if (!this.CheckForCircularReference(dependentCell, new List<Cell>(tempCheckList)))
                {
                    return false;
                }
            }

            return true;
        }

        /// <summary>
        /// Checks to see if a string contains operands.
        /// </summary>
        /// <param name="g">String input.</param>
        /// <returns>True if it contains operands.</returns>
        public bool IsEquationString(string g)
        {
            char[] operands = { '+', '-', '*', '/' };
            foreach (char operand in operands)
            {
                if (g.Contains(operand))
                    {
                    return true;
                }
            }

            return false;
        }

        /// <summary>
        /// Can see if undo stack is empty.
        /// </summary>
        /// <returns>Bool.</returns>
        public bool IsUndosEmpty()
        {
            return this.undos.Count == 0;
        }

        /// <summary>
        /// Checks if redo stack is empty.
        /// </summary>
        /// <returns>Bool yes or no.</returns>
        public bool IsRedoEmpty()
        {
            return this.redos.Count == 0;
        }

        /// <summary>
        /// Undos recent change.
        /// </summary>
        public void Undo()
        {
            UndoRedoClass temp;
            if (this.undos.Peek().Type == "color")
            {
                int numTimes = this.undos.Peek().ColorTimes;
                for (int i = 0; i < numTimes; i++)
                {
                    temp = this.undos.Pop();
                    this.redos.Push(temp.GetOpposite());
                    temp.Execute();
                }

                this.redos.Peek().ColorTimes = numTimes;
                return;
            }

            temp = this.undos.Pop();
            this.redos.Push(temp.GetOpposite());
            temp.Execute();
        }

        /// <summary>
        /// Pops undo stack.
        /// </summary>
        public void PopUndo()
        {
            this.undos.Pop();
        }

        /// <summary>
        /// Implements a redo.
        /// </summary>
        public void Redo()
        {
            UndoRedoClass temp;
            if (this.redos.Peek().Type == "color")
            {
                int numTimes = this.redos.Peek().ColorTimes;
                for (int i = 0; i < numTimes; i++)
                {
                    temp = this.redos.Pop();
                    this.undos.Push(temp.GetOpposite());
                    temp.Execute();
                }

                this.undos.Peek().ColorTimes = numTimes;
                return;
            }

            temp = this.redos.Pop();
            this.undos.Push(temp.GetOpposite());
            temp.Execute();
        }

        /// <summary>
        /// Sets the top undos time.
        /// </summary>
        /// <param name="n">Number of cells changed.</param>
        public void SetTopUndoNumber(int n)
        {
            this.undos.Peek().ColorTimes = n;
        }

        /// <summary>
        /// Pops Redo Stack.
        /// </summary>
        public void RedoPop()
        {
            this.redos.Pop();
        }

        /// <summary>
        /// sees the top undo type.
        /// </summary>
        /// <returns>The type of top undo.</returns>
        public string UndoType()
        {
            if (this.undos.Count > 0)
            {
                UndoRedoClass temp = this.undos.Peek();
                return temp.Type;
            }

            return "empty";
        }

        /// <summary>
        /// Checks top redo type.
        /// </summary>
        /// <returns>String representing top redo type.</returns>
        public string RedoType()
        {
            UndoRedoClass temp = this.redos.Peek();
            return temp.Type;
        }

        /// <summary>
        /// Returns number of rows.
        /// </summary>
        /// <returns>Number of Rows.</returns>
        public int RowCount()
        {
            return this.rows;
        }

        /// <summary>
        /// Gets the corresponding cell.
        /// </summary>
        /// <param name="row">Row Index.</param>
        /// <param name="column">Column Index.</param>
        /// <returns>Corresponding Cell.</returns>
        public Cell GetCell(int row, int column)
        {
            if (row >= this.rows || column >= this.columns || row < 0 || column < 0)
            {
                return null;
            }
            else
            {
                return this.cells[row, column];
            }
        }

        /// <summary>
        /// This is the function that saves the spreadsheet.
        /// </summary>
        /// <param name="saveFile"> A filestream recieved from the form.</param>
        public void SaveSheet(Stream saveFile)
        {
            XmlWriter saveWriter = XmlWriter.Create(saveFile);
            saveWriter.WriteStartElement("Spreadsheet");
            foreach (Cell cell in this.cells)
            {
                if (cell.Text != string.Empty || cell.BGCOLOR != 0xFFFFFFFF)
                {
                    saveWriter.WriteStartElement("Cell");
                    saveWriter.WriteAttributeString("row", cell.RowIndex.ToString());
                    saveWriter.WriteAttributeString("column", cell.ColumnIndex.ToString());
                    if (cell.Text != string.Empty)
                    {
                        saveWriter.WriteAttributeString("text", cell.Text);
                    }

                    if (cell.BGCOLOR != 0xFFFFFFFF)
                    {
                        saveWriter.WriteAttributeString("color", cell.BGCOLOR.ToString());
                    }

                    saveWriter.WriteEndElement();
                }
            }

            saveWriter.WriteEndElement();
            saveWriter.WriteEndDocument();
            saveWriter.Close();
        }

        /// <summary>
        /// Clears the entire sheet, used when loading.
        /// </summary>
        public void Clear()
        {
            foreach (Cell cell1 in this.cells)
            {
                if (cell1.Text == null || cell1.Text == string.Empty || cell1.Text[0] != '=')
                {
                    continue;
                }
                else
                {
                    cell1.Text = string.Empty;
                    cell1.BGCOLOR = 0xFFFFFFFF;
                }
            }

            foreach (Cell cell in this.cells)
            {
                cell.Text = string.Empty;
                cell.BGCOLOR = 0xFFFFFFFF;
            }
        }

        /// <summary>
        /// The function that saves the sheet.
        /// </summary>
        /// <param name="loadFile">The filestream to be loaded from.</param>
        public void LoadSheet(Stream loadFile)
        {
            this.Clear();
            Dictionary<Cell, string> dependentCells = new Dictionary<Cell, string>();
            XmlReader loadSheet = XmlReader.Create(loadFile);
            while (loadSheet.Read())
            {
                if (loadSheet.NodeType == XmlNodeType.Element && loadSheet.Name == "Cell")
                {
                    int row = Convert.ToInt32(loadSheet.GetAttribute("row"));
                    int column = Convert.ToInt32(loadSheet.GetAttribute("column"));
                    Cell cell = this.GetCell(row, column);
                    if (loadSheet.GetAttribute("text") != null)
                    {
                        if (loadSheet.GetAttribute("text") == null || loadSheet.GetAttribute("text") == string.Empty || loadSheet.GetAttribute("text")[0] != '=')
                        {
                            cell.Text = loadSheet.GetAttribute("text");
                        }
                        else if (loadSheet.GetAttribute("text")[0] == '=')
                        {
                            dependentCells[cell] = loadSheet.GetAttribute("text");
                        }
                        else
                        {
                            cell.Text = "RuhRohRaggy";
                        }
                    }

                    if (loadSheet.GetAttribute("color") != null)
                    {
                        cell.BGCOLOR = Convert.ToUInt32(loadSheet.GetAttribute("color"));
                    }
                }
            }

            loadSheet.Close();
            foreach (KeyValuePair<Cell, string> entry in dependentCells)
            {
                entry.Key.Text = entry.Value;
            }

            this.undos = new Stack<UndoRedoClass>();
            this.redos = new Stack<UndoRedoClass>();
        }
    }
}
