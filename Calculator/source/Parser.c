
#include <stdio.h>
#include "../include/Parser.h"
#include "../include/HashTable.h"
#include "../include/Lexer.h"

enum {MAX_CYCLE_LEN = 100000};

static struct lexem_t get_cur_lexem (int i, struct lex_array_t* ptr);
static int get_cur_size(struct lex_array_t* ptr);
static bool is_add_sub (int i);
static bool is_mul_div(int i);
static int is_brace(int i);
static bool is_assign(int i);
static bool is_stop(int i);
static bool is_num_input_variable(int i);
static bool is_print_assign(int i);


static struct lexem_t get_cur_lexem (int i, struct lex_array_t* ptr)
{
    static struct lex_array_t* larr = NULL;
    if (ptr != NULL) {
        larr = ptr;
    }
    assert(larr);
    return (larr->lexems[i]);
}

static int get_cur_size(struct lex_array_t* ptr)
{
    static struct lex_array_t* larr = NULL;
    if (ptr != NULL) {
        larr = ptr;
    }
    assert(larr);
    return (larr->size);
}

static bool is_add_sub (int i)
{
    return (get_cur_lexem(i, NULL).lex.op == ADD || get_cur_lexem(i, NULL).lex.op == SUB);
}

static bool is_mul_div(int i)
{
    return (get_cur_lexem(i, NULL).lex.op == MUL || get_cur_lexem(i, NULL).lex.op == DIV);
}

static int is_brace(int i)
{
    if (get_cur_lexem(i, NULL).lex.b == LBRAC && get_cur_lexem(i, NULL).kind == BRACE)
        return LBRAC;
    if (get_cur_lexem(i, NULL).lex.b == RBRAC && get_cur_lexem(i, NULL).kind == BRACE)
        return RBRAC;
    return POISON;
}

static bool is_assign(int i)
{
    return (get_cur_lexem(i, NULL).kind == COMMAND && get_cur_lexem(i, NULL).lex.com == ASSIGN);
}

static bool is_stop(int i)
{
    struct lexem_t lexem = get_cur_lexem(i, NULL);
    return (lexem.kind == COMMAND && lexem.lex.com == END_COMMAND);
}

static bool is_print(int i)
{
    struct lexem_t lexem = get_cur_lexem(i, NULL);
    return (lexem.kind == COMMAND && lexem.lex.com == PRINT);
}

static bool is_num_input_variable(int i)
{
    struct lexem_t lexem = get_cur_lexem(i, NULL);
    bool res = (lexem.kind == COMMAND && lexem.lex.com == INPUT)
                || (lexem.kind == VARIABLE)
                || (lexem.kind == NUM);
    return res;
}

static bool is_if(int i)
{
    struct lexem_t lexem = get_cur_lexem(i, NULL);
    return (lexem.kind == COMMAND && lexem.lex.com == IF);
}

static bool is_while(int i)
{
    struct lexem_t lexem = get_cur_lexem(i, NULL);
    return (lexem.kind == COMMAND && lexem.lex.com == WHILE);
}

static bool is_comparison(int i)
{
    return (get_cur_lexem(i, NULL).kind == COMPAR_SIGNS);
}

static enum braces_t is_figur_brace(int i)
{
    struct lexem_t lexem = get_cur_lexem(i, NULL);
    if (lexem.kind != BRACE)
        return false;
    switch (lexem.lex.b) {
        case LFIGURBRAC: return LFIGURBRAC;
        case RFIGURBRAC: return RFIGURBRAC;
        default:;
    }
    return false;
}

static bool is_print_assign(int i){
    return (get_cur_lexem(i, NULL).kind == COMMAND &&
            (get_cur_lexem(i, NULL).lex.com == PRINT ||
            get_cur_lexem(i, NULL).lex.com == ASSIGN));
}

int TxtLen(FILE* txt) {
    assert(txt);
    fseek(txt, 0, SEEK_END);
    int length = ftell(txt);
    fseek(txt, 0, SEEK_SET);
    return length;
}

void RunProgramm(int argc, char** argv){
    FILE* f = fopen(argv[1], "r");
    assert(f);
    size_t res = 0;
    struct lex_array_t larr = {};
    int len = TxtLen(f);
    char* inp = calloc(len + 1, sizeof(char));
    res = fread(inp, sizeof(char), len, f);
    assert(res != 0);

    larr = lex_string(inp);

    if (larr.lexems == NULL) {
        fprintf(stderr, "Larr ERROR, line - %d", __LINE__);
        exit(ERROR);
    }
    struct node_t* top = BuildTree(larr);
    Calculation(top);
    free_tree(top);
    free_all(larr);
    free(inp);
}

