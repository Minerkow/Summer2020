#include "Variable.h"
#include "Random.h"

struct var_arr_t* CreateVarArr() {
    struct var_arr_t* varArr = calloc(1, sizeof(varArr));
    varArr->size = 0;
    varArr->arr = calloc(MAX_VARIABLE, sizeof(struct variable_t));
    for (int i = 0; i < MAX_VARIABLE; ++i) {
        varArr->arr[i].value = -1;
    }
    return varArr;
}

void FreeVarArr(struct var_arr_t* varArr) {
    for(int i = 0; i < varArr->size; ++i)
        free(varArr->arr[i].name);
    free(varArr->arr);
    free(varArr);
}

int AddVar(struct var_arr_t* varArr) {
    int i = varArr->size;
    varArr->arr[i].value = 0;
    varArr->size++;
    int varLen = rand() % MAX_LEN_VAR + 1;
    varArr->arr[i].name = calloc(varLen, sizeof(char));
    for (int j = 0; j < varLen; ++j) {
        varArr->arr[i].name[j] = 'a' + rand() % ('z' - 'a');
    }
    //printf(varArr->arr[i].name);
    return i;
}