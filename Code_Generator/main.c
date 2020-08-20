#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "Tree.h"
#include "Random.h"

int main() {
    srand(time(0));
    struct tree_t* tree = GenerateRandomTree(5);
    //struct node_t* top = RandomExpr();
    PrintTree(tree->top);
    //inOrder(tree->top);
    //FreeTree(tree->top);
    //free(tree->top);
    //free(tree);
    return 0;
}

