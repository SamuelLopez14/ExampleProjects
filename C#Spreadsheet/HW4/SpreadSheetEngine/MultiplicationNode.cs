// <copyright file="MultiplicationNode.cs" company="Samuel Lopez 11417858">
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
    /// Class representing the multiplication node.
    /// </summary>
   public class MultiplicationNode : OperatorNode
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="MultiplicationNode"/> class.
        /// </summary>
        /// <param name="c">Char representing multiplication.</param>
        public MultiplicationNode(char c)
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
            return this.LeftChild.Evaluate() * this.RightChild.Evaluate();
        }
    }
}
