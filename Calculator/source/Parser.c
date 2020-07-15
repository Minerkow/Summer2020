
#include "../include/Parser.h"
#include "../include/HashTable.h"
#include "../include/Lexer.h"

static struct lexem_t get_cur_lexem (int i, struct lex_array_t* ptr);
static int get_cur_size(struct lex_array_t* ptr);
static bool is_add_sub (int i);
static bool is_mul_div(int i);
static int is_brace(int i);
static bool is_assign(int i);
//static bool is_end(int i);
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

static bool is_print_assign(int i){
    return (get_cur_lexem(i, NULL).kind == COMMAND &&
            (get_cur_lexem(i, NULL).lex.com == PRINT ||
            get_cur_lexem(i, NULL).lex.com == ASSIGN));
}

void RunProgramm(){
    //FILE* f = fopen("/home/bibi/CLionProject/Vladimirov/Calculator/test.expr", "r");
    //assert(f);
    int res = 0;
    struct lex_array_t larr = {};
    char inp[MAXLEN] = {0};

    res = scanf( "%1023c", inp);
    assert(res == 1);

    larr = lex_string(inp);

    if (larr.lexems == NULL) {
        fprintf(stderr, "Larr ERROR, line - %d", __LINE__);
        exit(1);
    }
    //dump_lexarray(larr);
    //printf("\n");
    struct node_t* top = BuildTree(larr);
    //print_tree(top);
    Calculation(top);
    free_tree(top);
    free_all(larr);
}

struct node_t* BuildTree (struct lex_array_t larr)
{
    int i = 0;
    get_cur_lexem(0, &larr);
    get_cur_size(&larr);
    struct node_t* top = Sentense(&i);
    if (top == NULL)
    {
        fprintf (stderr, "Empty input, line - %d", __LINE__);
        exit(1);
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
    if (top->lexem.kind == COMMAND && top->lexem.lex.com == PRINT)
    {
        printf("%d\n", Calculation(top->left));
        return 0;
    }
    if (top->lexem.kind == COMMAND && top->lexem.lex.com == ASSIGN){
        if (top->left->lexem.kind != VARIABLE)
        {
            fprintf(stderr, "Assign ERROR, line - %d", __LINE__);
            exit(1);
        }
        r = Calculation(top->right);
        variable_value(top->left->lexem.lex.num, r, true, NULL);
        return 0;
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
        case COMMAND:
            if (top->lexem.lex.com == INPUT)
            {
                int input = 0;
                fscanf(stdin,"%d", &input);
                return input;
            }
            break;
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
                    return l / r;
                default:
                    fprintf(stderr, "Error operation, line - %d", __LINE__);
                    exit(101);
            }
        default:
            printf("\n");
            print_node(top->lexem);
            printf("\n");
            fprintf(stderr, "Unknown operation, line - %d", __LINE__);
            exit(1);
    }
}

struct node_t* Sentense(int* i){
    struct node_t* node = Create_Node();
    struct node_t* top = node;
    int num = 0;
    while((*i) <= get_cur_size(NULL) - 1)
    {
        node->lexem.kind = SENTENSE;
        node->lexem.lex.num = num;
        num++;
        node->left = Comm(i);
        if(!is_stop(*i))
        {
            fprintf(stderr, "\nExpected ';' : line - %d, i - %d", __LINE__, *i);
            exit(0);
        }
        (*i)++;
        node->right = Create_Node();
        node->right->lexem.kind = VOID;
        node = node->right;
    }
    return top;
}

struct node_t* Comm(int* i) {
    struct node_t *node = NULL;
    struct node_t *comm_left = Expr(i);
    //printf("_%d_", *i);
    if (is_stop(*i))
        return comm_left;
    node = Create_Node();
    node->lexem = get_cur_lexem(*i, NULL);
    if (is_print(*i)){
        (*i)++;
       node->left = Expr(i);
       node->right = Create_Node();
       node->right->lexem.kind = VOID;
    }
    if (is_assign(*i)) {
        (*i)++;
       node->left = comm_left;
        //printf("_%d_", *i);
        node->right = Expr(i);
        //printf("_%d_", *i);
    }
    comm_left = node;
    return comm_left;
}

struct node_t* Expr(int* i)
{
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

        //printf("_%d_", get_cur_lexem(*i, NULL).kind);
        //printf("(%d)\n", *i);
        struct lexem_t lexem = get_cur_lexem(*i, NULL);
        if (lexem.kind != NUM && lexem.kind != BRACE && lexem.kind != VARIABLE)
        {
            //printf("{%d}", *i);
            fprintf(stderr, "Expected expression, line - %d", __LINE__);
            exit(1);
        }
        node->left = expr_left;
        node->right = Mult(i);

        expr_left = node;
    }
    return expr_left;
}


struct node_t* Mult(int* i)
{
    struct node_t* node = NULL;
    struct node_t* mult_left = Term(i);
/*    if (*i >= get_cur_size(NULL) - 1)
        return mult_left;*/
/*    if (is_stop(*i))
        return mult_left;*/

    while (is_mul_div(*i) && get_cur_lexem(*i, NULL).kind == OP) {

/*        if (is_stop(*i))
            return mult_left;*/

        node = Create_Node();
        node->lexem = get_cur_lexem(*i, NULL);

        //printf("{%d}\n", *i);

        (*i)++;

        struct lexem_t lexem = get_cur_lexem(*i, NULL);
        if (lexem.kind != NUM && lexem.kind != BRACE && lexem.kind != VARIABLE)
        {
            fprintf(stderr,"Expected expression, line - %d", __LINE__);
            exit(1);
        }
        node->left = mult_left;
        node->right = Term(i);

        mult_left = node;
    }
    return mult_left;
}

struct node_t* Term (int* i)
{
    struct node_t* node = NULL;
    if (is_num_input_variable(*i))
    {
        node = Create_Node();
        node->lexem = get_cur_lexem(*i, NULL);
        if (get_cur_lexem(*i + 1, NULL).kind == NUM)
        {
            fprintf(stderr, "Two numbers in a row, line - %d", __LINE__);
            exit(1);
        }
        (*i)++;
        return node;
    }
    if(is_brace(*i) == LBRAC)
    {
        //fprintf(stderr, "{%d}", *i);
        (*i)++;
        if (is_brace(*i) == RBRAC)
        {
            fprintf(stderr, "Extra brace, line - %d", __LINE__);
            exit(1);
        }
        node = Expr(i);
        if (is_brace(*i) != RBRAC)
        {
            fprintf(stderr, "Extra brace, line - %d", __LINE__);
            exit(1);
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
                default:
                    exit(1);
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
            }
            break;
        case SENTENSE:
            printf ("SENTENSE #%d ", lex.lex.num);
            break;
        case VOID:
            printf ("VOID");
            break;
        default:
            exit(2);
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
