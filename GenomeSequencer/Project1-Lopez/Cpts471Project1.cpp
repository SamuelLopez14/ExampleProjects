// Cpts471Project1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
#include <iomanip>
#include <vector>

using namespace std;

typedef struct DP_cell {
    int Sscore;       // Substitution (S) score       
    int Dscore;      // Deletion (D) score
    int Iscore;        // Insertion (I) score 

}DPCell;

int SubstitutionScore(char c1, char c2, int match, int mismatch)
{
    if (c1 == c2)
    {
        //cout << c1 << "=" << c2 << "returning" << match << endl;
        return match;
    }
    else if (c1 != c2)
    {
        //cout << c1 << "!=" << c2 << "returning" << mismatch << endl;
        return mismatch;
    }
    else
    {
        cout << "Error in SubstitutionScore\n";
            return -1;
    }
}

int Maxof3(int a, int b, int c)
{
   // cout << "Comparing " << a << " " << b << " " << c << endl;
    if (a >= b && a >= c)
    {
        return a;
    }
    else if (b >= a && b >= c)
    {
        return b;
    }
    else if (c >= a && c >= b)
    {
        return c;
    }
    else
    {
        cout << "Error occured in Maxof3\n";
        return -1;
    }
}

void GlobalAlignment(string s1, string s2, int h, int match, int mismatch, int g,string s1name,string s2name,string filename)
{
    cout << "Entered Global Alignment Function\n";
    string finals1="", finals2="",matchstring="";
    char Case='l';
    int I, J,lastnum,matches=0,mismatches=0,gapstart=0,gap=0;
    vector<vector<DPCell>> cellBox(s1.length() + 1, vector<DPCell>(s2.length() + 1));
    for (int i = 0; i < s1.length() + 1; i++)
    {
        for (int j = 0; j < s2.length() + 1; j++)
        {
            if (i == 0 && j == 0)
            {
                cellBox[i][j].Dscore = 0;
                cellBox[i][j].Iscore = 0;
                cellBox[i][j].Sscore = 0;
            }
            else if (i == 0 && j != 0)
            {
                cellBox[i][j].Iscore = h + (g * j);
                cellBox[i][j].Dscore = -2000000000;
                cellBox[i][j].Sscore = -2000000000;
            }
            else if (j == 0 && i != 0)
            {
                cellBox[i][j].Dscore = h + (g * i);
                cellBox[i][j].Sscore = -2000000000;
                cellBox[i][j].Iscore = -2000000000;
            }
            else
            {
                cellBox[i][j].Sscore = Maxof3(cellBox[i - 1][j - 1].Dscore + SubstitutionScore(s1[i - 1], s2[j - 1], match, mismatch), cellBox[i - 1][j - 1].Iscore + SubstitutionScore(s1[i - 1], s2[j - 1], match, mismatch), cellBox[i - 1][j - 1].Sscore + SubstitutionScore(s1[i - 1], s2[j - 1], match, mismatch));
                cellBox[i][j].Dscore = Maxof3(cellBox[i - 1][j].Dscore + g, cellBox[i - 1][j].Sscore + g + h, cellBox[i - 1][j].Iscore + g + h);
                cellBox[i][j].Iscore = Maxof3(cellBox[i][j - 1].Iscore + g, cellBox[i][j - 1].Sscore + g + h, cellBox[i][j - 1].Dscore + g + h);
            }
            //cout << "Cell:" << i << "," << j << "D:" << cellBox[i][j].Dscore << " i:" << cellBox[i][j].Iscore << " S:" << cellBox[i][j].Sscore << endl;
            //system("pause");
        }
    }
    int score = Maxof3(cellBox[s1.length()][s2.length()].Dscore, cellBox[s1.length()][s2.length()].Iscore, cellBox[s1.length()][s2.length()].Sscore);
    cout << "Global Score: " << score << endl;
    I = s1.length();
    J = s2.length();

    if (cellBox[I][J].Dscore >= cellBox[I][J].Sscore && cellBox[I][J].Dscore >= cellBox[I][J].Iscore)
    {
        Case = 'd';
        lastnum = cellBox[I][J].Dscore;
        finals1 = finals1 + s1[I - 1];
        finals2 = finals2 + '-';
        I = I - 1;
    }
    else if (cellBox[I][J].Sscore >= cellBox[I][J].Iscore && cellBox[I][J].Sscore >= cellBox[I][J].Dscore)
    {
        Case = 's';
        lastnum = cellBox[I][J].Sscore;
        finals1 = finals1 + s1[I - 1];
        finals2 = finals2 + s2[J - 1];
        I--;
        J--;
    }
    else if (cellBox[I][J].Iscore >= cellBox[I][J].Sscore && cellBox[I][J].Iscore >= cellBox[I][J].Dscore)
    {
        Case = 'i';
        lastnum = cellBox[I][J].Iscore;
        finals1 = finals1 + '-';
        finals2 = finals2 + s2[J - 1];
        J--;
    }
    cout << "Case is " << Case << endl;

    while (I > 0 || J > 0)
    {
        //cout << "Case: " << Case << " lastnum" << lastnum << " s1fin" << finals1 << " s2fin" << finals2 << endl;
       // system("pause");
        if (Case == 'd')
        {
            if (cellBox[I][J].Dscore == lastnum - g)
            {
                Case = 'd';
                lastnum = cellBox[I][J].Dscore;
                finals1 = finals1 + s1[I - 1];
                finals2 = finals2 + '-';
                I = I - 1;
            }
            else if (cellBox[I][J].Sscore == lastnum - g - h)
            {
                Case = 's';
                lastnum = cellBox[I][J].Sscore;
                finals1 = finals1 + s1[I - 1];
                finals2 = finals2 + s2[J - 1];
                I--;
                J--;
            }
            else if (cellBox[I][J].Iscore == lastnum - g - h)
            {
                Case = 'i';
                lastnum = cellBox[I][J].Iscore;
                finals1 = finals1 + '-';
                finals2 = finals2 + s2[J - 1];
                J--;
            }
        }
        else if (Case == 's')
        {
            if (cellBox[I][J].Dscore == lastnum - SubstitutionScore(s1[I], s2[J], match, mismatch))
            {
                Case = 'd';
                lastnum = cellBox[I][J].Dscore;
                finals1 = finals1 + s1[I - 1];
                finals2 = finals2 + '-';
                I = I - 1;
            }
            else if (cellBox[I][J].Sscore == lastnum - SubstitutionScore(s1[I], s2[J], match, mismatch))
            {
                Case = 's';
                lastnum = cellBox[I][J].Sscore;
                finals1 = finals1 + s1[I - 1];
                finals2 = finals2 + s2[J - 1];
                I--;
                J--;
            }
            else if (cellBox[I][J].Iscore == lastnum - SubstitutionScore(s1[I], s2[J], match, mismatch))
            {
                Case = 'i';
                lastnum = cellBox[I][J].Iscore;
                finals1 = finals1 + '-';
                finals2 = finals2 + s2[J - 1];
                J--;
            }
        }
        else if (Case == 'i')
        {
            if (cellBox[I][J].Dscore == lastnum - g - h)
            {
                Case = 'd';
                lastnum = cellBox[I][J].Dscore;
                finals1 = finals1 + s1[I - 1];
                finals2 = finals2 + '-';
                I = I - 1;
            }
            else if (cellBox[I][J].Sscore == lastnum - g - h)
            {
                Case = 's';
                lastnum = cellBox[I][J].Sscore;
                finals1 = finals1 + s1[I - 1];
                finals2 = finals2 + s2[J - 1];
                I--;
                J--;
            }
            else if (cellBox[I][J].Iscore == lastnum - g)
            {
                Case = 'i';
                lastnum = cellBox[I][J].Iscore;
                finals1 = finals1 + '-';
                finals2 = finals2 + s2[J - 1];
                J--;
            }
        }
    }
    //cout<<"amde it to end\n";
    reverse(begin(finals1),end(finals1));
    
    reverse(begin(finals2), end(finals2));
    
    for (I = 0; I < finals1.length(); I++)
    {
        if (finals1[I] == finals2[I])
        {
            matchstring=matchstring+'|';
            matches++;
        }
        else
        {
            matchstring=matchstring+" ";
            if (finals1[I] != '-' && finals2[I] != '-')
            {
                mismatches++;
            }
            if (finals1[I] == '-')
            {
                if (I == 0 || finals1[I - 1] != '-')
                {
                    gapstart++;
                }
                gap++;
            }
            if (finals2[I] == '-')
            {
                if (I == 0 || finals2[I - 1] != '-')
                {
                    gapstart++;
                }
                gap++;
            }
        }
    }
    cout << finals1 << endl;
    cout << matchstring<<endl;
    cout << finals2 << endl;
    cout << "Matches: " << matches << " Mismatches: " << mismatches << " Gapstarts: " << gapstart << " Gap: " << gap << endl;
    fstream filestream;
    filestream.open(filename, std::ios::app);
    filestream << endl << "Output:\n*******\n\n" << "Scores:  match: " << match << ", mismatch: " << mismatch << " h: " << h << " g: " << g << endl;
    filestream << "Sequence 1:" << s1name << " Length = " << s1.length() << " characters\n";
    filestream << "Sequence 2:" << s2name << " Length = " << s2.length() << " characters\n";
    int s1len = 1, s2len = 1;
    I = 0;
    cout << finals1.length() << finals2.length() << matchstring.length();
    while (I < finals1.length())
    {
        filestream << "s1" << setw(10) << s1len<<" ";
        //cout << "I is " << I << endl;
        //cout << "wrote thte first line\n";
        J = I + 60;
        for (int i = I; i < finals1.length() && i<J; i++)
        {
            //cout << "i: " << i << "char: " << finals1[i];
            //cout<<"i+1: "<<i+1<<" mod 60: "<<(i+1)
            filestream << finals1[i];
            if (finals1[i] != '-')
            {
                s1len++;
            }
        }
        
        filestream << "\t" << s1len-1 << "\n";
        filestream << setw(13)<<" ";
        for (int i = I; i < matchstring.length() && i < J; i++)
        {
            filestream << matchstring[i];
        }
        filestream << "\n";
        filestream << "s2" << setw(10) << s2len<<" ";
        for (int i = I; i < finals2.length() && i<J; i++)
        {
            I++;
            filestream << finals2[i];
            if (finals2[i] != '-')
            {
                s2len++;
            }
        }

        filestream << "\t" << s2len-1 << "\n";
    }

    filestream << "\n\n\nReport:\n" << "Global Optimal Score = " << score << "\n" << "Number of: matches = " << matches << ", Mismatches = " << mismatches << ", Opening Starts = " << gapstart << ", gap extensions = " << gap << "\n";
    filestream << "Identities = " << matches << "/" << finals1.length() << "(" << ((double)matches / (double)finals1.length())*100 << ")%, Gaps = " << gap << "/" << finals1.length() << "(" << ((double)gap / (double)finals1.length())*100 << ")%\n";
   
}

