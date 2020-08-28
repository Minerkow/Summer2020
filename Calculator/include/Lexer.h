#pragma once

#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <values.h>
#include <ctype.h>

// lexem is operation | brace | number
enum lexem_kind_t { POISON1, OP , BRACE,  NUM ,  VARIABLE, COMMAND, SENTENSE, VOID, COMPAR_SIGNS};

// operation is: +, -, *, /
enum operation_t { POISON2, ADD, SUB, MUL, DIV };

// braces are: (, )
enum braces_t { POISON3 ,LBRAC , RBRAC, LFIGURBRAC, RFIGURBRAC };

//command is: print, ?, =, ;
enum command_t { POISON4, PRINT, INPUT, ASSIGN, END_COMMAND, IF, WHILE };

enum compar_signs_t {  POISON5, NOT, EQUAL, NOT_EQUAL, GREATER, LESS, EQ_OR_GR, EQ_OR_LESS};

enum {POISON = MAXINT, ERROR = 1};

// lexem is one of lexem kind entities
// if (l.kind == BRACE)
//   assert(l.lex.b == LBRAC || l.lex.b == RBRAC)
struct lexem_t {
    enum lexem_kind_t kind;
    union {
        enum operation_t op;
        enum braces_t b;
        enum command_t com;
        enum compar_signs_t cs;
        int num;
    } lex;
    size_t line;
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

bool iscomparsigns(const char* str, int i);