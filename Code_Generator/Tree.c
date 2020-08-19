#include "Tree.h"
#include "Lexem.h"
#include <time.h>
#include <assert.h>
#include <stdio.h>

struct tree_t* GenerateRandomTree(size_t numSent) {
    struct tree_t* tree = calloc(1, sizeof(tree));
    tree->numSent = numSent;
    FillSent(tree);
    FillNode(tree);
    return tree;
}

int* RandomIntArray(size_t numNode) {
    int* res = calloc(numNode, sizeof(int));
    assert(res);
    for (int i = 0; i < numNode; ++i) {
        res[i] = rand() % DIFFVALNODE;
    }
    PrintArr(res, numNode);
    return res;
}

struct tree_t* CreateSearchTreeByArray(int* arr, size_t len) {
    struct tree_t* tree = calloc(1, sizeof(tree));
    for (int i = 0; i < len; ++i) {
        InsertNode(tree, arr[i]);
    }
    return tree;
}

void InsertNode(struct tree_t* tree, int value) {
    struct node_t* node = CreateNode();
    node->lexem.kind = NUM;
    node->lexem.lex.num = value;
    if (tree->top == NULL) {
        tree->top = node;
        return;
    }
    struct node_t* temp = tree->top;
    struct node_t* parent = tree->top;
    while (temp != NULL) {
        parent = temp;
        if (value >= temp->lexem.lex.num) {
            temp = temp->right;
        } else {
            temp = temp->left;
        }
    }
    assert(!temp);
    assert(parent);
    if (value >= parent->lexem.lex.num) {
        parent->right = node;
        //printf ("_%d_", value);
    } else {
        parent->left = node;
        //printf ("_%d_", value);
    }
}

void PrintNode (struct lexem_t lex) {
    switch (lex.kind) {
        case OP:
            switch (lex.lex.op) {
                case ADD:
                    printf ("ADD ");
                    break;
                case SUB:
                    printf ("SUB ");
                    break;
                case MUL:
                    printf ("MUL ");
                    break;
                case DIV:
                    printf ("DIV ");
                    break;
                default: {
                    fprintf(stderr, "Unknown operation");
                    exit(ERROR);
                }
            }
            break;
        case NUM:
            printf ("%d ", lex.lex.num);
            break;
        case VARIABLE:
            printf ("var_%d ", lex.lex.num);
            break;
        case COMMAND:
            switch (lex.lex.com){
                case PRINT:
                    printf("PRINT ");
                    break;
                case INPUT:
                    printf("INPUT ");
                    break;
                case ASSIGN:
                    printf("ASSIGN ");
                    break;
                case END_COMMAND:
                    printf("END_COMMAND ");
                    break;
                case IF:
                    printf("IF ");
                    break;
                case WHILE:
                    printf("WHILE ");
                    break;
                default: {
                    fprintf(stderr, "Unknown command");
                    exit(ERROR);
                }
            }
            break;
        case SENTENSE:
            printf ("SENTENSE #%d ", lex.lex.num);
            break;
        case VOID:
            printf ("VOID");
            break;
        case COMPAR_SIGNS:
            switch (lex.lex.cs) {
                case NOT: printf("NOT "); break;
                case EQUAL: printf("EQUAL "); break;
                case NOT_EQUAL: printf("NOT_EQUAL "); break;
                case GREATER: printf("GREATER "); break;
                case LESS: printf("LESS "); break;
                case EQ_OR_GR: printf("EQ_OR_GR "); break;
                case EQ_OR_LESS: printf("EQ_OR_LESS"); break;
                default: {
                    fprintf(stderr, "Unknown compar sings");
                    exit(ERROR);
                }
            }
            break;
        default:
            assert(0 && "ERROR READ");
    }
}

void PrintTree (struct node_t* top) {
    if (top == NULL){
        fprintf (stderr, "Error: top is NULL, line - %d\n", __LINE__);
        return;
    }
    if (top->left == NULL && top->right == NULL)
        return;

    PrintNode (top->lexem);

    if (top->left == NULL) {
        printf ("NULL ");
        //return;
    } else {
        PrintNode(top->left->lexem);
    }

    if (top->right == NULL)
    {
        //printf ("_%d_", top->right->lexem.lex.num);
        printf("NULL ");
        //return;
    } else {
        PrintNode(top->right->lexem);
    }

    printf ("\n");
    if (top->left != NULL) {
        PrintTree(top->left);
    }
    if (top->right != NULL) {
        PrintTree(top->right);
    }
}


struct node_t* CreateNode() {
    return calloc(1, sizeof(struct node_t));
}

void FreeTree(struct node_t *t) {
    if (t == NULL) return;
    FreeTree(t->left);
    FreeTree(t->right);
    free(t);
}

void PrintArr(int* arr, size_t len) {
    printf ("{");
    for (int i = 0; i < len; ++i) {
        printf ("%d, ", arr[i]);
    }
    printf ("}\n");
}

void Inorder(struct node_t* node){
    if (node == NULL) return;
    inOrder(node->left);
    printf ("%d ", node->lexem.lex.num);
    inOrder(node->right);
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

enum command_t RandomCommand() {
    int r = rand() % COMMANDNUM;
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
    int exprSize = rand() % MAXEXPRSIZE;
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
    int rnd  = rand() % OPNUM;
    switch(rnd) {
        case 0: return ADD;
        case 1: return SUB;
        case 2: return MUL;
        case 3: return DIV;
        default: assert(0 && "ERROR");
    }
}

struct node_t* RandomVariable() {
    //TODO:: RandomVariable: return NULL, if Var not found
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
    res->lexem.lex.num = rand() % DIFFVALNODE;
    return res;
}

