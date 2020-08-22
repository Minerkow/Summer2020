#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <assert.h>
#include "Tree.h"
#include "Random.h"

int main() {
    srand(time(0));
    struct tree_t* tree = GenerateRandomTree(4);
    TreeIntoTxt(tree);
    //struct node_t* top = RandomSent();
    //assert(top);
    PrintTree(tree->top);
    //inOrder(tree->top);
    //FreeTree(tree->top);
    //free(tree->top);
    //free(tree);
    return 0;
}

