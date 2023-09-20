// <copyright file="OperatorNode.cs" company="Samuel Lopez 11417858">
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
    /// This is the class holding operators in the expression tree.
    /// </summary>
    public abstract class OperatorNode : Node
    {
        /// <summary>
        /// char representing the operator.
        /// </summary>
        private char operatorChar;

        /// <summary>
        /// Left operand.
        /// </summary>
        private Node leftChild;

        /// <summary>
        /// Right Operand.
        /// </summary>
        private Node rightChild;

        /// <summary>
        /// Initializes a new instance of the <see cref="OperatorNode"/> class.
        /// </summary>
        /// <param name="c">Char repressenting a mathematical operator.</param>
        public OperatorNode(char c)
        {
            this.operatorChar = c;
            this.leftChild = null;
            this.rightChild = null;
        }

        /// <summary>
        /// Gets or Sets operatorchar.
        /// </summary>
        public char OperatorChar
        {
            get
            {
                return this.operatorChar;
            }

            set
            {
                this.operatorChar = value;
            }
        }

        /// <summary>
        /// Gets or sets leftChild.
        /// </summary>
        public Node LeftChild
        {
            get
            {
                return this.leftChild;
            }

            set
            {
                this.leftChild = value;
            }
        }

        /// <summary>
        /// Gets or sets rightChild.
        /// </summary>
        public Node RightChild
        {
            get
            {
                return this.rightChild;
            }

            set
            {
                this.rightChild = value;
            }
        }

        /// <summary>
        /// Gets or sets the char representing the assocaitivity of the operator.
        /// </summary>
        public char Associative { get; set; }

        /// <summary>
        /// Gets or sets the int representing presidence.
        /// </summary>
        public int Precidence { get; set; }
    }
}
