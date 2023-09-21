#include "SuffixTree.h"

void SuffixTree::setBaseString(string pathname)
{
	std::ifstream infile;
	string tempstring;
	infile.open(pathname);
	if (infile.is_open())
	{
		getline(infile, tempstring);
		cout << "String name: " << tempstring.substr(1) << endl;
		while (getline(infile, tempstring))
		{
			this->baseString += tempstring;
		}
		this->baseString += "$";
	}
	else
	{
		cout << "pathname: " << pathname << "Could not be opened\n";
		exit(-1);
	}
}

Node* SuffixTree::FindPath(Node* StartingNode, int startingIndex)
{
	bool NotFound = true;
	Node* tempnode = NULL,*tempnode2 = NULL;
	while (NotFound)
	{
		tempnode = StartingNode->Chilren[this->CharMap[this->baseString[startingIndex]]];
		if (tempnode == NULL)
		{
			tempnode = new Node(this->AlphabetSize);
			tempnode->edgelabel[0] = startingIndex;
			tempnode->edgelabel[1] = this->baseString.length() - 1;
			StartingNode->Chilren[this->CharMap[this->baseString[startingIndex]]] = tempnode;
			tempnode->parent = StartingNode;
			tempnode->ID = this->currentLeafIndex;
			this->currentLeafIndex++;
			tempnode->stringDepth = tempnode->parent->stringDepth + (tempnode->edgelabel[1] - tempnode->edgelabel[0] + 1);
			//cout << "Added suffix: " << this->GetStringFromNode(tempnode) << endl;
			NotFound = false;
			this->leafNodes++;
			break;
		}
		else
		{
			for (int i = tempnode->edgelabel[0]; i <= tempnode->edgelabel[1]; i++)
			{
				if (this->baseString[startingIndex] != this->baseString[i])
				{
					tempnode2 = new Node(this->AlphabetSize);
					tempnode2->edgelabel[0] = tempnode->edgelabel[0];
					tempnode2->edgelabel[1] = i - 1;
					tempnode2->parent = StartingNode;
					tempnode2->Chilren[this->CharMap[this->baseString[i]]] = tempnode;
					StartingNode->Chilren[this->CharMap[this->baseString[tempnode2->edgelabel[0]]]] = tempnode2;
					tempnode->parent = tempnode2;
					tempnode->edgelabel[0] = i; 
					tempnode = new Node(this->AlphabetSize);
					tempnode->parent = tempnode2;
					tempnode->edgelabel[0] = startingIndex;
					tempnode->edgelabel[1] = this->baseString.length() - 1;
					tempnode2->Chilren[this->CharMap[this->baseString[startingIndex]]] = tempnode;
					//cout << "Added suffix: " << this->GetStringFromNode(tempnode) << endl;
					tempnode2->ID = this->currentInternalNodeIndex;
					this->currentInternalNodeIndex++;
					tempnode->ID = this->currentLeafIndex;
					this->currentLeafIndex++;
					tempnode2->stringDepth = tempnode2->parent->stringDepth + (tempnode2->edgelabel[1] - tempnode2->edgelabel[0] + 1);
					tempnode->stringDepth = tempnode->parent->stringDepth + (tempnode->edgelabel[1] - tempnode->edgelabel[0] + 1);
					//tempnode2->stringDepth = tempnode2->parent->stringDepth + (tempnode2->edgelabel[1] - tempnode2->edgelabel[0] + 1);
					this->leafNodes++;
					this->internalNodes++;
					if (tempnode2->stringDepth > this->deepestInternalNode->stringDepth)
					{
						this->deepestInternalNode = tempnode2;
					}
					this->sumInternalNodeDepths += tempnode2->stringDepth;
					NotFound = false;
					break;
					
				}
				else
				{
					startingIndex += 1;
				}
			}
		}
		StartingNode = tempnode;
	}
	return tempnode;
}

string SuffixTree::GetStringFromNode(Node* n)
{
	if (n == NULL)
	{
		return "";
	}
	string suffix = "";
	while (n!=this->root)
	{
		suffix = this->baseString.substr(n->edgelabel[0], (n->edgelabel[1] - n->edgelabel[0] + 1))+suffix;
		n = n->parent;
	}
	return suffix;
}

