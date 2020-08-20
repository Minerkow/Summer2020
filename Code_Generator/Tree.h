#pragma once

#include "Lexem.h"
#include <stdlib.h>

struct tree_t {
    struct node_t* top;
    size_t numSent;
};

struct node_t {
    struct node_t* left;
    struct node_t* right;
    struct lexem_t lexem;
};

struct node_t* CreateNode();

struct tree_t* CreateSearchTreeByArray(int* arr, size_t len);

void InsertNode(struct tree_t* tree, int value);

void PrintTree (struct node_t* top);
void PrintNode (struct lexem_t lex);

void FreeTree(struct node_t *t);

void PrintArr(int* arr, size_t len);

void Inorder(struct node_t* node);



