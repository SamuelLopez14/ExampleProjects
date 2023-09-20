// <copyright file="Cell.cs" company="Samuel Lopez 11417858">
// Copyright (c) Samuel Lopez 11417858. All rights reserved.
// </copyright>

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CptS321
{
    /// <summary>
    /// The cell class used to represent a cell in the spreadsheet.
    /// </summary>
    public abstract class Cell : INotifyPropertyChanged
    {
        /// <summary>
        /// Int that keeps track of the row index of the cell.
        /// </summary>
        protected int rowIndex;

        /// <summary>
        /// Int that keeps track of the column index of the cell.
        /// </summary>
        protected int columnIndex;

        /// <summary>
        /// A string that represents the typed in text.
        /// </summary>
        protected string text;

        /// <summary>
        /// A string that repressents the cell value.
        /// </summary>
        protected string value;

        /// <summary>
        /// Holds the other cells this one is dependent on.
        /// </summary>
        protected List<Cell> dependencyList;

        /// <summary>
        /// Protected variable that stores the background color.
        /// </summary>
        protected uint bGColor;

        /// <summary>
        /// Initializes a new instance of the <see cref="Cell"/> class.
        /// The default constructor of the Cell class.
        /// </summary>
        /// <param name="a">The supplied row index.</param>
        /// <param name="b">The supplied column index. </param>
        public Cell(int a, int b)
        {
            this.rowIndex = a;
            this.columnIndex = b;
            this.dependencyList = new List<Cell>();

            this.value = string.Empty;
            this.text = string.Empty;
            this.bGColor = 0xFFFFFFFF;
            this.IsFromUndo = false;
            this.ColorGroup = -1;
        }

        /// <summary>
        /// The event Handler for the Cell Class.
        /// </summary>
        public event PropertyChangedEventHandler PropertyChanged = (sender, e) => { };

        /// <summary>
        /// Gets the rowIndex field.
        /// </summary>
        public int RowIndex
        {
            get => this.rowIndex;
        }

        /// <summary>
        /// Gets the columnIndex field.
        /// </summary>
        public int ColumnIndex
        {
            get => this.columnIndex;
        }

        /// <summary>
        /// Gets or sets and Sets the text string.
        /// </summary>
        public string Text
        {
            get
            {
                return this.text;
            }

            set
            {
                if (value == this.text)
                {
                    return;
                }

                this.OldText = this.text;
                this.text = value;
                this.PropertyChanged(this, new PropertyChangedEventArgs(this.rowIndex.ToString() + " " + this.columnIndex.ToString()));
            }
        }

        /// <summary>
        /// Gets the value string.
        /// </summary>
        public string Value
        {
            get
            {
                return this.value;
            }
        }

        /// <summary>
        /// Gets or sets the old text string.
        /// </summary>
        public string OldText { get; set; }

        /// <summary>
        /// Gets or sets the old colorvalue.
        /// </summary>
        public uint OldColor { get; set; }

        /// <summary>
        /// Gets or Sets the bGcolor field.
        /// </summary>
        public uint BGCOLOR
        {
            get
            {
                return this.bGColor;
            }

            set
            {
                if (this.bGColor == value)
                {
                    return;
                }
                else
                {
                    this.OldColor = this.bGColor;
                    this.bGColor = value;
                    this.PropertyChanged(this, new PropertyChangedEventArgs("color"));
                }
            }
        }

        /// <summary>
        /// Gets or sets a value indicating whether sees if recent changes are from an undo.
        /// </summary>
        public bool IsFromUndo { get; set; }

        /// <summary>
        /// Gets or sets color group number.
        /// </summary>
        public int ColorGroup { get; set; }

        /// <summary>
        /// Gets the dependency list.
        /// </summary>
        public List<Cell> Dependencies
        {
            get
            {
                return this.dependencyList;
            }
        }

        /// <summary>
        /// Triggers the cell to reevaluate in case of dependency change.
        /// </summary>
        /// <param name="sender">The object that triggered the event.</param>
        /// <param name="e">String representing the argument.</param>
        public void DependentCellChanged(object sender, PropertyChangedEventArgs e)
        {
            this.OldText = this.text;
            this.PropertyChanged(this, new PropertyChangedEventArgs("Dependency Changed"));
        }

        /// <summary>
        /// This should go through the list and remove all of the dependicies.
        /// </summary>
        public void RemoveAllDependencies()
        {
            foreach (Cell cell in this.dependencyList)
            {
                cell.PropertyChanged -= this.DependentCellChanged;
            }

            this.dependencyList = new List<Cell>();
        }

        /// <summary>
        /// Adds a dependent cell to the list.
        /// </summary>
        /// <param name="c">A cell that this cell depends on.</param>
        public void AddDependency(Cell c)
        {
            this.dependencyList.Add(c);
            c.PropertyChanged += this.DependentCellChanged;
        }
    }
}
