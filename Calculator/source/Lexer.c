#include "HashTable.h"
#include "Lexer.h"

static void swap_lexems(struct lex_array_t* larr, int index1, int index2)
{
    struct lexem_t copy = larr->lexems[index1];
    larr->lexems[index1] = larr->lexems[index2];
    larr->lexems[index2] = copy;
}

bool iscomparsigns(const char* str, int i) {
    if (str[i] == '!' || str[i] == '>' || str[i] == '<')
        return true;
    if (str[i] == '=' && str[i + 1] == '=')
        return true;
    return false;
}

struct lex_array_t lex_string(const char *str) {
    assert(str != NULL);

    struct lex_array_t larr = { (struct lexem_t*)calloc(ICAP , sizeof(struct lexem_t)),
                                0,
                                ICAP };
    if (larr.lexems == NULL)
        exit(2);
    assert(larr.lexems != NULL);

    int i = 0;
    struct variable_t* hash_table = create_hash_table();
    variable_value(0, 0, false, hash_table);

    while (str[i])
    {
        if (larr.size == larr.capacity ) {
            larr.lexems = (struct lexem_t *)realloc(larr.lexems, sizeof(struct lexem_t) * (larr.capacity + 10));
            assert(larr.lexems);
            larr.capacity += 10;
        }

        //Read Space

        if (isspace(str[i]))
        {
            i++;
            continue;
        }

        //Read Numbers

        if (isdigit(str[i]))
        {
            larr.lexems[larr.size].kind = NUM;
            larr.lexems[larr.size].lex.num = 0;
            while(isdigit(str[i]))
            {
                larr.lexems[larr.size].lex.num = larr.lexems[larr.size].lex.num * 10 + str[i] - '0';
                i++;
            }
            larr.size++;
            continue;
        }

        //Read Operation

        if (str[i] == '+' || str[i] == '-' || str[i] == '*' || str[i] == '/')
        {
            larr.lexems[larr.size].kind = OP;
            int code = str[i];
            switch ( code )
            {
                case '+':
                    larr.lexems[larr.size].lex.op = ADD;
                    break;
                case '-':
                    larr.lexems[larr.size].lex.op = SUB;
                    break;
                case '*':
                    larr.lexems[larr.size].lex.op = MUL;
                    break;
                case '/':
                    larr.lexems[larr.size].lex.op = DIV;
                default:;
            }
            larr.size++;
            i++;
            continue;
        }

        if (iscomparsigns(str, i))
        {
            larr.lexems[larr.size].kind = COMPAR_SIGNS;
            switch (str[i])
            {
                case '>':
                    if (str[i + 1] == '=') {
                        larr.lexems[larr.size].lex.cs = EQ_OR_GR;
                        i++;
                        break;
                    }
                    larr.lexems[larr.size].lex.cs = GREATER;
                    break;
                case '<':
                    if (str[i + 1] == '=') {
                        larr.lexems[larr.size].lex.cs = EQ_OR_LESS;
                        i++;
                        break;
                    }
                    larr.lexems[larr.size].lex.cs = LESS;
                    break;
                case '=':
                    if (str[i + 1] == '=') {
                        larr.lexems[larr.size].lex.cs = EQUAL;
                        i++;
                    }
                    break;
                case '!':
                    if (str[i + 1] == '=') {
                        larr.lexems[larr.size].lex.cs = NOT_EQUAL;
                        i++;
                        break;
                    }
                    larr.lexems[larr.size].lex.cs = NOT;
                    break;
                default: assert(0 && "READ ERROR");
            }
            larr.size++;
            i++;
            continue;
        }

        //Read Brace

        if (str[i] == '(' || str[i] == ')')
        {
            larr.lexems[larr.size].kind = BRACE;
            int code = str[i];
            switch (code)
            {
                case '(':
                    larr.lexems[larr.size].lex.b = LBRAC;
                    break;
                case ')':
                    larr.lexems[larr.size].lex.b = RBRAC;
                    break;
                default:;
            }
            larr.size++;
            i++;
            continue;
        }

        if (str[i] == '{' || str[i] == '}')
        {
            larr.lexems[larr.size].kind = BRACE;
            switch (str[i])
            {
                case '{':
                    larr.lexems[larr.size].lex.b = LFIGURBRAC;
                    break;
                case '}':
                    larr.lexems[larr.size].lex.b = RFIGURBRAC;
                    break;
                default:;
            }
            larr.size++;
            i++;
            continue;
        }

        //Read Variables and Command

        if (isalpha(str[i]))
        {
            int numsymbol = 0;
            int hash = 0;
            larr.lexems[larr.size].kind = VARIABLE;
            char* word = (char*)calloc(MAXLENWORD + 1, sizeof(char));
            while (isalpha(str[i]) || isdigit(str[i]))
            {
                hash = calc_hash(hash, numsymbol, str[i]);
                if (numsymbol == MAXLENWORD - 1)
                {
                    printf("Easy, too many letters in your variable");
                    exit(ERROR);
                }
                word[numsymbol] = str[i];
                numsymbol++;
                i++;
            }

            if (!strcmp(word, "print"))
            {
                larr.lexems[larr.size].kind = COMMAND;
                larr.lexems[larr.size].lex.com = PRINT;
                larr.size++;
                continue;
            }

            if (!strcmp(word, "if"))
            {
                larr.lexems[larr.size].kind = COMMAND;
                larr.lexems[larr.size].lex.com = IF;
                larr.size++;
                if (larr.size)
                continue;
            }

            if (!strcmp(word, "while"))
            {
                larr.lexems[larr.size].kind = COMMAND;
                larr.lexems[larr.size].lex.com = WHILE;
                larr.size++;
                continue;
            }

            larr.lexems[larr.size].lex.num = check_hash_table(hash_table, hash, word);
            larr.size++;
            continue;
        }

        //Read Special Characters
        assert(i - 1 > 0);
        if (str[i] == '?')
        {
            larr.lexems[larr.size].kind = COMMAND;
            larr.lexems[larr.size].lex.com = INPUT;
            larr.size++;
            i ++;
            continue;
        }

        if (str[i] == '=')
        {
            larr.lexems[larr.size].kind = COMMAND;
            larr.lexems[larr.size].lex.com = ASSIGN;
            larr.size++;
            i++;
            continue;
        }

        if (str[i] == ';')
        {
            larr.lexems[larr.size].kind = COMMAND;
            larr.lexems[larr.size].lex.com = END_COMMAND;
            larr.size++;
            i++;
            continue;
        }
    }
    larr.lexems = (struct lexem_t *)realloc(larr.lexems, sizeof(struct lexem_t) * larr.size );
    //free_hash_table(hash_table);
    return larr;
}

