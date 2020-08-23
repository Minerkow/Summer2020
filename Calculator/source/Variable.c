#include <assert.h>
#include "Variable.h"
#include "Random.h"

struct var_arr_t* CreateVarArr() {
    struct var_arr_t* varArr = calloc(1, sizeof(varArr));
    varArr->size = 0;
    assert(MAX_VARIABLE >= 1 && "Please, choose more variables");
    varArr->capacity = MAX_VARIABLE;
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
    if (varArr->size == varArr->capacity)
        return -1;
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

char* VariableName(int numVar, struct var_arr_t* varArr) {
    static struct var_arr_t* vArr = NULL;
    if (varArr != NULL) {
        vArr = varArr;
        return NULL;
    }
    assert(vArr->size >= numVar);
    return vArr->arr[numVar].name;
}