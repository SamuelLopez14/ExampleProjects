// <copyright file="OperatorFactory.cs" company="Samuel Lopez 11417858">
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
    /// holds the function that makes new operator nodes.
    /// </summary>
   public class OperatorFactory
    {
        /// <summary>
        /// Function that makes the correct node.
        /// </summary>
        /// <param name="c">Char representing the operation.</param>
        /// <returns>Corresponding OperatorNode.</returns>
        public static OperatorNode MakeOperator(char c)
        {
            if (c == '+')
            {
                return new AdditionNode('+');
            }
            else if (c == '-')
            {
                return new SubtractionNode('-');
            }
            else if (c == '*')
            {
                return new MultiplicationNode('*');
            }
            else if (c == '/')
            {
                return new DivisionNode('/');
            }
            else
            {
                return null;
            }
        }
    }
}