void
print_op(enum operation_t opcode) {
    switch(opcode) {
        case ADD: printf(" PLUS"); break;
        case SUB: printf(" MINUS"); break;
        case MUL: printf(" MUL"); break;
        case DIV: printf(" DIV"); break;
        default: assert(0 && "unknown opcode");
    }
}
void
print_brace(enum braces_t bracetype){
    switch(bracetype) {
        case LBRAC: printf(" LBRAC"); break;
        case RBRAC: printf(" RBRAC"); break;
        case RFIGURBRAC: printf (" RFIGURBRAC"); break;
        case LFIGURBRAC: printf (" LFIGURBRAC"); break;
        default: assert(0 && "unknown bracket");
    }
}


void
print_num(int n) {
    printf(" NUMBER:%d", n);
}

void
print_variable(int n){
    printf(" VARIABLE:%d", n);
}

void
print_command(enum command_t command){
    switch(command){
        case IF: printf(" IF"); break;
        case WHILE: printf(" WHILE"); break;
        case INPUT: printf(" INPUT"); break;
        case PRINT: printf(" PRINT"); break;
        case ASSIGN: printf(" ASSIGN"); break;
        case END_COMMAND: printf(" END_COMMAND\n"); break;
        default: assert(0 && "unknown command");
    }
}

void
print_compar_signs(enum compar_signs_t cs){
    switch (cs) {
        case NOT: printf(" NOT"); break;
        case EQUAL: printf(" EQUAL"); break;
        case NOT_EQUAL: printf(" NOT_EQUAL"); break;
        case GREATER: printf (" GREATER"); break;
        case LESS: printf (" LESS"); break;
        case EQ_OR_LESS: printf (" EQ_OR_LESS"); break;
        case EQ_OR_GR: printf (" EQ_OR_GR"); break;
        default: assert(0 && "unknown compar signs");
    }
}

void
print_lexem(struct lexem_t lxm) {
    switch(lxm.kind) {
        case OP: print_op(lxm.lex.op); break;
        case BRACE: print_brace(lxm.lex.b); break;
        case NUM: print_num(lxm.lex.num); break;
        case VARIABLE: print_variable(lxm.lex.num); break;
        case COMMAND: print_command(lxm.lex.com); break;
        case COMPAR_SIGNS: print_compar_signs(lxm.lex.cs); break;
        default: assert(0 && "unknown lexem");
    }
}

void dump_lexarray(struct lex_array_t pl) {
    int i;
    assert(pl.lexems != NULL);
    for (i = 0; i < pl.size; ++i)
        print_lexem(pl.lexems[i]);
}

