#pragma once

#include <stdlib.h>

struct variable_t {
    int value;
};

struct var_arr_t {
    struct variable_t* arr;
    size_t size;
};

struct var_arr_t* CreateVarArr();
void FreeVarArr(struct var_arr_t* varArr);

int AddVar(struct var_arr_t* varArr);