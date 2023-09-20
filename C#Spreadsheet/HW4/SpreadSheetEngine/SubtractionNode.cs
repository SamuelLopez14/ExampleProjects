// <copyright file="SubtractionNode.cs" company="Samuel Lopez 11417858">
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
    /// Class representing the subtraction Node.
    /// </summary>
    public class SubtractionNode : OperatorNode
    {
/// <summary>
/// Initializes a new instance of the <see cref="SubtractionNode"/> class.
/// </summary>
/// <param name="c">Char repressenting subtraction.</param>
        public SubtractionNode(char c)
            : base(c)
        {
            this.OperatorChar = c;
            this.Precidence = 1;
            this.RightChild = this.LeftChild = null;
            this.Associative = 'L';
        }

        /// <summary>
        /// Evaluates the node.
        /// </summary>
        /// <returns>Double of evaluated node.</returns>
        public override double Evaluate()
        {
            return this.LeftChild.Evaluate() - this.RightChild.Evaluate();
        }
    }
}