Node* SuffixTree::NodeHops(string beta, Node* startingNode)
{
	Node* tempnode = startingNode->Chilren[this->CharMap[beta[0]]], *newNode = NULL;
	if (beta == "")
	{
		return startingNode;
	}
	if (tempnode == NULL)
	{
		cout << "Tempnode is nullfisrt\n";
		exit(-1);
	}


	while (beta.length() > (tempnode->edgelabel[1] - tempnode->edgelabel[0] + 1))
	{
		if (tempnode == NULL)
		{
			cout << "Tempnode is nullsecond\n";
			exit(-1);
		}

		else
		{
			beta = beta.substr(tempnode->edgelabel[1] - tempnode->edgelabel[0] + 1);
			startingNode = tempnode;
			tempnode = startingNode->Chilren[this->CharMap[beta[0]]];
		}



	}
	if (beta.length() == (tempnode->edgelabel[1] - tempnode->edgelabel[0] + 1))
	{
		return tempnode;
	}
	else
	{
		newNode = new Node(this->AlphabetSize);
		newNode->edgelabel[0] = tempnode->edgelabel[0];
		newNode->edgelabel[1] = newNode->edgelabel[0] + beta.length() - 1;
		newNode->parent = startingNode;
		//newNode->suffixLink = NULL;
		tempnode->edgelabel[0] = newNode->edgelabel[1] + 1;
		startingNode->Chilren[this->CharMap[this->baseString[newNode->edgelabel[0]]]] = newNode;
		newNode->Chilren[this->CharMap[this->baseString[tempnode->edgelabel[0]]]] = tempnode;
		tempnode->parent = newNode;
		newNode->ID = this->currentInternalNodeIndex;
		this->currentInternalNodeIndex++;
		this->internalNodes++;
		newNode->stringDepth = newNode->parent->stringDepth + (newNode->edgelabel[1] - newNode->edgelabel[0] + 1);
		if (newNode->stringDepth > (this->deepestInternalNode->stringDepth))
		{
			this->deepestInternalNode = newNode;
		}
		this->sumInternalNodeDepths += newNode->stringDepth;
		return newNode;
	}
}

void SuffixTree::BuildingSuffix()
{
	Node* lastNewNode, *activeNode;
	string beta;
	//1. make the root node

	this->root = new Node(this->AlphabetSize);
	this->root->parent = root;
	this->root->suffixLink = root;
	root->ID = 0;
	root->stringDepth = 0;
	this->deepestInternalNode = this->root;

	//2. Set the lastNewNode to Findpath basestring, and root
	
	lastNewNode = FindPath(root, 0);
	this->EndNodes.push_front(lastNewNode);
	//Enumerate through the rest of the suffixes
	for (int i = 1; i < this->baseString.length(); i++)
	{
		if (lastNewNode->parent->suffixLink != NULL && lastNewNode->parent == this->root)
		{
			lastNewNode = this->FindPath(root, i);
			this->EndNodes.push_front(lastNewNode);
		}
		else if (lastNewNode->parent->suffixLink != NULL && lastNewNode->parent != this->root)
		{
			lastNewNode = this->FindPath(lastNewNode->parent->suffixLink, (i + lastNewNode->parent->suffixLink->stringDepth));
			this->EndNodes.push_front(lastNewNode);
		}
		else if (lastNewNode->parent->suffixLink == NULL && lastNewNode->parent->parent != this->root)
		{
			beta = this->baseString.substr(lastNewNode->parent->edgelabel[0], (lastNewNode->parent->edgelabel[1] - lastNewNode->parent->edgelabel[0] + 1));
			activeNode = this->NodeHops(beta, lastNewNode->parent->parent->suffixLink);
			lastNewNode->parent->suffixLink = activeNode;
			lastNewNode = this->FindPath(activeNode, i + activeNode->stringDepth);
			this->EndNodes.push_front(lastNewNode);
		}
		else if(lastNewNode->parent->suffixLink == NULL && lastNewNode->parent->parent == this->root)
		{
			beta = this->baseString.substr(lastNewNode->parent->edgelabel[0], (lastNewNode->parent->edgelabel[1] - lastNewNode->parent->edgelabel[0] + 1));
			if (beta.length() == 1)
			{
				beta = "";
			}
			else
			{
				beta = beta.substr(1);
			}
			activeNode = this->NodeHops(beta, lastNewNode->parent->parent->suffixLink);
			lastNewNode->parent->suffixLink = activeNode;
			cout << "ActiveNodeSepth: " << activeNode->stringDepth << "\nI: " << i << endl;
			lastNewNode = this->FindPath(activeNode, i + activeNode->stringDepth);
			this->EndNodes.push_front(lastNewNode);
		}
		else
		{
			cout << "Unknown error occurred :(\n";
			exit(-1);
		}
	}
	/*for (auto var :this->EndNodes)
	{
		cout << "Suffix: " << this->GetStringFromNode(var) << endl << "Id: " << var->ID << endl;
	}*/
}

