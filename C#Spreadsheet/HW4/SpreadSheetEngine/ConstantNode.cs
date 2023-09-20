// <copyright file="ConstantNode.cs" company="Samuel Lopez 11417858">
// Copyright (c) Samuel Lopez 11417858. All rights reserved.
// </copyright>

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CptS321
{
    /// <summary>
    /// The Node used for constants.
    /// </summary>
    public class ConstantNode : Node
    {
        /// <summary>
        /// This is the value that stores a double constant value.
        /// </summary>
        private double constant;

        /// <summary>
        /// Initializes a new instance of the <see cref="ConstantNode"/> class.
        /// </summary>
        /// <param name="value">Double value passed in.</param>
        public ConstantNode(double value)
        {
            this.constant = value;
        }

        /// <summary>
        /// Gets or sets constant.
        /// </summary>
        public double Constant
        {
            get
            {
                return this.constant;
            }

            set
            {
                this.constant = value;
            }
        }

        /// <summary>
        /// Evalutates the Constant Node.
        /// </summary>
        /// <returns>The Evaluated Node.</returns>
        public override double Evaluate()
        {
            return this.constant;
        }
    }
}
