#pragma once
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <vector>
using namespace std;
class Node
{
public:
	Node* parent;
	int edgelabel[2];
	Node* suffixLink;
	int ID;
	vector<Node*> Chilren;
	int stringDepth;

	Node(int alphabetsize)
	{
		this->Chilren.resize(alphabetsize);
		this->ID = 0;
		this->parent = NULL;
		this->suffixLink = NULL;
		this->edgelabel[0] = 0;
		this->edgelabel[1] = 0;
	}

	void SetID(int newID);
};

