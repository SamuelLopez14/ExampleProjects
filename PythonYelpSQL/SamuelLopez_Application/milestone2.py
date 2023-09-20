from inspect import stack
from select import select
import string
import sys
from traceback import StackSummary
from unicodedata import category
from PyQt5.QtWidgets import QMainWindow, QApplication, QWidget, QAction, QTableWidget,QTableWidgetItem,QVBoxLayout
from PyQt5 import uic, QtCore
from PyQt5.QtGui import QIcon, QPixmap
import psycopg2

qtCreatorFile = "milestone1app.ui" # Enter file here.

Ui_MainWindow, QtBaseClass = uic.loadUiType(qtCreatorFile)

class milestone1(QMainWindow):
    def __init__(self):
        super(milestone1, self).__init__()
        self.ui = Ui_MainWindow()
        self.ui.setupUi(self)
        self.loadStateList()
        self.ui.stateBox.currentTextChanged.connect(self.stateChanged)
        self.ui.cityList.itemSelectionChanged.connect(self.cityChanged)
        self.ui.ZipList.itemSelectionChanged.connect(self.zipChanged)
        self.ui.FilterButton.clicked.connect(self.ApplyFilter)
        self.ui.RemoveFilterButton.clicked.connect(self.RemoveFilter)
        self.ui.PopButton.clicked.connect(self.PopSuccessfulBusiness)




    def executeQuery(self,sql_str):
        try:
            conn = psycopg2.connect("dbname='yelpdb_temp' user='postgres' host='localhost' password='12345'")
        except:
            print("Unable to connect to database")
        cur = conn.cursor()
        try:
            cur.execute(sql_str)
        except:
            print("Unable to execute query "+ sql_str)
        conn.commit()
        result = cur.fetchall()
        conn.close()
        return result

    def loadStateList(self):
        self.ui.stateBox.clear()
        sql_str = "SELECT distinct state FROM business ORDER BY state;"
        try:
            results = self.executeQuery(sql_str)
            for row in results:
                self.ui.stateBox.addItem(row[0])
        except:
            print("query failed")
        self.ui.stateBox.setCurrentIndex(-1)
        self.ui.stateBox.clearEditText()
    
    def stateChanged(self):
        self.ui.cityList.clear()
        state = self.ui.stateBox.currentText()
        if self.ui.stateBox.currentIndex()>=0:
            sql_str = "SELECT distinct city FROM business WHERE state = '"+state+"' ORDER BY city;"
            print(sql_str)
            try:
                results = self.executeQuery(sql_str)
                ##print(results)
                for row in results:
                    self.ui.cityList.addItem(row[0])
            except:
                print("StateChange failed")
            # try:
            #     sql_str = "SELECT name,city,state,zip FROM business WHERE state = '"+state+"' ORDER BY name;"
            #     results = self.executeQuery(sql_str)

            #     self.ui.businessTable.setColumnCount(len(results[0]))
            #     self.ui.businessTable.setRowCount(len(results))
            #     style = "::section {""background-color: #f3f3f3;}"
            #     self.ui.businessTable.horizontalHeader().setStyleSheet(style)
            #     self.ui.businessTable.setHorizontalHeaderLabels(["Business name","City","State","Zip"])
            #     self.ui.businessTable.setColumnWidth(0,300)
            #     self.ui.businessTable.setColumnWidth(1,100)
            #     self.ui.businessTable.setColumnWidth(2,50)


            #     currentRow = 0
            #     for row in results:
            #         for colCount in range(0,len(results[0])):
            #             self.ui.businessTable.setItem(currentRow,colCount,QTableWidgetItem(row[colCount]))
            #         currentRow+=1
            # except:
            #     print("Statechange failed")

            # sql_str = "SELECT distinct category FROM business,businesscategory WHERE business.BusinessID = businesscategory.BusinessID AND business.state = '" + state +"' ORDER BY category"
            # try:
            #     results = self.executeQuery(sql_str)
            #     self.ui.CategoryList.clear()
            #     ##print(results)
            #     for row in results:
            #         self.ui.CategoryList.addItem(row[0])
            # except:
            #     print("StateChange failed")




    def cityChanged(self):
        if self.ui.stateBox.currentIndex()<0 or len(self.ui.cityList.selectedItems()) <=0:
            return
        state = self.ui.stateBox.currentText()
        city = self.ui.cityList.selectedItems()[0].text()
        sql_str = "SELECT distinct zip FROM business WHERE state = '"+state+"' AND city = '"+city+"' ORDER BY zip;"
        try:
                results = self.executeQuery(sql_str)
                self.ui.ZipList.clear()
                ##print(results)
                for row in results:
                    self.ui.ZipList.addItem(row[0])
        except:
                print("StateChange failed")

        # sql_str = "SELECT name,city,state,zip FROM business WHERE state = '"+state+"' AND city = '"+city+"' ORDER BY name;"
        # results = self.executeQuery(sql_str)
        # try:

        #         results = self.executeQuery(sql_str)
        #         style = "::section {""background-color: #f3f3f3;}"
        #         self.ui.businessTable.horizontalHeader().setStyleSheet(style)
        #         self.ui.businessTable.setColumnCount(len(results[0]))
        #         self.ui.businessTable.setRowCount(len(results))
        #         self.ui.businessTable.setHorizontalHeaderLabels(["Business name","City","State","Zip"])
        #         self.ui.businessTable.setColumnWidth(0,300)
        #         self.ui.businessTable.setColumnWidth(1,100)
        #         self.ui.businessTable.setColumnWidth(2,50)


        #         currentRow = 0
        #         for row in results:
        #             for colCount in range(0,len(results[0])):
        #                 self.ui.businessTable.setItem(currentRow,colCount,QTableWidgetItem(row[colCount]))
        #             currentRow+=1
        # except:
        #         print("Statechange failed")

        # sql_str = "SELECT distinct category FROM business,businesscategory WHERE business.BusinessID = businesscategory.BusinessID AND business.state = '" + state +"'AND business.city = '"+ city +"' ORDER BY category"
        # try:
        #         results = self.executeQuery(sql_str)
        #         self.ui.CategoryList.clear()
        #         ##print(results)
        #         for row in results:
        #             self.ui.CategoryList.addItem(row[0])
        # except:
        #         print("StateChange failed")

    def zipChanged(self):
        if self.ui.stateBox.currentIndex()<0 or len(self.ui.cityList.selectedItems()) <=0 or len(self.ui.ZipList.selectedItems())<=0:
            return
        state = self.ui.stateBox.currentText()
        city = self.ui.cityList.selectedItems()[0].text()
        zip = self.ui.ZipList.selectedItems()[0].text()
        sql_str = "SELECT name,city,state,zip FROM business WHERE state = '"+state+"' AND city = '"+city+ "' AND zip ='"+zip+"' ORDER BY name;"
        results = self.executeQuery(sql_str)
        try:

                results = self.executeQuery(sql_str)
                style = "::section {""background-color: #f3f3f3;}"
                self.ui.businessTable.horizontalHeader().setStyleSheet(style)
                self.ui.businessTable.setColumnCount(len(results[0]))
                self.ui.businessTable.setRowCount(len(results))
                self.ui.businessTable.setHorizontalHeaderLabels(["Business name","City","State","Zip"])
                self.ui.businessTable.setColumnWidth(0,300)
                self.ui.businessTable.setColumnWidth(1,100)
                self.ui.businessTable.setColumnWidth(2,50)


                currentRow = 0
                for row in results:
                    for colCount in range(0,len(results[0])):
                        self.ui.businessTable.setItem(currentRow,colCount,QTableWidgetItem(row[colCount]))
                    currentRow+=1
        except:
                print("Statechange failed")

        sql_str = "SELECT distinct category FROM business,businesscategory WHERE business.BusinessID = businesscategory.BusinessID AND business.state = '" + state +"'AND business.city = '"+ city +"'AND zip = '"+zip+"' ORDER BY category"
        try:
                results = self.executeQuery(sql_str)
                self.ui.CategoryList.clear()
                ##print(results)
                for row in results:
                    self.ui.CategoryList.addItem(row[0])
        except:
                print("StateChange failed")

        try:
                sql_str = "SELECT count(*) FROM business WHERE business.zip='"+zip+"';"
                results = self.executeQuery(sql_str)
                self.ui.NumBusinessText.setPlainText(str(results[0][0]))
                sql_str = "SELECT * FROM zipcodedata WHERE zipcode ='"+zip+"';"
                results = self.executeQuery(sql_str)
                self.ui.PopulationText.setPlainText(str(results[0][3]))
                self.ui.IncomeTextEdit.setPlainText(str(results[0][2]))
                print(results)
        except:
                print("ZipDetails failed")



        try:
            sql_str = "SELECT count(business.businessid),businesscategory.category FROM business,businesscategory WHERE business.businessid = businesscategory.businessid AND business.zip = '"+zip+"' GROUP BY businesscategory.category"
            results = self.executeQuery(sql_str)
            ##print(results)
            style = "::section {""background-color: #f3f3f3;}"
            self.ui.CategoryTable.horizontalHeader().setStyleSheet(style)
            self.ui.CategoryTable.setColumnCount(len(results[0]))
            self.ui.CategoryTable.setRowCount(len(results))
            self.ui.CategoryTable.setHorizontalHeaderLabels(["#","Category"])
            self.ui.CategoryTable.setColumnWidth(0,50)
            self.ui.CategoryTable.setColumnWidth(1,100)



            currentRow = 0
            for row in results:
                 for colCount in range(0,len(results[0])):
                        self.ui.CategoryTable.setItem(currentRow,colCount,QTableWidgetItem(str(row[colCount])))
                 currentRow+=1
        except:
            print("CategoryTable UPdate failed")
            print(results)

    def ApplyFilter(self):
        if self.ui.stateBox.currentIndex()<0 or len(self.ui.cityList.selectedItems()) <=0 or len(self.ui.ZipList.selectedItems())<=0 or len(self.ui.CategoryList.selectedItems())<=0:
            return
        state = self.ui.stateBox.currentText()
        city = self.ui.cityList.selectedItems()[0].text()
        zip = self.ui.ZipList.selectedItems()[0].text()
        category = self.ui.CategoryList.selectedItems()[0].text()

        sql_str = "SELECT name,city,state,zip FROM business,businesscategory WHERE business.businessID = businesscategory.businessID AND state = '"+state+"' AND city = '"+city+ "' AND zip ='"+zip+"'AND category ='"+category+"' ORDER BY name;"
        results = self.executeQuery(sql_str)
        try:

                results = self.executeQuery(sql_str)
                style = "::section {""background-color: #f3f3f3;}"
                self.ui.businessTable.horizontalHeader().setStyleSheet(style)
                self.ui.businessTable.setColumnCount(len(results[0]))
                self.ui.businessTable.setRowCount(len(results))
                self.ui.businessTable.setHorizontalHeaderLabels(["Business name","City","State","Zip"])
                self.ui.businessTable.setColumnWidth(0,300)
                self.ui.businessTable.setColumnWidth(1,100)
                self.ui.businessTable.setColumnWidth(2,50)


                currentRow = 0
                for row in results:
                    for colCount in range(0,len(results[0])):
                        self.ui.businessTable.setItem(currentRow,colCount,QTableWidgetItem(row[colCount]))
                    currentRow+=1
        except:
                print("Filter failed")

    def RemoveFilter(self):
        if self.ui.stateBox.currentIndex()<0 or len(self.ui.cityList.selectedItems()) <=0 or len(self.ui.ZipList.selectedItems())<=0:
            return

        state = self.ui.stateBox.currentText()
        city = self.ui.cityList.selectedItems()[0].text()
        zip = self.ui.ZipList.selectedItems()[0].text()
        sql_str = "SELECT name,city,state,zip FROM business WHERE state = '"+state+"' AND city = '"+city+ "' AND zip ='"+zip+"' ORDER BY name;"
        results = self.executeQuery(sql_str)
        try:

                results = self.executeQuery(sql_str)
                style = "::section {""background-color: #f3f3f3;}"
                self.ui.businessTable.horizontalHeader().setStyleSheet(style)
                self.ui.businessTable.setColumnCount(len(results[0]))
                self.ui.businessTable.setRowCount(len(results))
                self.ui.businessTable.setHorizontalHeaderLabels(["Business name","City","State","Zip"])
                self.ui.businessTable.setColumnWidth(0,300)
                self.ui.businessTable.setColumnWidth(1,100)
                self.ui.businessTable.setColumnWidth(2,50)


                currentRow = 0
                for row in results:
                    for colCount in range(0,len(results[0])):
                        self.ui.businessTable.setItem(currentRow,colCount,QTableWidgetItem(row[colCount]))
                    currentRow+=1
        except:
                print("remove Filter failed")


    def PopSuccessfulBusiness(self):
        if len(self.ui.ZipList.selectedItems())<=0:
            return
        zip = self.ui.ZipList.selectedItems()[0].text()
        try:
            sql_str = "SELECT name,reviewrating,revew_count FROM business, (SELECT zip as zipcode, avg(reviewrating) as avg FROM business GROUP BY zip) as temp WHERE business.zip = temp.zipcode AND business.zip = '"+zip+"' ORDER BY (reviewrating-avg) desc"
            results = self.executeQuery(sql_str)
            style = "::section {""background-color: #f3f3f3;}"
            self.ui.PopularTable.horizontalHeader().setStyleSheet(style)
            self.ui.PopularTable.setColumnCount(len(results[0]))
            self.ui.PopularTable.setRowCount(len(results))
            self.ui.PopularTable.setHorizontalHeaderLabels(["Business name","Rating","Reviews"])
            self.ui.PopularTable.setColumnWidth(0,300)
            self.ui.PopularTable.setColumnWidth(1,150)

            currentRow = 0
            for row in results:
                for colCount in range(0,len(results[0])):
                        if type(row[colCount])==float:
                            popstr = f'{row[colCount]:.2f}'
                        else:
                            popstr = str(row[colCount])
                        self.ui.PopularTable.setItem(currentRow,colCount,QTableWidgetItem(popstr))
                currentRow+=1
            
        
        except Exception as e:
            print("Popular Update Failed")
            print(e)

        
        try:
            sql_str = "SELECT name,numcheckins FROM business,zipcodedata WHERE business.zip = zipcodedata.zipcode AND business.zip = '"+zip+"' ORDER BY (business.numcheckins*zipcodedata.medianincome) desc"
            results = self.executeQuery(sql_str)
            style = "::section {""background-color: #f3f3f3;}"
            self.ui.SuccessfulBUsinesses.horizontalHeader().setStyleSheet(style)
            self.ui.SuccessfulBUsinesses.setColumnCount(len(results[0]))
            self.ui.SuccessfulBUsinesses.setRowCount(len(results))
            self.ui.SuccessfulBUsinesses.setHorizontalHeaderLabels(["Business name","Checkins"])
            self.ui.SuccessfulBUsinesses.setColumnWidth(0,300)
            self.ui.SuccessfulBUsinesses.setColumnWidth(1,150)

            currentRow = 0
            for row in results:
                for colCount in range(0,len(results[0])):
                        if type(row[colCount])==float:
                            popstr = f'{row[colCount]:.2f}'
                        else:
                            popstr = str(row[colCount])
                        self.ui.SuccessfulBUsinesses.setItem(currentRow,colCount,QTableWidgetItem(popstr))
                currentRow+=1
        except Exception as e:
            print("Success update failed")
            print(e)

         


if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = milestone1()
    window.show()
    sys.exit(app.exec_())