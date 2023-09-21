#pragma once
#include <iostream>
#include <chrono>
#include <stdio.h>
#include <string.h>
#include <map>
#include "Node.h"
#include <list>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;
class SuffixTree
{
public:
	Node* root, *deepestInternalNode;
	map<char, int> CharMap;
	string baseString;
    list<Node*> EndNodes;
	int AlphabetSize;
    int internalNodes;
    int leafNodes;
    int currentLeafIndex;
    int currentInternalNodeIndex;
    int sumInternalNodeDepths;
    double Time;

	

	SuffixTree(string fasta, string alphabet)
    {
        Node* tempnode;
		cout << "Getting Alphabet From Text File\n";
        this->AlphabetSize = this->AnalyzeAlphabet(alphabet);
        cout << "Size of alphabet: " << this->AlphabetSize << endl;

        for (auto element : this->CharMap)
        {

            //element.first represents key
            cout << "Char:" << element.first;

            //element.second represents value
            cout<<" Val: " << element.second << endl;
        }

        cout << "Now Getting Base String\n";
        this->baseString = "";
        this->setBaseString(fasta);
        cout << "The base string is: " << this->baseString << endl;
        this->internalNodes = 1;
        this->leafNodes = 0;
        this->currentLeafIndex = 1;
        this->currentInternalNodeIndex = this->baseString.length() + 1;
        auto start = chrono::high_resolution_clock::now();
        this->BuildingSuffix();
        auto end = chrono::high_resolution_clock::now();
        auto total_time = chrono::duration_cast<chrono::duration<double>>(end - start);
        this->Time = total_time.count();
        /*this->root = new Node(this->AlphabetSize);
        this->root->parent = root;
        this->root->suffixLink = root;
        root->ID = 0;
        root->stringDepth = 0;
        for (int i = 0; i < this->baseString.length(); i++)
        {
            tempnode = this->FindPath(this->root, i);
            cout << "Added node: " << this->GetStringFromNode(tempnode) << endl;
            cout << "Id: " << tempnode->ID << " Depth: " << tempnode->stringDepth << endl;
        }*/

        cout << "End\n";
        

	}

	int AnalyzeAlphabet(string alphabet)
	{
        std::ifstream infile;
        std::string line;
        infile.open(alphabet);
        if (infile.is_open())
        {
            this->CharMap.insert(std::pair<char, int>('$', 0));
            std::getline(infile, line);
            for (char character : line)
            {
                if (character != ' ')
                {
                    
                    this->CharMap.insert(std::pair<char, int>(character, this->CharMap.size()));
                }
            }
            infile.close();
            return this->CharMap.size();
        }
        else
        {
            std::cout << "error in alphabet files\n";
            exit(1);
        }
	}

    void setBaseString(string pathname);
    Node* FindPath(Node* StartingNode, int startingIndex);
    string GetStringFromNode(Node* n);
    Node* NodeHops(string beta, Node* startingNode);
    void BuildingSuffix();
    void DFSHelper(Node* n);
    void DepthFirstSearch();
    void BWTIndex();
    void BWTHelper(Node* n);
    void DisplayInformation();
    void DisplayConstructionTime();
    void DisplayNodesChildren(Node *n);
    void GetLeafsOfNode(Node* s, list<int> &l);
    void PrintrandomNodes(Node* n);
     
};

