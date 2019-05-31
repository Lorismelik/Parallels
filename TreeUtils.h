#pragma once

#include "Tree.h"

#define MAX_VALUE 0xFFFFFFFF        
#define GENERATE_COUNT 1000000  

#define SUCCESS 0
#define ERROR_CREATE_THREAD -1
#define ERROR_JOIN_THREAD   -2

struct pthreadArg {
    struct tnode *tree;
    int threadCount;
};

tnode* makeRandomTree();
unsigned long long getSumOfAllChilds(tnode* tree);
unsigned long long getSumOfAllChilds_OpenMP(tnode* tree);
void* getSumOfAllChilds_Pthread(void *args);