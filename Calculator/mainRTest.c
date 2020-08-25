#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <assert.h>
#include "Tree.h"
#include "Random.h"

int main(int argc, char** argv) {
    srand(time(0));
    assert(argv && "\nAdd argument: file for save\n");
    struct tree_t* tree = GenerateRandomTree(NUM_SENT);
    TreeIntoTxt(tree, argv);
    return 0;
}

