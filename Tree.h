#pragma once

#include <iostream>

using namespace std;


struct tnode
{
	unsigned long long value = 0;   
	unsigned long long sum = 0;	 
	struct tnode *left = NULL;	
	struct tnode *right =NULL;	
};


tnode* addNode(unsigned long long v, tnode *tree);

tnode* makeNewTree(unsigned long long v, tnode *tree);
