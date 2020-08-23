#include "Random.h"
#include "Variable.h"
#include "Lexem.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

struct tree_t* GenerateRandomTree(size_t numSent) {
    struct tree_t* tree = calloc(1, sizeof(tree));
    tree->numSent = numSent;
    FillSent(tree);
    FillNode(tree);
    return tree;
}

void FillSent(struct tree_t* tree) {
    struct node_t* temp = CreateNode();
    tree->top = temp;
    temp->lexem.kind = SENTENSE;
    temp->lexem.lex.num = 0;
    for (int i = 1; i < tree->numSent; ++i) {
        temp->right = CreateNode();
        temp->right->lexem.kind = SENTENSE;
        temp->right->lexem.lex.num = i;
        temp = temp->right;
    }
    temp->right = CreateNode();
    temp->right->lexem.kind = VOID;
}

void FillNode(struct tree_t* tree) {
    struct node_t* temp = tree->top;
    struct node_t* node = NULL;
    while (temp->lexem.kind != VOID) {
        temp->left = CreateNode();
        temp->left->lexem.kind = COMMAND;
        temp->left->lexem.lex.com = RandomCommand();
        node = temp->left;
        switch (node->lexem.lex.com) {
            case PRINT:
                node->left = RandomExpr(CAN_USE_ALL_VAR);
                break;
            case ASSIGN:
                node->left = RandomVariable(false, true);
                node->right = RandomExpr(node->left->lexem.lex.num);
                break;
            case IF:
                node->left = RandomBoolExpr();
                node->right = GenerateRandomTree(rand() % MAX_LEN_FUNC_BODY)->top;
                break;
            case WHILE:
                node->left = RandomBoolExpr();
                node->right = GenerateRandomTree(rand() % MAX_LEN_FUNC_BODY)->top;
                break;
            default:
                assert(0 && "ERROR");
        }
        temp = temp->right;
    }
}

int* RandomIntArray(size_t numNode) {
    int* res = calloc(numNode, sizeof(int));
    assert(res);
    for (int i = 0; i < numNode; ++i) {
        res[i] = rand() % DIFF_VAL_NODE;
    }
    //PrintArr(res, numNode);
    return res;
}

enum command_t RandomCommand() {
    int r = rand() % COMMAND_NUM;
    switch (r) {
        case 0: return PRINT;
        case 1: return ASSIGN;
        case 2: return IF;
        case 3: return WHILE;
        default:;
    }
    return PRINT;
}

struct node_t* RandomExpr(int withoutVarable) {
    int exprSize = rand() % MAX_EXPR_SIZE + 1;
    int* arr = RandomIntArray(exprSize);
    struct tree_t* exprTree = CreateSearchTreeByArray(arr, exprSize);
    assert(exprTree->top);
    GenerateArithmOperations(exprTree->top, withoutVarable);
    return exprTree->top;
}

void GenerateArithmOperations(struct node_t* top, int withoutVariable) {
    assert(top);
    if (top->left == NULL && top->right == NULL) {
        return;
    }
    top->lexem.kind = OP;
    top->lexem.lex.op = RandomOperation();
    if (top->left == NULL && top->right != NULL) {
        top->left = RandomVariable(false, false);
        if (top->left == NULL || top->left->lexem.lex.num == withoutVariable) {
            top->left = RandomNum();
            assert(top->left);
        }
    }
    if (top->right == NULL && top->left != NULL) {
        top->right = RandomVariable(false, false);
        if (top->right == NULL || top->right->lexem.lex.num == withoutVariable) {
            top->right = RandomNum();
            assert(top->right);
        }
    }
    GenerateArithmOperations(top->left, withoutVariable);
    GenerateArithmOperations(top->right, withoutVariable);
}

enum operation_t RandomOperation() {
    int rnd  = rand() % OP_NUM;
    switch(rnd) {
        case 0: return ADD;
        case 1: return SUB;
        case 2: return MUL;
        case 3: return DIV;
        default: assert(0 && "ERROR");
    }
}

struct node_t* RandomVariable(bool delete, bool add) {
    static struct var_arr_t* varArr = NULL;
    if (varArr == NULL) {
        varArr = CreateVarArr();
        VariableName( 0, varArr);
    }
    if (delete == DESTROY) {
        FreeVarArr(varArr);
    }
    struct node_t* var = CreateNode();
    var->lexem.kind = VARIABLE;
    if (add == true) {
        var->lexem.lex.num = AddVar(varArr);
        if (var->lexem.lex.num == -1)
            var->lexem.lex.num = rand() % varArr->size;
    }
    if (add == false) {
        if (varArr->size == 0)
            return NULL;
        var->lexem.lex.num = rand() % varArr->size;
    }
    return var;
}

struct node_t* RandomSent() {
    int rnd = rand() % MAX_LEN_FUNC_BODY + 1;
    struct tree_t* tree = calloc(1, sizeof(tree));
    tree->numSent = rnd;
    FillSent(tree);
    FillNode(tree);
    struct node_t* temp = tree->top;
    free(tree);
    return temp;
}



struct node_t* RandomBoolExpr() {
    struct node_t* node = CreateNode();
    node->lexem.kind = COMPAR_SIGNS;
    node->lexem.lex.cs = rand() % COMP_SIGN_NUM + EQUAL;
    node->left = RandomExpr(CAN_USE_ALL_VAR);
    node->right = RandomExpr(CAN_USE_ALL_VAR);
    return node;
}

struct node_t* RandomNum() {
    struct node_t* res = NULL;
    res = CreateNode();
    res->lexem.kind = NUM;
    res->lexem.lex.num = rand() % DIFF_VAL_NODE;
    return res;
}
