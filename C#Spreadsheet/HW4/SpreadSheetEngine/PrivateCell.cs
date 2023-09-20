// <copyright file="PrivateCell.cs" company="Samuel Lopez 11417858">
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
    /// This should be the inherited class that only the spreadsheet class can use.
    /// </summary>
    internal class PrivateCell : Cell
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="PrivateCell"/> class.
        /// </summary>
        /// <param name="a"> row Index.</param>
        /// <param name="b">column Index.</param>
        public PrivateCell(int a, int b)
            : base(a, b)
        {
        }

        /// <summary>
        /// Gets or sets the value field.
        /// </summary>
        internal new string Value
        {
            get
            {
                return this.value;
            }

            set
            {
                this.value = value;
            }
        }
    }
}
