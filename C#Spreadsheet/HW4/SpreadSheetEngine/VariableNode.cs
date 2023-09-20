// <copyright file="VariableNode.cs" company="Samuel Lopez 11417858">
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
    /// The Class that holds variables.
    /// </summary>
    public class VariableNode : Node
    {
        /// <summary>
        /// Field that represents the input variables.
        /// </summary>
        private string variable;

        /// <summary>
        /// Links to the expression tree variable dictionary.
        /// </summary>
        private Dictionary<string, double> variableDic;

        /// <summary>
        /// Initializes a new instance of the <see cref="VariableNode"/> class.
        /// </summary>
        /// <param name="value">The input string representing a variable.</param>
        /// /// <param name="dictionary">The expression trees variable dictionary.</param>
        public VariableNode(string value, Dictionary<string, double> dictionary)
        {
            this.variable = value;
            this.variableDic = dictionary;
        }

       /// <summary>
       /// Gets or sets the variable.
       /// </summary>
        public string Variable
        {
            get
            {
                return this.variable;
            }

            set
            {
                this.variable = value;
            }
        }

        /// <summary>
        /// evaluates the current node.
        /// </summary>
        /// <returns>The evaluated Node value.</returns>
        public override double Evaluate()
        {
                return this.variableDic[this.variable];
        }
    }
}
