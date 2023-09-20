// <copyright file="Node.cs" company="Samuel Lopez 11417858">
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
    /// This is the abstract class from which all Nodes are based off of.
    /// </summary>
    public abstract class Node
    {
        /// <summary>
        /// Evaluates the current Node.
        /// </summary>
        /// <returns>Evaluated value as a double.</returns>
        public abstract double Evaluate();
    }
}
