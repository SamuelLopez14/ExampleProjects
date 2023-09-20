// <copyright file="AdditionNode.cs" company="Samuel Lopez 11417858">
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
    /// Node representing the addition Node.
    /// </summary>
    public class AdditionNode : OperatorNode
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="AdditionNode"/> class.
        /// Creates an addition node.
        /// </summary>
        /// <param name="c">The char representing the operation.</param>
        public AdditionNode(char c)
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
        /// <returns>The evaluated value.</returns>
        public override double Evaluate()
        {
            return this.LeftChild.Evaluate() + this.RightChild.Evaluate();
        }
    }
}