void SuffixTree::DFSHelper(Node* n)
{
	if (n == NULL)
	{
		return;
	}
	else
	{
		ofstream outfile;
		outfile.open("DFS.txt", std::ios_base::app);
		//outfile.close();

		
		outfile << "Node ID: " << n->ID << " EdgeLabels: " << n->edgelabel[0] << "," << n->edgelabel[1] << " Depth: " << n->stringDepth << endl;
		outfile.close();
		for (Node* c : n->Chilren)
		{
			this->DFSHelper(c);
		}
	}
}

void SuffixTree::DepthFirstSearch()
{
	remove("DFS.txt");
	this->DFSHelper(this->root);
}

void SuffixTree::BWTIndex()
{
	remove("output.txt");
	this->BWTHelper(this->root);
}

void SuffixTree::BWTHelper(Node* n)
{
	int index;
	if (n == NULL)
	{
		return;
	}
	else
	{
		for (Node* c : n->Chilren)
		{
			BWTHelper(c);
		}
		if (n->ID >= 1 && n->ID <= this->baseString.length())
		{
			int index = n->ID - 2;
			if (index == -1)
			{
				index = this->baseString.length() - 1;
			}
			ofstream outfile;
			outfile.open("output.txt", std::ios_base::app);
			outfile << this->baseString[index]<<endl;
			outfile.close();
		}
	}
}

void SuffixTree::DisplayInformation()
{
	int avgDepth;
	if (this->internalNodes == 0)
	{
		avgDepth = 0;
	}
	else
	{
		avgDepth = this->sumInternalNodeDepths / this->internalNodes;
	}
	cout << "------------------------------------------------------------------------------\n";
	cout << "Internal Nodes: " << this->internalNodes << endl;
	cout << "Leaves: " << this->leafNodes << endl;
	cout << "Avg Internal Node String Depth: " << avgDepth << endl;
	cout << "Deepest Internal Node: " << this->deepestInternalNode->stringDepth<<endl;
	cout << "Longest Matching Repeat: " << this->GetStringFromNode(this->deepestInternalNode) << endl;
	list<int> endNodes;
	this->GetLeafsOfNode(this->deepestInternalNode, endNodes);
	cout << "Longest repeat Positions: " << endl;
	for (int i : endNodes)
	{
		cout << i << endl;
	}
}

void SuffixTree::DisplayConstructionTime()
{
	cout << "Time in milliseconds: " << this->Time*1000 << endl;
}

void SuffixTree::DisplayNodesChildren(Node* n)
{
	for (Node* a : n->Chilren)
	{
		if (a != NULL)
		{
			cout << "ID: " << a->ID << " Depth: " << a->stringDepth << endl;
		}
	}
}

void SuffixTree::GetLeafsOfNode(Node* s, list<int>& l)
{
	if (s == NULL)
	{
		return;
	}
	else if (s->ID >= 1 && s->ID <= this->baseString.length())
	{
		l.push_front(s->ID);
	}
	else
	{
		for (Node* n : s->Chilren)
		{
			this->GetLeafsOfNode(n, l);
		}
	}
}

void SuffixTree::PrintrandomNodes(Node* n)
{
	if (n == NULL)
	{
		return;
	}
	else
	{
		if (n->ID % 2 == 0)
		{
			this->DisplayNodesChildren(n);
		}
		for (Node* s : n->Chilren)
		{
			this->PrintrandomNodes(s);
		}
	}
}


