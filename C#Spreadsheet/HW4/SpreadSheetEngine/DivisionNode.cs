// <copyright file="DivisionNode.cs" company="Samuel Lopez 11417858">
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
    /// Represents the Division Node.
    /// </summary>
    public class DivisionNode : OperatorNode
    {
/// <summary>
/// Initializes a new instance of the <see cref="DivisionNode"/> class.
/// </summary>
/// <param name="c">Char representing subtraction.</param>
        public DivisionNode(char c)
            : base(c)
        {
            this.OperatorChar = c;
            this.Precidence = 1;
            this.RightChild = this.LeftChild = null;
            this.Associative = 'L';
        }

        /// <summary>
        /// Evaluates the Node.
        /// </summary>
        /// <returns>Double representing the evaluated node.</returns>
        public override double Evaluate()
        {
            if (this.RightChild.Evaluate() == 0)
            {
                throw new DivideByZeroException("Can't Have Zero As a Divisor");
            }
            else
            {
                return this.LeftChild.Evaluate() / this.RightChild.Evaluate();
            }
        }
    }
}
