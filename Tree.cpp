#include <stdio.h>
#include <fstream>
#include <iostream>

#include "Tree.h"

using namespace std;

tnode* addNode(unsigned long long v, tnode *tree)
{
	if (tree == NULL)
	{
		tree = makeNewTree(v, tree);
	}
	else if (v < tree->value)	
		tree->left = addNode(v, tree->left);
	else if(v > tree->value)
		tree->right = addNode(v, tree->right);
	return(tree);
}


tnode* makeNewTree(unsigned long long v, tnode *tree)
{
    tree = new tnode;		
    tree->value = v;		
    tree->sum = 0;			
    tree->right = NULL;
    tree->left = NULL;		
    return(tree);
}