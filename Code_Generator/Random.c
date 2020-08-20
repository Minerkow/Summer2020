#include "Random.h"
#include "Variable.h"
#include "Lexem.h"
#include <assert.h>
#include <stdbool.h>

struct tree_t* GenerateRandomTree(size_t numSent) {
    struct tree_t* tree = calloc(1, sizeof(tree));
    tree->numSent = numSent;
    FillSent(tree);
    FillNode(tree);
    return tree;
}

void FillSent(struct tree_t* tree) {
    struct node_t* temp = NULL;
    for (int i = 0; i < tree->numSent; ++i) {
        temp = CreateNode();
        temp->lexem.kind = SENTENSE;
        temp->lexem.lex.num = i;
        if (i == 0)
            tree->top = temp;
    }
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
                node->left = RandomExpr();
                break;
            case ASSIGN:
                node->left = RandomVariable();
                node->right = RandomExpr();
                break;
            case IF:
                node->left = RandomBoolExpr();
                node->right = RandomSent();
                break;
            case WHILE:
                node->left = RandomBoolExpr();
                node->right = RandomSent();
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
    PrintArr(res, numNode);
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

struct node_t* RandomExpr() {
    int exprSize = rand() % MAX_EXPR_SIZE;
    int* arr = RandomIntArray(exprSize);
    struct tree_t* exprTree = CreateSearchTreeByArray(arr, exprSize);
    GenerateArithmOperations(exprTree->top);
    return exprTree->top;
}

void GenerateArithmOperations(struct node_t* top) {
    if (top->left == NULL && top->right == NULL) {
        return;
    }
    top->lexem.kind = OP;
    top->lexem.lex.op = RandomOperation();
    if (top->left == NULL && top->right != NULL) {
        top->left = RandomVariable();
        if (top->left == NULL)
            top->left = RandomNum();
    }
    if (top->right == NULL && top->left != NULL) {
        top->right = RandomVariable();
        if (top->right == NULL)
            top->right = RandomNum();
    }
    GenerateArithmOperations(top->left);
    GenerateArithmOperations(top->right);
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
    }
    if (delete == DESTROY) {
        FreeVarArr(varArr);
    }
    struct node_t* var = CreateNode();
    var->lexem.kind = VARIABLE;
    if (add == true)
        var->lexem.lex.num = AddVar(varArr);
    if (add == false) {
        if (varArr == 0)
            return NULL;
        var->lexem.lex.num = rand() % varArr->size;
    }
}

struct node_t* RandomSent() {
    //TODO:: RandomSent
    

}

struct node_t* RandomBoolExpr() {
    //TODO:: RandomBoolExpr
}

struct node_t* RandomNum() {
    struct node_t* res = NULL;
    res = CreateNode();
    res->lexem.kind = NUM;
    res->lexem.lex.num = rand() % DIFF_VAL_NODE;
    return res;
}
