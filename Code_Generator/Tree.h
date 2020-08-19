#pragma once

#include "Lexem.h"
#include <stdlib.h>

enum {DIFFVALNODE = 1000, MAXEXPRSIZE = 20};

enum {COMMANDNUM = 4, OPNUM = 4};

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

struct tree_t* GenerateRandomTree(size_t numSent);

void FillSent(struct tree_t* tree);

void FillNode(struct tree_t* tree);

int* RandomIntArray(size_t numNode);

struct tree_t* CreateSearchTreeByArray(int* arr, size_t len);

void InsertNode(struct tree_t* tree, int value);

void PrintTree (struct node_t* top);
void PrintNode (struct lexem_t lex);

void FreeTree(struct node_t *t);

void PrintArr(int* arr, size_t len);

void inOrder(struct node_t* node);

enum command_t RandomCommand();
enum operation_t RandomOperation();

struct node_t* RandomExpr();
struct node_t* RandomBoolExpr();

void GenerateArithmOperations(struct node_t* top);

struct node_t* RandomVariable();
struct node_t* RandomSent();
struct node_t* RandomNum();