int Maxof4(int a, int b, int c,int d)
{
    // cout << "Comparing " << a << " " << b << " " << c << endl;
    if (a >= b && a >= c && a>=d)
    {
        return a;
    }
    else if (b >= a && b >= c && b>=d)
    {
        return b;
    }
    else if (c >= a && c >= b && c>=d)
    {
        return c;
    }
    else if(d >= a && d >= b && d >= c)
    {
        return d;
    }
    else
    {
        cout << "Error occured in Maxof4\n";
        return -1;
    }
}

void LocalAlignment(string s1, string s2, int h, int match, int mismatch, int g, string s1name, string s2name, string filename)
{
    cout << "Entered Local Alignment Function\n";
    string finals1 = "", finals2 = "", matchstring = "";
    char Case = 'l';
    int I, J, lastnum, matches = 0, mismatches = 0, gapstart = 0, gap = 0,currentmax=0;
    vector<vector<DPCell>> cellBox(s1.length() + 1, vector<DPCell>(s2.length() + 1));
    for (int i = 0; i < s1.length() + 1; i++)
    {
        for (int j = 0; j < s2.length() + 1; j++)
        {
            if (i == 0 && j == 0)
            {
                cellBox[i][j].Dscore = 0;
                cellBox[i][j].Iscore = 0;
                cellBox[i][j].Sscore = 0;
            }
            else if (i == 0 && j != 0)
            {
                cellBox[i][j].Iscore = 0;
                cellBox[i][j].Dscore = 0;
                cellBox[i][j].Sscore = 0;
            }
            else if (j == 0 && i != 0)
            {
                cellBox[i][j].Dscore = 0;
                cellBox[i][j].Sscore = 0;
                cellBox[i][j].Iscore = 0;
            }
            else
            {
                cellBox[i][j].Sscore = Maxof4(0,cellBox[i - 1][j - 1].Dscore + SubstitutionScore(s1[i - 1], s2[j - 1], match, mismatch), cellBox[i - 1][j - 1].Iscore + SubstitutionScore(s1[i - 1], s2[j - 1], match, mismatch), cellBox[i - 1][j - 1].Sscore + SubstitutionScore(s1[i - 1], s2[j - 1], match, mismatch));
                cellBox[i][j].Dscore = Maxof4(0,cellBox[i - 1][j].Dscore + g, cellBox[i - 1][j].Sscore + g + h, cellBox[i - 1][j].Iscore + g + h);
                cellBox[i][j].Iscore = Maxof4(0,cellBox[i][j - 1].Iscore + g, cellBox[i][j - 1].Sscore + g + h, cellBox[i][j - 1].Dscore + g + h);
            }
            if (cellBox[i][j].Dscore >= currentmax || cellBox[i][j].Sscore >= currentmax || cellBox[i][j].Iscore >= currentmax)
            {
                currentmax = Maxof3(cellBox[i][j].Dscore, cellBox[i][j].Iscore, cellBox[i][j].Sscore);
                I = i;
                J = j;
                //cout << "I and J " << I << " " << J << " currentmax" << currentmax << endl;
            }
            //cout << "Cell:" << i << "," << j << "D:" << cellBox[i][j].Dscore << " i:" << cellBox[i][j].Iscore << " S:" << cellBox[i][j].Sscore << endl;
            //system("pause");
        }
    }
    int score = Maxof3(cellBox[I][J].Dscore, cellBox[I][J].Iscore, cellBox[I][J].Sscore);
    cout << "Local Score: " << score << endl;
    //I = s1.length();
    //J = s2.length();

    if (cellBox[I][J].Dscore >= cellBox[I][J].Sscore && cellBox[I][J].Dscore >= cellBox[I][J].Iscore)
    {
        Case = 'd';
        lastnum = cellBox[I][J].Dscore;
        finals1 = finals1 + s1[I - 1];
        finals2 = finals2 + '-';
        I = I - 1;
    }
    else if (cellBox[I][J].Sscore >= cellBox[I][J].Iscore && cellBox[I][J].Sscore >= cellBox[I][J].Dscore)
    {
        Case = 's';
        lastnum = cellBox[I][J].Sscore;
        finals1 = finals1 + s1[I - 1];
        finals2 = finals2 + s2[J - 1];
        I--;
        J--;
    }
    else if (cellBox[I][J].Iscore >= cellBox[I][J].Sscore && cellBox[I][J].Iscore >= cellBox[I][J].Dscore)
    {
        Case = 'i';
        lastnum = cellBox[I][J].Iscore;
        finals1 = finals1 + '-';
        finals2 = finals2 + s2[J - 1];
        J--;
    }
    cout << "Case is " << Case << endl;

    while (cellBox[I][J].Dscore>0|| cellBox[I][J].Sscore>0|| cellBox[I][J].Iscore>0)
    {
        //cout << "Case: " << Case << " lastnum" << lastnum << " s1fin" << finals1 << " s2fin" << finals2 << endl;
       // system("pause");
        if (Case == 'd')
        {
            if (cellBox[I][J].Dscore == lastnum - g)
            {
                Case = 'd';
                lastnum = cellBox[I][J].Dscore;
                finals1 = finals1 + s1[I - 1];
                finals2 = finals2 + '-';
                I = I - 1;
            }
            else if (cellBox[I][J].Sscore == lastnum - g - h)
            {
                Case = 's';
                lastnum = cellBox[I][J].Sscore;
                finals1 = finals1 + s1[I - 1];
                finals2 = finals2 + s2[J - 1];
                I--;
                J--;
            }
            else if (cellBox[I][J].Iscore == lastnum - g - h)
            {
                Case = 'i';
                lastnum = cellBox[I][J].Iscore;
                finals1 = finals1 + '-';
                finals2 = finals2 + s2[J - 1];
                J--;
            }
        }
        else if (Case == 's')
        {
            if (cellBox[I][J].Dscore == lastnum - SubstitutionScore(s1[I], s2[J], match, mismatch))
            {
                Case = 'd';
                lastnum = cellBox[I][J].Dscore;
                finals1 = finals1 + s1[I - 1];
                finals2 = finals2 + '-';
                I = I - 1;
            }
            else if (cellBox[I][J].Sscore == lastnum - SubstitutionScore(s1[I], s2[J], match, mismatch))
            {
                Case = 's';
                lastnum = cellBox[I][J].Sscore;
                finals1 = finals1 + s1[I - 1];
                finals2 = finals2 + s2[J - 1];
                I--;
                J--;
            }
            else if (cellBox[I][J].Iscore == lastnum - SubstitutionScore(s1[I], s2[J], match, mismatch))
            {
                Case = 'i';
                lastnum = cellBox[I][J].Iscore;
                finals1 = finals1 + '-';
                finals2 = finals2 + s2[J - 1];
                J--;
            }
        }
        else if (Case == 'i')
        {
            if (cellBox[I][J].Dscore == lastnum - g - h)
            {
                Case = 'd';
                lastnum = cellBox[I][J].Dscore;
                finals1 = finals1 + s1[I - 1];
                finals2 = finals2 + '-';
                I = I - 1;
            }
            else if (cellBox[I][J].Sscore == lastnum - g - h)
            {
                Case = 's';
                lastnum = cellBox[I][J].Sscore;
                finals1 = finals1 + s1[I - 1];
                finals2 = finals2 + s2[J - 1];
                I--;
                J--;
            }
            else if (cellBox[I][J].Iscore == lastnum - g)
            {
                Case = 'i';
                lastnum = cellBox[I][J].Iscore;
                finals1 = finals1 + '-';
                finals2 = finals2 + s2[J - 1];
                J--;
            }
        }
    }
    //cout<<"amde it to end\n";
    reverse(begin(finals1), end(finals1));

    reverse(begin(finals2), end(finals2));

    for (I = 0; I < finals1.length(); I++)
    {
        if (finals1[I] == finals2[I])
        {
            matchstring = matchstring + '|';
            matches++;
        }
        else
        {
            matchstring = matchstring + " ";
            if (finals1[I] != '-' && finals2[I] != '-')
            {
                mismatches++;
            }
            if (finals1[I] == '-')
            {
                if (I == 0 || finals1[I - 1] != '-')
                {
                    gapstart++;
                }
                gap++;
            }
            if (finals2[I] == '-')
            {
                if (I == 0 || finals2[I - 1] != '-')
                {
                    gapstart++;
                }
                gap++;
            }
        }
    }
    cout << finals1 << endl;
    cout << matchstring << endl;
    cout << finals2 << endl;
    cout << "Matches: " << matches << " Mismatches: " << mismatches << " Gapstarts: " << gapstart << " Gap: " << gap << endl;
    fstream filestream;
    filestream.open(filename, std::ios::app);
    filestream << endl << "Output:\n*******\n\n" << "Scores:  match: " << match << ", mismatch: " << mismatch << " h: " << h << " g: " << g << endl;
    filestream << "Sequence 1:" << s1name << " Length = " << s1.length() << " characters\n";
    filestream << "Sequence 2:" << s2name << " Length = " << s2.length() << " characters\n";
    int s1len = 1, s2len = 1;
    I = 0;
    cout << finals1.length() << finals2.length() << matchstring.length();
    while (I < finals1.length())
    {
        filestream << "s1" << setw(10) << s1len << " ";
        //cout << "I is " << I << endl;
        //cout << "wrote thte first line\n";
        J = I + 60;
        for (int i = I; i < finals1.length() && i < J; i++)
        {
            //cout << "i: " << i << "char: " << finals1[i];
            //cout<<"i+1: "<<i+1<<" mod 60: "<<(i+1)
            filestream << finals1[i];
            if (finals1[i] != '-')
            {
                s1len++;
            }
        }

        filestream << "\t" << s1len - 1 << "\n";
        filestream << setw(13) << " ";
        for (int i = I; i < matchstring.length() && i < J; i++)
        {
            filestream << matchstring[i];
        }
        filestream << "\n";
        filestream << "s2" << setw(10) << s2len << " ";
        for (int i = I; i < finals2.length() && i < J; i++)
        {
            I++;
            filestream << finals2[i];
            if (finals2[i] != '-')
            {
                s2len++;
            }
        }

        filestream << "\t" << s2len - 1 << "\n";
    }

    filestream << "\n\n\nReport:\n" << "Local Optimal Score = " << score << "\n" << "Number of: matches = " << matches << ", Mismatches = " << mismatches << ", Opening Starts = " << gapstart << ", gap extensions = " << gap << "\n";
    filestream << "Identities = " << matches << "/" << finals1.length() << "(" << ((double)matches / (double)finals1.length()) * 100 << ")%, Gaps = " << gap << "/" << finals1.length() << "(" << ((double)gap / (double)finals1.length()) * 100 << ")%\n";

}

