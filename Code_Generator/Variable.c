#include "Variable.h"
#include "Random.h"

struct var_arr_t* CreateVarArr() {
    struct var_arr_t* varArr = calloc(1, sizeof(varArr));
    varArr->arr = calloc(MAX_VARIABLE, sizeof(struct variable_t));
    for (int i = 0; i < MAX_VARIABLE; ++i) {
        varArr->arr[i].value = -1;
    }
    return varArr;
}

void FreeVarArr(struct var_arr_t* varArr) {
    free(varArr->arr);
    free(varArr);
}

int AddVar(struct var_arr_t* varArr) {
    int i = 0;
    while(i != -1) {
        ++i;
    }
    varArr->arr[i].value = 0;
    varArr->size++;
    return i;
}