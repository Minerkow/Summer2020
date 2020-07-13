#include <stdio.h>
#include <assert.h>

#include "include/Lexer.h"
#include "include/Parser.h"

enum { MAXLEN = 1024 };


int main() {
    FILE* f = fopen("/home/bibi/CLionProject/Vladimirov/Calculator/test.expr", "r");
    assert(f);
    int res = 0;
    struct lex_array_t larr = {};
    char inp[MAXLEN] = {0};

    res = fscanf( f,"%1023c", inp);
    assert(res == 1);

    larr = lex_string(inp);

    if (larr.lexems == NULL) {
        printf("ERROR\n");
        return 0;
    }
    dump_lexarray(larr);
    printf("\n");
    struct node_t* top = BuildTree(larr);
    print_tree(top);
    return 0;
}