#pragma once

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <values.h>

// lexem is operation | brace | number
enum lexem_kind_t {  OP , BRACE, NUM ,  VARIABLE, COMMAND, SENTENSE, VOID};

// operation is: +, -, *, /
enum operation_t { ADD, SUB, MUL, DIV };

// braces are: (, )
enum braces_t {  LBRAC , RBRAC };

//command is: print, ?, =, ;
enum command_t { PRINT, INPUT, ASSIGN, END_COMMAND};

enum {POISON = MAXINT};

// lexem is one of lexem kind entities
// if (l.kind == BRACE)
//   assert(l.lex.b == LBRAC || l.lex.b == RBRAC)
struct lexem_t {
    enum lexem_kind_t kind;
    union {
        enum operation_t op;
        enum braces_t b;
        enum command_t com;
        int num;
    } lex;
};

// array of lexems
// size: actual number of elements
// capacity: number of additions before realloc
struct lex_array_t {
    struct lexem_t *lexems;
    int size, capacity;
};
//initial sentence
enum {ISEN = 10};

// initial capacity
enum { ICAP = 10 };

// string to lexem array: see Problem LX on slides
struct lex_array_t lex_string(const char *str);

// printinvoid print_lexem(struct lexem_t lxm);
void dump_lexarray(struct lex_array_t pl);
