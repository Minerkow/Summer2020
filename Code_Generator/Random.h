#pragma once

#include <stdbool.h>
#include "Tree.h"
#include "Lexem.h"
#include "Variable.h"

enum {DIFF_VAL_NODE = 1000, MAX_EXPR_SIZE = 10, MAX_VARIABLE = 10, MAX_LEN_VAR = 4, MAX_LEN_FUNC_BODY = 5};

enum {COMMAND_NUM = 4, OP_NUM = 4, COMP_SIGN_NUM = 6, DESTROY = true, CAN_USE_ALL_VAR = -1};

struct tree_t* GenerateRandomTree(size_t numSent);

void FillSent(struct tree_t* tree);

void FillNode(struct tree_t* tree);

int* RandomIntArray(size_t numNode);

enum command_t RandomCommand();
enum operation_t RandomOperation();

struct node_t* RandomExpr(int withoutVariable);
struct node_t* RandomBoolExpr();

void GenerateArithmOperations(struct node_t* top, int withoutVariable);

struct node_t* RandomVariable(bool delete, bool add);
struct node_t* RandomSent();
struct node_t* RandomNum();
