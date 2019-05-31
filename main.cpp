#include <stdio.h>
#include <ctime>
#include <fstream>
#include <omp.h>
#include <math.h>
#include <chrono>
#include <ctime>
#include <sched.h>
#include <stdlib.h>
#include <iostream>
#include <stdint.h>
#include <string>
#include <sched.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "Tree.h"
#include "TreeUtils.h"
#include <cmath>

using namespace std;

std::chrono::system_clock::time_point timeBefore, timeAfter;


const int level = 6;

void defaultSum(tnode* tree){
    timeBefore = std::chrono::high_resolution_clock::now();
    unsigned long long sum = getSumOfAllChilds(tree);
    timeAfter = std::chrono::high_resolution_clock::now();
    
    printf("[Default] Time: %lf\n", std::chrono::duration_cast<std::chrono::duration<double>>(timeAfter - timeBefore).count());
    printf("[Default] Sum: %llu\n", sum);
}

void pthreadSum(tnode* tree){
       
    int threads = pow(2, level) - 1; ;
    pthreadArg arg;
    arg.tree = tree;
    arg.threadCount = threads;
    printf("[Pthread] Number of threads: %llu\n", threads);
    timeBefore = std::chrono::high_resolution_clock::now();
    getSumOfAllChilds_Pthread((void *) &arg);
    timeAfter = std::chrono::high_resolution_clock::now();
        
    printf("[Pthread] Time: %lf\n", std::chrono::duration_cast<std::chrono::duration<double>>(timeAfter - timeBefore).count());
    printf("[Pthread] Sum: %llu\n", arg.tree->sum);
}

void openMpSum(tnode* tree){
       
    
    omp_set_nested(1);
    omp_set_max_active_levels(level-1);
    
    timeBefore = std::chrono::high_resolution_clock::now();
    unsigned long long sum = getSumOfAllChilds_OpenMP(tree);
    timeAfter = std::chrono::high_resolution_clock::now();
    
    printf("[OpenMP] Time: %lf\n", std::chrono::duration_cast<std::chrono::duration<double>>(timeAfter - timeBefore).count());
    printf("[OpenMP] Sum: %llu\n", sum);
}

int main(){
    tnode* tree = makeRandomTree();
    
    defaultSum(tree);
    openMpSum(tree);
    pthreadSum(tree);
    
    return 0;
}