struct node_t* BuildTree (struct lex_array_t larr)
{
    int i = 0;
    get_cur_lexem(0, &larr);
    get_cur_size(&larr);
    struct node_t* top = Sentense(&i);
    if (top == NULL)
    {
        fprintf (stderr, "Empty input, Code Line - %d", __LINE__);
        exit(ERROR);
    }
    return top;
}

int Calculation(struct node_t* top){
    int l = 0;
    int r = 0;
    if (top == NULL)
        return 0;
    if (top->lexem.kind == VOID)
        return 0;
    if (top->lexem.kind == COMMAND) {
        switch (top->lexem.lex.com) {
            case PRINT:
                printf("%d\n", Calculation(top->left));
                return 0;
            case ASSIGN:
                if (top->left->lexem.kind != VARIABLE)
                {
                    fprintf(stderr, "Assign ERROR, line - %zu", top->left->lexem.line);
                    exit(ERROR);
                }
                r = Calculation(top->right);
                variable_value(top->left->lexem.lex.num, r, true, NULL);
                return 0;
            case IF:
                l = Calculation(top->left);
                if (l == true)
                    Calculation(top->right);
                return 0;
            case WHILE:
                l = Calculation(top->left);
                int i = 0;
                while (l == true) {
                    i++;
                    Calculation(top->right);
                    l = Calculation(top->left);
                    if (i == MAX_CYCLE_LEN)
                    {
                        fprintf(stderr, "Infinity cycle");
                        exit(ERROR);
                    }
                }
                return 0;
            case INPUT:
                fscanf(stdin, "%d", &l);
                return l;
            default: {
                fprintf(stderr, "Unknown command, line - %d", __LINE__);
                exit(ERROR);
                }
        }
    }
    if (top->lexem.kind == COMPAR_SIGNS) {
        r = Calculation(top->right);
        l = Calculation(top->left);
        switch (top->lexem.lex.cs) {
            case EQUAL: return (l == r);
            case NOT_EQUAL: return (l != r);
            case GREATER: return (l > r);
            case LESS: return (l < r);
            case EQ_OR_GR: return (l >= r);
            case EQ_OR_LESS: return (l <= r);
            case NOT: return (!l);
            default: {
                fprintf(stderr, "Unknown compar signs");
                exit(ERROR);
            }
        }
    }
    if (top->lexem.kind == SENTENSE)
    {
        Calculation(top->left);
        Calculation(top->right);
        return 0;
    }
    switch (top->lexem.kind)
    {
        case VARIABLE:
            return variable_value(top->lexem.lex.num, 0, false, NULL);
        case NUM:
            return top->lexem.lex.num;
        default:;
    }
    l = Calculation(top->left);
    r = Calculation(top->right);
    return Calc(l, r, top);
}


int Calc (int l, int r, struct node_t *top)
{
    switch (top->lexem.kind) {
        case OP:
            switch (top->lexem.lex.op){
                case ADD:
                    return l + r;
                case SUB:
                    return l - r;
                case MUL:
                    return l * r;
                case DIV:
                    if (r == 0){
                        fprintf(stderr, "Division by zero, line - %zu", top->lexem.line);
                        exit(ERROR);
                    }
                    return l / r;
                default:
                    fprintf(stderr, "Error operation, line - %d", __LINE__);
                    exit(ERROR);
            }
        default:
            printf("\n");
            print_node(top->lexem);
            printf("\n");
            fprintf(stderr, "Unknown operation, line - %d", __LINE__);
            exit(ERROR);
    }
}

struct node_t* Sentense(int* i){
    struct node_t* node = Create_Node();
    struct node_t* top = node;
    int num = 0;
    while((*i) <= get_cur_size(NULL) - 1 && is_figur_brace(*i) != RFIGURBRAC)
    {
        node->lexem.kind = SENTENSE;
        node->lexem.lex.num = num;
        num++;
        node->left = Comm(i);
        if(!is_stop(*i) && is_figur_brace(*i) != RFIGURBRAC)
        {
            fprintf(stderr, "\nExpected ';' : line - %zu, i - %d", get_cur_lexem(*i, NULL).line, *i);
            exit(ERROR);
        }
        (*i)++;
        node->right = Create_Node();
        node->right->lexem.kind = VOID;
        node = node->right;
    }
    return top;
}

