// <copyright file="Tests.cs" company="Samuel Lopez 11417858">
// Copyright (c) Samuel Lopez 11417858. All rights reserved.
// </copyright>

using System.Globalization;
using System.IO;
using NUnit.Framework;

namespace CptS321.Tests
{
    /// <summary>
    /// The Default Test Class.
    /// </summary>
    public class Tests
    {
        /// <summary>
        /// The default test set up function.
        /// </summary>
        [SetUp]
        public void Setup()
        {
        }

        /// <summary>
        /// Tests the spreadsheet class get row and column functions.
        /// </summary>
        [Test]
        public void TestGetRowAndColumn()
        {
            SpreadSheet testSheet = new SpreadSheet(5, 4);
            Assert.AreEqual(5, testSheet.RowCount());
            Assert.AreEqual(4, testSheet.ColumnCount());
        }

        /// <summary>
        /// Tests the basic expression tree.
        /// </summary>
        [Test]

        public void TestbasicTreeAddition()
        {
            CptS321.ExpressionTree test = new CptS321.ExpressionTree("2+3+4");
            Assert.AreEqual(9D, test.Evaluate());
        }

        /// <summary>
        /// Tests the basic multiplication features.
        /// </summary>
        [Test]

        public void TestbasicTreeMulti()
        {
            CptS321.ExpressionTree test = new CptS321.ExpressionTree("2*3*4");
            Assert.AreEqual(24D, test.Evaluate());
        }

        /// <summary>
        /// Tests the basic subtraction features.
        /// </summary>
        [Test]

        public void TestbasicTreeSubtraction()
        {
            CptS321.ExpressionTree test = new CptS321.ExpressionTree("8-3-2");
            Assert.AreEqual(3D, test.Evaluate());
        }

        /// <summary>
        /// Tests the basic multiplication features.
        /// </summary>
        [Test]

        public void TestbasicTreeDiv()
        {
            CptS321.ExpressionTree test = new CptS321.ExpressionTree("5/2/2");
            Assert.AreEqual(1.25D, test.Evaluate());
        }

        /// <summary>
        /// Tests evaluate with one number input.
        /// </summary>
        [Test]

        public void TestbasicTreeOneNum()
        {
            CptS321.ExpressionTree test = new CptS321.ExpressionTree("234");
            Assert.AreEqual(234D, test.Evaluate());
        }

        /// <summary>
        /// Tests the basic addition for undefined variable.
        /// </summary>
        [Test]

        public void TestUnsignedVariable()
        {
            CptS321.ExpressionTree test = new CptS321.ExpressionTree("AA+BB");
            Assert.That(() => test.Evaluate(), Throws.TypeOf<System.Collections.Generic.KeyNotFoundException>());
        }

        /// <summary>
        /// Tests addition for assigned variables.
        /// </summary>
        [Test]

        public void TestAssignedVariable()
        {
            CptS321.ExpressionTree test = new CptS321.ExpressionTree("AA+BB");
            test.SetVariable("AA", 10D);
            test.SetVariable("BB", 11D);
            Assert.AreEqual(21D, test.Evaluate());
        }

        /// <summary>
        /// This tests the max value as an edge case.
        /// </summary>
        [Test]
        public void TestMax()
        {
            CptS321.ExpressionTree test = new CptS321.ExpressionTree("1.7976931348623157E+308+1.7976931348623157E+308");
            Assert.AreEqual(double.PositiveInfinity, test.Evaluate());
        }

        /// <summary>
        /// Tests Parenthesis Functions.
        /// </summary>
        [Test]
        public void TestParenthesis()
        {
            CptS321.ExpressionTree test = new CptS321.ExpressionTree("(2+2)*(2+2)*(2+2)");
            Assert.AreEqual(64D, test.Evaluate());
        }

        /// <summary>
        /// Tests order of operations.
        /// </summary>
        [Test]
        public void TestOrderofOperations()
        {
            CptS321.ExpressionTree test = new CptS321.ExpressionTree("1+5*2/5+4-3");
            Assert.AreEqual(4D, test.Evaluate());
        }

        /// <summary>
        /// Tests order of operations.
        /// </summary>
        [Test]
        public void TestDivideByZeroException()
        {
            CptS321.ExpressionTree test = new CptS321.ExpressionTree("1/0");
            Assert.That(() => test.Evaluate(), Throws.TypeOf<System.DivideByZeroException>());
        }

