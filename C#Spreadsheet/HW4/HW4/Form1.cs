// <copyright file="Form1.cs" company="Samuel Lopez 11417858">
// Copyright (c) Samuel Lopez 11417858. All rights reserved.
// </copyright>

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace HW4
{
    /// <summary>
    /// This is the default form class of the application.
    /// </summary>
    public partial class Form1 : Form
    {
        /// <summary>
        /// The Spreadsheet for the class.
        /// </summary>
        private CptS321.SpreadSheet formSheet = new CptS321.SpreadSheet(50, 26);

        /// <summary>
        /// Initializes a new instance of the <see cref="Form1"/> class.
        /// The default constructor for the form.
        /// </summary>
        public Form1()
        {
            this.InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            string columnAndRowName = string.Empty;
            char columnNameHelper = 'A';
            for (int i = 0; i < 26; i++)
            {
                columnNameHelper = (char)('A' + i);
                this.dataGridView1.Columns.Add(columnNameHelper.ToString(), columnNameHelper.ToString());
            }

            for (int i = 0; i < 50; i++)
            {
                int rowId = this.dataGridView1.Rows.Add();
                this.dataGridView1.Rows[i].HeaderCell.Value = (i + 1).ToString();
            }

            this.formSheet.PropertyChanged += this.UpdateSheet;
            this.undoToolStripMenuItem.Enabled = false;
            this.redoToolStripMenuItem.Enabled = false;

            // CptS321.Cell cell = this.formSheet.GetCell(0, 1);
            // cell.Text = "teststring";
            // cell = this.formSheet.GetCell(0, 0);
            // cell.Text = "=B1";
        }

        private void UpdateSheet(object sender, PropertyChangedEventArgs e)
        {
            if (e.PropertyName == "color")
            {
                this.dataGridView1[(sender as CptS321.Cell).ColumnIndex, (sender as CptS321.Cell).RowIndex].Style.BackColor = System.Drawing.Color.FromArgb((int)(sender as CptS321.Cell).BGCOLOR);

                if (!this.formSheet.IsUndosEmpty())
                {
                    this.undoToolStripMenuItem.Enabled = true;
                    this.undoToolStripMenuItem.Text = "Undo Recent " + this.formSheet.UndoType() + " changes";
                }
                else
                {
                    this.undoToolStripMenuItem.Text = "Undo";
                    this.undoToolStripMenuItem.Enabled = false;
                }

                if (!this.formSheet.IsRedoEmpty())
                {
                    this.redoToolStripMenuItem.Enabled = true;
                    this.redoToolStripMenuItem.Text = "Redo Recent " + this.formSheet.RedoType() + " changes";
                }
                else
                {
                    this.redoToolStripMenuItem.Text = "Redo";
                    this.redoToolStripMenuItem.Enabled = false;
                }

                return;
            }

            this.dataGridView1[(sender as CptS321.Cell).ColumnIndex, (sender as CptS321.Cell).RowIndex].Value = (sender as CptS321.Cell).Value;
            if (!this.formSheet.IsUndosEmpty())
            {
                this.undoToolStripMenuItem.Enabled = true;
                this.undoToolStripMenuItem.Text = "Undo Recent " + this.formSheet.UndoType() + " changes";
            }
            else
            {
                this.undoToolStripMenuItem.Text = "Undo";
                this.undoToolStripMenuItem.Enabled = false;
            }

            if (!this.formSheet.IsRedoEmpty())
            {
                this.redoToolStripMenuItem.Enabled = true;
                this.redoToolStripMenuItem.Text = "Redo Recent " + this.formSheet.RedoType() + " changes";
            }
            else
            {
                this.redoToolStripMenuItem.Text = "Redo";
                this.redoToolStripMenuItem.Enabled = false;
            }
        }

        /// <summary>
        /// Activates the Spreadsheet demo.
        /// </summary>
        /// <param name="sender">Object Sender.</param>
        /// <param name="e">Argument Strings.</param>
        private void DemoToolStripMenuItem_Click(object sender, EventArgs e)
        {
            int row, column;
            Random randomInt = new Random();
            CptS321.Cell currentCell;
            for (int i = 0; i < 50; i++)
            {
                row = randomInt.Next(0, 50);
                column = randomInt.Next(0, 26);
                currentCell = this.formSheet.GetCell(row, column);
                currentCell.Text = "Hello World";
            }

            for (int i = 0; i < 50; i++)
            {
                currentCell = this.formSheet.GetCell(i, 1);
                currentCell.Text = "This is Cell B" + (i + 1).ToString();
            }

            for (int i = 0; i < 50; i++)
            {
                currentCell = this.formSheet.GetCell(i, 0);
                currentCell.Text = "=B" + (i + 1).ToString();
            }
        }

        private void DataGridView1_CellBeginEdit(object sender, DataGridViewCellCancelEventArgs e)
        {
            int row = this.dataGridView1.CurrentCell.RowIndex;
            int column = this.dataGridView1.CurrentCell.ColumnIndex;
            CptS321.Cell cell = this.formSheet.GetCell(row, column);
            this.dataGridView1.CurrentCell.Value = cell.Text;
        }

        private void DataGridView1_CellEndEdit(object sender, DataGridViewCellEventArgs e)
        {
            int row = this.dataGridView1.CurrentCell.RowIndex;
            int column = this.dataGridView1.CurrentCell.ColumnIndex;
            CptS321.Cell cell = this.formSheet.GetCell(row, column);
            cell.Text = (string)this.dataGridView1.CurrentCell.Value;
            this.dataGridView1.CurrentCell.Value = cell.Value;
        }

        private void ChangeColorToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ColorDialog colorDialog = new ColorDialog();
            DialogResult color = colorDialog.ShowDialog();
            int index = 0;
            if (color == DialogResult.OK)
            {
                foreach (DataGridViewCell datacell in this.dataGridView1.SelectedCells)
                {
                    CptS321.Cell cell = this.formSheet.GetCell(datacell.RowIndex, datacell.ColumnIndex);
                    cell.BGCOLOR = (uint)colorDialog.Color.ToArgb();
                    index++;
                }

                this.formSheet.SetTopUndoNumber(index);
            }

            return;
        }

        private void UndoToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.formSheet.Undo();
        }

        private void RedoToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.formSheet.Redo();
        }

        private void SaveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SaveFileDialog saveFile = new SaveFileDialog();
            saveFile.Filter = "xml files (*.xml)|*.xml";
            if (saveFile.ShowDialog() == DialogResult.OK)
            {
                FileStream fs = File.Create(saveFile.FileName);
                this.formSheet.SaveSheet(fs);
                fs.Dispose();
            }
        }

        private void LoadToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenFileDialog openFile = new OpenFileDialog();
            openFile.Filter = "xml files (*.xml)|*.xml";
            if (openFile.ShowDialog() == DialogResult.OK)
            {
                FileStream fs = File.OpenRead(openFile.FileName);
                this.formSheet.LoadSheet(fs);
                fs.Dispose();
            }

            this.undoToolStripMenuItem.Text = "Undo";
            this.undoToolStripMenuItem.Enabled = false;
            this.redoToolStripMenuItem.Text = "Redo";
            this.redoToolStripMenuItem.Enabled = false;
        }
    }
}