struct node_t* Comm(int* i) {
    //<-- assert(if(i != NULL && "Comm shall have non-null input");
    struct node_t *node = NULL;
    struct node_t *comm_left = Comp(i);
    if (is_stop(*i))
        return comm_left;
    node = Create_Node();
    node->lexem = get_cur_lexem(*i, NULL);
    if (is_print(*i)){
        (*i)++;
       node->left = Comp(i);
        if (node->left == NULL) {
            fprintf(stderr, "Waiting expresion, line - %zu, i - %d", node->lexem.line, *i);
            exit(ERROR);
        }
       node->right = Create_Node();
       node->right->lexem.kind = VOID;
    }
    if (is_assign(*i)) {
        (*i)++;
       node->left = comm_left;
        if (node->left == NULL) {
            fprintf(stderr, "Waiting variable, line - %zu, i - %d", node->lexem.line, *i);
            exit(ERROR);
        }
       node->right = Comp(i);
        if (node->right == NULL) {
            fprintf(stderr, "Waiting expresion, line - %zu, i - %d", node->lexem.line, *i);
            exit(ERROR);
        }
    }
    if (is_if(*i) || is_while(*i)) {
        (*i)++;
        node->left = Comp(i);
        if (is_figur_brace(*i) != LFIGURBRAC) {
            fprintf(stderr, "Waiting figure brace - '{', line - %zu, i - %d", get_cur_lexem(*i, NULL).line, *i);
            exit(ERROR);
        }
        (*i)++;
        if (is_figur_brace(*i) == LFIGURBRAC) {
            fprintf(stderr, "Too many brace - '{', line - %zu, i - %d", get_cur_lexem(*i, NULL).line, *i);
            exit(ERROR);
        }
        node->right = Sentense(i);
        if (is_figur_brace(*i) != RFIGURBRAC) {
            fprintf(stderr, "Waiting figure brace - '}', line - %zu", get_cur_lexem(*i, NULL).line);
            exit(ERROR);
        }
    }


    comm_left = node;
    return comm_left;
}

struct node_t* Comp(int *i)
{
    //<-- assert(if(i != NULL && "Comp shall have non-null input");
    struct node_t* node = NULL;
    struct node_t* comp_left = Expr(i);
    if (is_stop(*i))
        return comp_left;
    while (is_comparison(*i))
    {
        if (is_stop(*i))
            return comp_left;
        node = Create_Node();
        node->lexem = get_cur_lexem(*i, NULL);
        if (node->lexem.lex.cs == NOT) {
            (*i)++;
            node->left = Expr(i);
            node->right = NULL;
            comp_left = node;
        } else {
            (*i)++;
            node->left = comp_left;
            if (node->left == NULL) {
                fprintf(stderr, "Waiting expresion , line - %zu, i - %d", get_cur_lexem(*i, NULL).line, *i);
                exit(ERROR);
            }
            node->right = Expr(i);
            if (node->right == NULL) {
                fprintf(stderr, "Waiting expresion , line - %zu, i - %d", get_cur_lexem(*i, NULL).line, *i);
                exit(ERROR);
            }
            comp_left = node;
        }
    }
    return comp_left;
}

struct node_t* Expr(int* i)
{
    //<-- assert(if(i != NULL && "Expr shall have non-null input");
    struct node_t* node = NULL;
    struct node_t* expr_left = Mult(i);
    if (is_stop(*i))
        return expr_left;

    while (is_add_sub(*i) && get_cur_lexem(*i, NULL).kind == OP)
    {
        if (is_stop(*i))
            return expr_left;

        node = Create_Node();
        node->lexem = get_cur_lexem(*i, NULL);
        (*i)++;

        struct lexem_t lexem = get_cur_lexem(*i, NULL);
        if (lexem.kind != NUM && lexem.kind != BRACE && lexem.kind != VARIABLE)
        {
            fprintf(stderr, "Expected expression, line - %zu", get_cur_lexem(*i, NULL).line);
            exit(ERROR);
        }
        node->left = expr_left;
        node->right = Mult(i);

        expr_left = node;
    }
    return expr_left;
}


struct node_t* Mult(int* i)
{
    //<-- assert(if(i != NULL && "Mult shall have non-null input");
    struct node_t* node = NULL;
    struct node_t* mult_left = Term(i);

    while (is_mul_div(*i) && get_cur_lexem(*i, NULL).kind == OP) {

        node = Create_Node();
        node->lexem = get_cur_lexem(*i, NULL);

        (*i)++;

        struct lexem_t lexem = get_cur_lexem(*i, NULL);
        if (lexem.kind != NUM && lexem.kind != BRACE && lexem.kind != VARIABLE)
        {
            fprintf(stderr,"Expected expression, line - %zu", get_cur_lexem(*i, NULL).line);
            exit(ERROR);
        }
        node->left = mult_left;
        node->right = Term(i);

        mult_left = node;
    }
    return mult_left;
}

