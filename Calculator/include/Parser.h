#pragma once

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "Lexer.h"

enum { MAXLEN = 1024 };

struct node_t
{
    struct lexem_t lexem;
    struct node_t* right;
    struct node_t* left;
};

struct node_t* Sentense(int* i);
struct node_t* Comm(int* i);
struct node_t* Expr(int* i);
struct node_t* Mult(int* i);
struct node_t* Term(int* i);
struct node_t* Create_Node();
//int calc_result(struct node_t *top);
void RunProgramm(int argc, char** argv);
int Calculation(struct node_t* top);
int Calc (int l, int r, struct node_t *top);
struct node_t* BuildTree (struct lex_array_t larr);
void free_tree(struct node_t *t);
//void Analyzer (struct lex_array_t larr);
void free_all(struct lex_array_t larr);

void print_node (struct lexem_t lex);
void print_tree (struct node_t* top);

//comm ::= comm {input, print, assign} comm | comm
//expr ::= mult {+, -} expr | mult
//mult ::= term {*, /} mult | term
//term ::= ( expr ) | number | variable
