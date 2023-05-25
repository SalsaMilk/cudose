#ifndef CUDOSE_LEXER_H
#define CUDOSE_LEXER_H

#define isNum(x) x >= '0' && x <= '9'

#define Byte unsigned char

#define VARIABLE 'x'

#define TOKEN_CONSTANT  0
#define TOKEN_VARIABLE  1
#define TOKEN_OPERATOR  2
#define TOKEN_GROUPER   3
#define TOKEN_FUNCTION  4

typedef struct {
    Byte type;
    char* value;
} Token;

int tokenize(const char* exp, Token* tokens) {
    Token* token = tokens;
    int count = 0;
    while(*exp) {
        if (isNum(*exp)) {
            // start of a constant (10 digit limit)
            char* constBuf = malloc(11);
            constBuf[10] = '\0';
            char* p = constBuf;
            while(isNum(*exp)) {
                if (!*p) {
                    fprintf(stderr, "Number too big!");
                    exit(-1);
                }
                *p++ = *exp++;
            }
            *p = '\0';
            Token t = { .type = TOKEN_CONSTANT, .value = constBuf};
            *token = t;
            exp--;
        }
        else if (*exp == VARIABLE) {
            char* str = calloc(2, 1);
            str[0] = VARIABLE;
            Token t = { .type = TOKEN_VARIABLE, .value = str};
            *token = t;
        }
        else if (*exp == '(' || *exp == ')') {
            char* str = calloc(2, 1);
            str[0] = *exp;
            Token t = { .type = TOKEN_GROUPER, .value = str};
            *token = t;
        }
        else if (*exp == '+' || *exp == '-' || *exp == '*' || *exp == '/' || *exp == '^') {
            char* str = calloc(2, 1);
            str[0] = *exp;
            Token t = {.type = TOKEN_OPERATOR, .value = str};
            *token = t;
        }
        else if (*exp == ' ') {
            exp++;
            continue;
        }
        else {
            fprintf(stderr, "Invalid glyph \"%c\"", *exp);
            exit(-1);
        }
        exp++;
        token++;
        count++;
    }
    return count;
}

#endif //CUDOSE_LEXER_H