struct node_t* Term (int* i)
{
    //<-- assert(if(i != NULL && "Term shall have non-null input");
    struct node_t* node = NULL;
    if (is_num_input_variable(*i))
    {
        node = Create_Node();
        node->lexem = get_cur_lexem(*i, NULL);
        if (get_cur_lexem(*i + 1, NULL).kind == NUM)
        {
            fprintf(stderr, "Two numbers in a row, line - %zu", get_cur_lexem(*i, NULL).line);
            exit(ERROR);
        }
        (*i)++;
        return node;
    }
    if(is_brace(*i) == LBRAC)
    {
        (*i)++;
        if (is_brace(*i) == RBRAC)
        {
            fprintf(stderr, "Extra brace, line - %zu", get_cur_lexem(*i, NULL).line);
            exit(ERROR);
        }
        node = Comp(i);
        if (is_brace(*i) != RBRAC)
        {
            fprintf(stderr, "Extra brace, line - %zu, i = %d", get_cur_lexem(*i, NULL).line, *i);
            exit(ERROR);
        }
        (*i)++;

        return node;
    }

    if (is_brace(*i) == RBRAC)
    {
        (*i)++;
        return node;
    }
    return 0;
}

struct node_t* Create_Node()
{
    struct node_t* node = (struct node_t*)calloc(1, sizeof(struct node_t));

    assert(node);

    return node;
}

void free_tree(struct node_t *t) {
    if (t == NULL) return;
    free_tree(t->left);
    free_tree(t->right);
    free(t);
}

void print_node (struct lexem_t lex) {
    switch (lex.kind) {
        case OP:
            switch (lex.lex.op) {
                case ADD:
                    printf ("ADD ");
                    break;
                case SUB:
                    printf ("SUB ");
                    break;
                case MUL:
                    printf ("MUL ");
                    break;
                case DIV:
                    printf ("DIV ");
                    break;
                default: {
                    fprintf(stderr, "Unknown operation");
                    exit(ERROR);
                }
            }
            break;
        case NUM:
            printf ("%d ", lex.lex.num);
            break;
        case VARIABLE:
            printf ("var_%d ", lex.lex.num);
            break;
        case COMMAND:
            switch (lex.lex.com){
                case PRINT:
                    printf("PRINT ");
                    break;
                case INPUT:
                    printf("INPUT ");
                    break;
                case ASSIGN:
                    printf("ASSIGN ");
                    break;
                case END_COMMAND:
                    printf("END_COMMAND ");
                    break;
                case IF:
                    printf("IF ");
                    break;
                case WHILE:
                    printf("WHILE ");
                    break;
                default: {
                    fprintf(stderr, "Unknown command");
                    exit(ERROR);
                }
            }
            break;
        case SENTENSE:
            printf ("SENTENSE #%d ", lex.lex.num);
            break;
        case VOID:
            printf ("VOID");
            break;
        case COMPAR_SIGNS:
            switch (lex.lex.cs) {
                case NOT: printf("NOT "); break;
                case EQUAL: printf("EQUAL "); break;
                case NOT_EQUAL: printf("NOT_EQUAL "); break;
                case GREATER: printf("GREATER "); break;
                case LESS: printf("LESS "); break;
                case EQ_OR_GR: printf("EQ_OR_GR "); break;
                case EQ_OR_LESS: printf("EQ_OR_LESS"); break;
                default: {
                    fprintf(stderr, "Unknown compar sings");
                    exit(ERROR);
                }
            }
            break;
        default:
            assert(0 && "ERROR READ");
    }
}

void print_tree (struct node_t* top) {
    if (top == NULL){
        fprintf (stderr, "Error: top is NULL, line - %d\n", __LINE__);
        return;
    }
    if (top->left == NULL && top->right == NULL)
        return;

    print_node (top->lexem);

    if (top->left == NULL) {
        printf ("NULL\n");
        return;
    }
    print_node (top->left->lexem);

    if (top->right == NULL)
    {
        printf("NULL ");
        return;
    }
    print_node (top->right->lexem);

    printf ("\n");

    print_tree (top->left);
    print_tree (top->right);
}

void free_all(struct lex_array_t larr)
{
    free(larr.lexems);
    variable_value(-1, 0, false, NULL);
}
