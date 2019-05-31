#include <ctime>
#include <fstream>
#include <omp.h>
#include <pthread.h>

#include "TreeUtils.h"

unsigned long long llrand() {
    unsigned long long r = 0;

    for (int i = 0; i < 5; ++i) {
        r = (r << 15) | rand(); 
    }

    return r & MAX_VALUE;
}

tnode* makeRandomTree() {

    tnode* tree = new tnode;
    tree->value = MAX_VALUE / 2; 

    srand(unsigned(time(NULL)));
    for (int i = 0; i < GENERATE_COUNT; i++) {
        unsigned long long random = llrand();
        addNode(random, tree);
    }
    
    printf("Random tree generated\n");
    printf("Number of nodes: %llu\n", GENERATE_COUNT);
    return tree;
}


unsigned long long getSumOfAllChilds(tnode* tree) {
	if (tree != NULL){
		unsigned long long leftSum = 0;
		unsigned long long rightSum = 0;

		if (tree->left != NULL) {
			tree->left->sum = getSumOfAllChilds(tree->left);
			leftSum = tree->left->sum + tree->left->value;
		}

		if (tree->right != NULL)
		{
			tree->right->sum = getSumOfAllChilds(tree->right);
			rightSum = tree->right->sum + tree->right->value;
		}

		return leftSum + rightSum;
	}
	return 0;
}

unsigned long long getSumOfAllChilds_OpenMP(tnode* tree) {
    if (tree != NULL) {
        unsigned long long leftSum = 0;
	unsigned long long rightSum = 0;
        
	if (omp_get_active_level() >= omp_get_max_active_levels ())
            return getSumOfAllChilds(tree);

        #pragma omp parallel num_threads(2) 
        {
            #pragma omp sections
            {
                #pragma omp section 
                { 
                   
                    if (tree->left != NULL){
                        tree->left->sum = getSumOfAllChilds_OpenMP(tree->left);
                        leftSum = tree->left->sum + tree->left->value;
                    }
		}

		#pragma omp section 
		{ 
                    
                    if (tree->right != NULL){
                        tree->right->sum = getSumOfAllChilds_OpenMP(tree->right);
                        rightSum = tree->right->sum + tree->right->value;
                    }				
		} 
            }
	}
        return leftSum + rightSum;
    }
    return 0;
}

void* getSumOfAllChilds_Pthread(void *args){
    pthreadArg *arg = (pthreadArg *)args; 
	
    if (arg->tree != NULL){	
        unsigned long long leftSum = 0; 
	unsigned long long rightSum = 0;
		
	
	if (arg->threadCount <= 1){
            arg->tree->sum = getSumOfAllChilds(arg->tree);
            return 0;
	}
        int leftJoinStatus, rightJoinStatus; 
        int leftCreateStatus, rightCreateStatus;

	
	pthread_t leftThread;
	pthreadArg leftArg;
	if (arg->tree->left != NULL){		
            leftArg.tree = arg->tree->left;
            leftArg.threadCount = (arg->threadCount - 1)/2;
            leftCreateStatus = pthread_create(&leftThread, NULL, getSumOfAllChilds_Pthread, (void*) &leftArg);	
            if (leftCreateStatus != 0) {
                printf("[ERROR] Can't create thread. Status: %d\n", leftCreateStatus);
		exit(ERROR_CREATE_THREAD);
            }
	}

	
	pthread_t rightThread;
	pthreadArg rightArg;
	if (arg->tree->right != NULL){		
            rightArg.tree = arg->tree->right;
            rightArg.threadCount =  (arg->threadCount - 1)/2;
            rightCreateStatus = pthread_create(&rightThread, NULL, getSumOfAllChilds_Pthread, (void*) &rightArg);	
            if (rightCreateStatus != 0) {
                printf("[ERROR] Can't create thread. Status: %d\n", rightCreateStatus);
		exit(ERROR_CREATE_THREAD);
            }
	}		
		
   
	leftCreateStatus = pthread_join(leftThread, (void**)&leftJoinStatus);
	if (leftCreateStatus != SUCCESS) {
            printf("[ERROR] Can't join thread. Status: %d\n", leftCreateStatus);
            exit(ERROR_JOIN_THREAD);
	}
	if (arg->tree->left != NULL){
            arg->tree->left->sum = leftArg.tree->sum;
            leftSum = arg->tree->left->sum + arg->tree->left->value;
	}

	rightCreateStatus = pthread_join(rightThread, (void**)&rightJoinStatus);
	if (rightCreateStatus != SUCCESS) {
            printf("[ERROR] Can't join thread. Status: %d\n", rightCreateStatus);
            exit(ERROR_JOIN_THREAD);
	}
	if (arg->tree->right != NULL){
            arg->tree->right->sum = rightArg.tree->sum;
            rightSum = arg->tree->right->sum + arg->tree->right->value;
	}
		
	arg->tree->sum = leftSum + rightSum;
    }
    return 0;
}