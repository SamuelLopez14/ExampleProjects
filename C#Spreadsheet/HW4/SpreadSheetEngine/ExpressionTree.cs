// <copyright file="ExpressionTree.cs" company="Samuel Lopez 11417858">
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
    /// This class represents the expression tree used to evaluate expressions in the spreadsheet application.
    /// </summary>
    public class ExpressionTree
    {
        /// <summary>
        /// The string form of the expression tree.
        /// </summary>
        private string expression;

        /// <summary>
        /// The root node of the expression tree.
        /// </summary>
        private Node root;

        /// <summary>
        /// The tree that holds the variable values.
        /// </summary>
        private Dictionary<string, double> variableDic;

        /// <summary>
        /// This is the List that holds the variables used in the list.
        /// </summary>
        private List<string> variableList;

/// <summary>
/// Initializes a new instance of the <see cref="ExpressionTree"/> class.
/// </summary>
/// <param name="s">The input string.</param>
        public ExpressionTree(string s)
        {
            this.variableList = new List<string>();
            this.variableDic = new Dictionary<string, double>();
            this.expression = s;
            this.CompileTree(s);
        }

        /// <summary>
        /// Gets the variableList field.
        /// </summary>
        public List<string> VariableList
        {
            get
            {
                return this.variableList;
            }
        }

        /// <summary>
        /// Gets the expression.
        /// </summary>
        public string Expression
        {
            get
            {
                return this.expression;
            }
        }

        /// <summary>
        /// Evaluates the tree and returns the value.
        /// </summary>
        /// <returns>The evaluated double from the tree.</returns>
        public double Evaluate()
        {
            return this.root.Evaluate();
        }

        /// <summary>
        /// Updates the variable dictionary.
        /// </summary>
        /// <param name="variableName">The key string.</param>
        /// <param name="variableValue">The double value.</param>
        public void SetVariable(string variableName, double variableValue)
        {
            this.variableDic[variableName] = variableValue;
        }

        /// <summary>
        /// Used in the spreadsheet application to set the dictionary.
        /// </summary>
        /// <param name="spreadsheetcell">2D array of cells.</param>
        public void SetDictWithVariableList(Cell[,] spreadsheetcell)
        {
            foreach (string name in this.variableList)
            {
                int rowIndex = name[0] - 'A';
                string tempString = name.Substring(1);
                int columnIndex = Convert.ToInt32(tempString);
                double dictValue = 0;
                if (double.TryParse(spreadsheetcell[columnIndex - 1, rowIndex].Value, out dictValue))
                {
                    this.variableDic[name] = dictValue;
                }
            }
        }

        private double EvaluateHelper(Node n)
        {
            ConstantNode tempConstant = n as ConstantNode;
            if (tempConstant != null)
            {
                return tempConstant.Constant;
            }

            VariableNode tempVariable = n as VariableNode;
            if (tempVariable != null)
            {
                if (this.variableDic.ContainsKey(tempVariable.Variable))
                {
                    return this.variableDic[tempVariable.Variable];
                }
                else
                {
                    return 0;
                }
            }

            OperatorNode tempOperator = n as OperatorNode;
            if (tempOperator != null)
            {
                if (tempOperator.OperatorChar == '+')
                {
                    return this.EvaluateHelper(tempOperator.LeftChild) + this.EvaluateHelper(tempOperator.RightChild);
                }
                else if (tempOperator.OperatorChar == '-')
                {
                    return this.EvaluateHelper(tempOperator.LeftChild) - this.EvaluateHelper(tempOperator.RightChild);
                }
                else if (tempOperator.OperatorChar == '*')
                {
                    return this.EvaluateHelper(tempOperator.LeftChild) * this.EvaluateHelper(tempOperator.RightChild);
                }
                else if (tempOperator.OperatorChar == '/')
                {
                    return this.EvaluateHelper(tempOperator.LeftChild) / this.EvaluateHelper(tempOperator.RightChild);
                }
            }

            return 0;
        }

        private void CompileTree(string s)
        {
            Stack<char> operatorStack = new Stack<char>();
            List<string> postOrderList = new List<string>();
            char[] operators = { '+', '-', '*', '/' };
            string tempString = string.Empty;
            OperatorNode tempOperator;
            double tempValue;

            for (int i = 0; i < s.Length; i++)
            {
                if (s[i] == '(')
                {
                    operatorStack.Push(s[i]);
                }
                else if (s[i] == ')')
                {
                    if (tempString != string.Empty)
                    {
                        postOrderList.Add(tempString);
                        tempString = string.Empty;
                    }

                    while (operatorStack.Peek() != '(')
                    {
                        postOrderList.Add(operatorStack.Pop().ToString());
                    }

                    operatorStack.Pop();
                }
                else if (operators.Contains(s[i]) && s[i - 1] != 'E')
                {
                    if (tempString != string.Empty)
                    {
                        postOrderList.Add(tempString);
                        tempString = string.Empty;
                    }

                    if (operatorStack.Count == 0 || operatorStack.Peek() == '(')
                    {
                        operatorStack.Push(s[i]);
                    }
                    else
                    {
                        while (this.GetPrecidenceOfChar(s[i]) <= this.GetPrecidenceOfChar(operatorStack.Peek()))
                        {
                            postOrderList.Add(operatorStack.Pop().ToString());
                            if (operatorStack.Count == 0)
                            {
                                break;
                            }
                        }

                        operatorStack.Push(s[i]);
                    }
                }
                else
                {
                    tempString = tempString + s[i];
                }
            }

            if (tempString != string.Empty)
            {
                postOrderList.Add(tempString);
                tempString = string.Empty;
            }

            while (operatorStack.Count != 0)
            {
                postOrderList.Add(operatorStack.Pop().ToString());
            }

            Stack<Node> nodeStack = new Stack<Node>();
            foreach (string listString in postOrderList)
            {
                if (listString.Length == 1 && operators.Contains(listString[0]))
                {
                    tempOperator = OperatorFactory.MakeOperator(listString[0]);
                    tempOperator.RightChild = nodeStack.Pop();
                    tempOperator.LeftChild = nodeStack.Pop();
                    nodeStack.Push(tempOperator);
                }
                else
                {
                    if (double.TryParse(listString, out tempValue))
                    {
                        nodeStack.Push(new ConstantNode(tempValue));
                    }
                    else
                    {
                        nodeStack.Push(new VariableNode(listString, this.variableDic));
                        this.variableList.Add(listString);
                    }
                }
            }

            this.root = nodeStack.Pop();
       }

        private int GetPrecidenceOfChar(char c)
        {
            if (c == '*' || c == '/')
            {
                return 2;
            }

            if (c == '+' || c == '-')
            {
                return 1;
            }

            return 0;
        }
    }
}
