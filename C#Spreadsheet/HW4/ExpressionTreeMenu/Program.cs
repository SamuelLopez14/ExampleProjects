// <copyright file="Program.cs" company="Samuel Lopez 11417858">
// Copyright (c) Samuel Lopez 11417858. All rights reserved.
// </copyright>

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ExpressionTreeMenu
{
    /// <summary>
    /// The default program of the expression tree menu.
    /// </summary>
    public class Program
    {
        /// <summary>
        /// Main program.
        /// </summary>
        /// <param name="args">Input arguments.</param>
        public static void Main(string[] args)
        {
            CptS321.ExpressionTree menuTree = new CptS321.ExpressionTree("A1+B1+C1");
            string userInput = string.Empty;
            string userInput2 = string.Empty;
            double userDouble = 0;

            while (true)
            {
                Console.WriteLine("Current Expression is: " + menuTree.Expression);
                Console.WriteLine("Please Select an option\n1. Enter New Expression\n2. Set Variable Value\n3. Evaluate\n4. Quit");
                userInput = Console.ReadLine();
                if (userInput == "1")
                {
                    Console.WriteLine("Please Enter New Expression");
                    userInput = Console.ReadLine();
                    menuTree = new CptS321.ExpressionTree(userInput);
                }
                else if (userInput == "2")
                {
                    Console.WriteLine("Please input the key");
                    userInput = Console.ReadLine();
                    Console.WriteLine("Please enter the value");
                    userInput2 = Console.ReadLine();
                    double.TryParse(userInput2, out userDouble);
                    menuTree.SetVariable(userInput, userDouble);
                }
                else if (userInput == "3")
                {
                    Console.WriteLine(menuTree.Evaluate());
                }
                else if (userInput == "4")
                {
                    break;
                }
            }
        }
    }
}