        /// <summary>
        /// Tests order of operations.
        /// </summary>
        [Test]
        public void TestNotDefinedException()
        {
            CptS321.ExpressionTree test = new CptS321.ExpressionTree("1B");
            Assert.That(() => test.Evaluate(), Throws.TypeOf<System.Collections.Generic.KeyNotFoundException>());
        }

        /// <summary>
        /// Tests the undo/redo function for text.
        /// </summary>
        [Test]
        public void TestUndoText()
        {
            CptS321.SpreadSheet testsheet = new SpreadSheet(2, 2);
            CptS321.Cell testcell = testsheet.GetCell(0, 0);
            testcell.Text = "First";
            Assert.AreEqual("First", testcell.Text);
            testcell.Text = "second";
            testsheet.Undo();
            Assert.AreEqual("First", testcell.Text);
            testsheet.Redo();
            Assert.AreEqual("second", testcell.Text);
        }

        /// <summary>
        /// Tests Undo/Redo for the color property.
        /// </summary>
        [Test]
        public void TestUndoRedoColor()
        {
            CptS321.SpreadSheet testsheet = new SpreadSheet(2, 2);
            CptS321.Cell testcell = testsheet.GetCell(0, 0);
            testcell.BGCOLOR = 5U;
            Assert.AreEqual(5U, testcell.BGCOLOR);
            testsheet.SetTopUndoNumber(1);
            testcell.BGCOLOR = 50000U;
            Assert.AreEqual(50000U, testcell.BGCOLOR);
            testsheet.SetTopUndoNumber(1);
            testsheet.Undo();
            Assert.AreEqual(5U, testcell.BGCOLOR);
            testsheet.Redo();
            Assert.AreEqual(50000U, testcell.BGCOLOR);
        }

        /// <summary>
        /// Tests the saving and loading functionality.
        /// </summary>
        [Test]
        public void TestSaveLoad()
        {
            CptS321.SpreadSheet testsheet = new SpreadSheet(2, 2);
            CptS321.Cell testcell = testsheet.GetCell(0, 0);
            testcell.Text = "testing";
            FileStream savestream = File.Create("tt.xml");
            testsheet.SaveSheet(savestream);
            savestream.Dispose();
            testsheet.Clear();
            Assert.AreEqual(string.Empty, testcell.Text);
            FileStream loadstream = File.OpenRead("tt.xml");
            testsheet.LoadSheet(loadstream);
            loadstream.Dispose();
            Assert.AreEqual("testing", testcell.Text);
        }

        /// <summary>
        /// Tests one bad input to see if it catches.
        /// </summary>
        [Test]
        public void TestBadReferenceOne()
        {
            CptS321.SpreadSheet testsheet = new SpreadSheet(2, 2);
            CptS321.Cell testcell = testsheet.GetCell(0, 0);
            testcell.Text = "=A100";
            Assert.AreEqual("!(Bad Input)", testcell.Value);
        }

        /// <summary>
        /// Tests one circular input to see if it catches.
        /// </summary>
        [Test]
        public void TestCircularReferenceOne()
        {
            CptS321.SpreadSheet testsheet = new SpreadSheet(2, 2);
            CptS321.Cell testcell = testsheet.GetCell(0, 0);
            testcell.Text = "=A1";
            Assert.AreEqual("!(BadReference)", testcell.Value);
        }

        /// <summary>
        /// Tests for circular references in an equation.
        /// </summary>
        [Test]
        public void TestCircularReferenceEquation()
        {
            CptS321.SpreadSheet testsheet = new SpreadSheet(2, 2);
            CptS321.Cell testcell = testsheet.GetCell(0, 0);
            testcell.Text = "=A1+36";
            Assert.AreEqual("!(BadReference)", testcell.Value);
        }

        /// <summary>
        /// Tests equation input to see if it catches.
        /// </summary>
        [Test]
        public void TestBadReferenceEquation()
        {
            CptS321.SpreadSheet testsheet = new SpreadSheet(2, 2);
            CptS321.Cell testcell = testsheet.GetCell(0, 0);
            testcell.Text = "=20000-A100";
            Assert.AreEqual("!(Bad Input)", testcell.Value);
        }
    }
}