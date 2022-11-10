/*
 * I'm itching to write some code, so I guess my goal
 * is to have this parse mathematical expressions
 * and perform calculus and such blah blah
 */

#include <stdio.h>
#include <stdlib.h>
//#include <math.h>
//#include <string.h>

#define isNum(x) x >= '0' && x <= '9'

#define Byte unsigned char

#define VARIABLE 'x'

#define TOKEN_NUMBER    0
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
            Token t = { .type = TOKEN_NUMBER, .value = constBuf};
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

int main() {
    // f(x) / exp / function
    const char* exp = "x^2 + 3*(x+12) - 95\0";

    Token tokens[30];
    int tokenCount = tokenize(exp, tokens);

    for (int i = 0; i < tokenCount; i++) {
        printf("{Type: ");
        switch (tokens[i].type) {
            case TOKEN_NUMBER:
                printf("'number'");
                break;
            case TOKEN_VARIABLE:
                printf("'variable'");
                break;
            case TOKEN_OPERATOR:
                printf("'operator'");
                break;
            case TOKEN_GROUPER:
                printf("'grouper'");
                break;
            case TOKEN_FUNCTION:
                printf("'function'");
                break;
            default:
                break;
        }
        printf(", Value: '%s'}\n", tokens[i].value);
    }

    for (int i = 0; i < tokenCount; ++i)
        free(tokens[i].value);

    return 0;
}