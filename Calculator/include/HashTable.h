#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

enum
{
    MAXLENWORD = 10,
    LENHASHTABLE = 10000,
    KEY = 3,
    ALFABETLEN = 68
};

struct variable_t* create_hash_table();
int exponent(int number, int degree);
int calc_hash(int hash, int numsymbol, char symbol);
int check_hash_table(struct variable_t* hash_table, int hash, char* word);
int variable_value(int hash, int value, bool input,  struct variable_t* ptr);