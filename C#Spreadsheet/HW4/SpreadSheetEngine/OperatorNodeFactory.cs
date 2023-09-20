// <copyright file="OperatorNodeFactory.cs" company="Samuel Lopez 11417858">
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
    /// This is the class that stores a function that will return an operatorNode based on the input character.
    /// </summary>
   public class OperatorNodeFactory
    {
        /// <summary>
        /// returns operator node.
        /// </summary>
        /// <param name="c">The char representing the operator.</param>
        /// <returns>The corresponding operator node sub class.</returns>
        public static OperatorNode MakeOpNode(char c)
        {
            return new OperatorNode(c);
        }
    }
}
