#include "Tree.h"
#include "Lexem.h"
#include <assert.h>
#include <stdio.h>

struct tree_t* CreateSearchTreeByArray(int* arr, size_t len) {
    struct tree_t* tree = calloc(1, sizeof(tree));
    for (int i = 0; i < len; ++i) {
        InsertNode(tree, arr[i]);
        //assert(tree->top);
    }
    //assert(tree->top);
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
    assert(tree->top);
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
    Inorder(node->left);
    printf ("%d ", node->lexem.lex.num);
    Inorder(node->right);
}

