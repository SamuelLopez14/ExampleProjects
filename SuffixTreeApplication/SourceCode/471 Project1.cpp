// 471 Project1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <stdio.h>
#include <string.h>
#include "Node.h"
#include "SuffixTree.h"
using namespace std;


int main(int argc, char* argv[])
{
    std::cout << "Starting Suffix Tree Construction\n";
    if (argc < 3)
    {
        cout << "Not Enough Arguments\n";
        return -1;
    }

    cout << "Fasta: " << argv[1] << "\nAlphabet: " << argv[2] << endl;
    SuffixTree sTree(argv[1], argv[2]);
    
    //sTree.DepthFirstSearch();
    sTree.BWTIndex();
    sTree.DisplayInformation();
    sTree.DisplayConstructionTime();

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
