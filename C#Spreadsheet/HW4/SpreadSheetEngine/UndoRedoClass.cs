// <copyright file="UndoRedoClass.cs" company="Samuel Lopez 11417858">
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
    /// Class Responsible for making undo/redo.
    /// </summary>
    public class UndoRedoClass
    {
        /// <summary>
        /// Holds the cell to be changed.
        /// </summary>
        private Cell cell;

        /// <summary>
        /// Initializes a new instance of the <see cref="UndoRedoClass"/> class.
        /// Constructs the undo instance.
        /// </summary>
        /// <param name="c">The cell.</param>
        /// <param name="type">The type.</param>
        /// <param name="text">The text.</param>
        /// <param name="color">The color.</param>
        public UndoRedoClass(Cell c, string type, string text, uint color)
        {
            this.cell = c;
            this.Type = type;
            this.Text = text;
            this.Color = color;
        }

        /// <summary>
        /// Gets or sets the type property.
        /// </summary>
        public string Type { get; set; }

        /// <summary>
        /// Gets or sets the texr proprty.
        /// </summary>
        public string Text { get; set; }

        /// <summary>
        /// Gets or sets the color property.
        /// </summary>
        public uint Color { get; set; }

        /// <summary>
        /// Gets or sets the current cell.
        /// </summary>
        public int ColorTimes { get; set; }

        /// <summary>
        /// Executes the undo/redo.
        /// </summary>
        public void Execute()
        {
            if (this.Type == "color")
            {
                this.cell.IsFromUndo = true;
                this.cell.BGCOLOR = this.Color;
                this.cell.ColorGroup = -1;
            }
            else if (this.Type == "text")
            {
                this.cell.IsFromUndo = true;
                this.cell.Text = this.Text;
            }
        }

        /// <summary>
        /// Returns opposite type.
        /// </summary>
        /// <returns>Opposite Undo Redo.</returns>
        public UndoRedoClass GetOpposite()
        {
            UndoRedoClass returnRedo = null;
            if (this.Type == "color")
            {
                returnRedo = new UndoRedoClass(this.cell, "color", this.cell.Text, this.cell.BGCOLOR);
            }
            else if (this.Type == "text")
            {
                returnRedo = new UndoRedoClass(this.cell, "text", this.cell.Text, this.cell.BGCOLOR);
            }

            return returnRedo;
        }
    }
}
