#pragma once

#include <stdlib.h>

enum {DONT_RETURN_NAME = -1};

struct variable_t {
    int value;
    char* name;
};

struct var_arr_t {
    struct variable_t* arr;
    size_t size;
};

struct var_arr_t* CreateVarArr();
void FreeVarArr(struct var_arr_t* varArr);

int AddVar(struct var_arr_t* varArr);
char* VariableName(int numVar, struct var_arr_t* varArr);