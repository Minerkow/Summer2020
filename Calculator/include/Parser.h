#pragma once

#include "Lexer.h"

struct node_t
{
    struct lexem_t lexem;
    struct node_t* right;
    struct node_t* left;
};

struct node_t* Comm(int* i);
struct node_t* Expr(int* i);
struct node_t* Mult(int* i);
struct node_t* Term(int* i);
struct node_t* Create_Node();
int calc_result(struct node_t *top);
int calc (int l, int r, struct node_t *top);
void BuildTree (struct lex_array_t larr, int* j);
void free_tree(struct node_t *t);
void Analyzer (struct lex_array_t larr);
void free_all(struct lex_array_t larr);

void print_node (struct lexem_t lex);
void print_tree (struct node_t* top);