int main(int argc, char* argv[])
{
    ofstream outstream;

    string parameterPath,tempString,s1name,s2name,s1,s2,filename;
    int match, mismatch, h, g;
    std::cout << "Hello World! number of arguments: "<<argc<<argv[0]<<endl;
    
    if (argc == 3)
    {
        cout << "Using Default Parameters\n";
        parameterPath = "./parameters.config";
    }
    else if(argc==4)
    {
        parameterPath = argv[3];
        cout << "Using Directory " << parameterPath << endl;
        
    }
    else if(argc < 3||argc > 4)
    {
        cout << "Improper number of arguments given\n";
    }

    cout << "Fetching input parameters\n";
    ifstream filestream;
    filestream.open(parameterPath);
    if (!filestream.is_open())
    {
        cout << "Could not open file: " << parameterPath << endl;
        return -1;
    }
    getline(filestream,tempString);
    sscanf_s(tempString.c_str(), "%*s %d", &match);
    cout << "match: " << match << endl;

    getline(filestream, tempString);
    sscanf_s(tempString.c_str(), "%*s %d", &mismatch);
    cout << "mismatch: " << mismatch << endl;

    getline(filestream, tempString);
    sscanf_s(tempString.c_str(), "%*s %d", &h);
    cout << "h: " << h << endl;
    getline(filestream, tempString);
    sscanf_s(tempString.c_str(), "%*s %d", &g);
    cout << "g: " << g << endl;

    cout << -2147483648 + h + g << endl;

    cout << "Now getting input strings\n";
    parameterPath = argv[1];
    cout << "Looking for file: " << parameterPath << endl;
    filestream.close();
    filestream.open(parameterPath);
    if (!filestream.is_open())
    {
        cout << "Could not open: " << parameterPath << endl;
        return -1;
    }
    getline(filestream, tempString);
    s1name = tempString.substr(1);
    cout << "Name of First String: " << s1name << endl;

    while (getline(filestream, tempString) && tempString[0] != '>')
    {
        s1 = s1 + tempString;
    }

    cout << "s1: " << s1 << endl;

    s2name = tempString.substr(1);
    cout << "Name of Second String: " << s2name << endl;

    while (getline(filestream, tempString))
    {
        s2 = s2 + tempString;
    }

    cout << "s2: " << s2 << endl;
    filestream.close();

    cout << "length of s1: " << s1.length() << endl;
    cout << "length of s2: " << s2.length() << endl;
    filename = "output.txt";
    outstream.open(filename);
    outstream << "INPUT:\n*******\n\n>" << s1name << endl << s1 << "\n\n"<<">"<<s2name<<endl<<s2<<endl;
    outstream.close();

    if (string(argv[2]) == "0")
    {
        cout << "You have selected global alignment\n";

        GlobalAlignment(s1, s2, h, match, mismatch, g,s1name,s2name,filename);
    }
    else if (string(argv[2]) == "1")
    {
        cout << "You have selected Local alignment\n";
        LocalAlignment(s1, s2, h, match, mismatch, g, s1name, s2name, filename);
    }
    else
    {
        cout << "Invalid Option Chosen: " << argv[2] << endl;
        return -1;
    }

    
}


