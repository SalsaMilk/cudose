#ifndef CUDOSE_LEXER_H
#define CUDOSE_LEXER_H

#define IS_NUMBER(x) x >= '0' && x <= '9'

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

void handle_number(const char** exp, Token* token) {
    char* constBuf = malloc(21);  // buffer size increased to accommodate decimals
    constBuf[20] = '\0';
    char* p = constBuf;
    int digits = 0;
    bool hasDecimal = false;

    while(IS_NUMBER(**exp) || **exp == '.') {
        if (**exp == '.') {
            if (hasDecimal) {
                fprintf(stderr, "Invalid number with multiple decimals!");
                exit(-1);
            }
            hasDecimal = true;
        } else if (digits > 20) {
            fprintf(stderr, "Number too big!");
            exit(-1);
        }

        *p++ = **exp;
        (*exp)++;
        digits++;
    }
    *p = '\0';
    token->type = TOKEN_CONSTANT;
    token->value = constBuf;
    (*exp)--;
}

void handle_token(const char** exp, Token* token, Byte type) {
    token->type = type;

    char* str = calloc(2, 1);
    str[0] = **exp;
    token->value = str;
}

int tokenize(const char* exp, Token* tokens) {
    Token* token = tokens;
    int count = 0;
    while(*exp) {
        if (IS_NUMBER(*exp)) {
            handle_number(&exp, token);
            if (*(exp + 1) == VARIABLE) {
                (++token)->type = TOKEN_OPERATOR;

                char *str = calloc(2, 1);
                str[0] = '*';
                token->value = str;
                count++;
            }
        }
        else if (*exp == VARIABLE) {
            handle_token(&exp, token, TOKEN_VARIABLE);
        }
        else if (*exp == '(' || *exp == ')') {
            handle_token(&exp, token, TOKEN_GROUPER);
        }
        else if (*exp == '+' || *exp == '-' || *exp == '*' || *exp == '/' || *exp == '^') {
            handle_token(&exp, token, TOKEN_OPERATOR);
        }
        else if (*exp == ' ') {
            exp++;
            continue;
        }
        else {
            fprintf(stderr, "Invalid glyph \"%c\"\n", *exp);
            exit(-1);
        }
        exp++;
        token++;
        count++;
    }
    return count;
}

#ifdef DEBUG
void debug_print_tokens(Token tokens[]) {
    printf("Tokenized expression:\n");
    for (int i = 0; i < tokenCount; i++) {
        printf("{Type: ");
        switch (tokens[i].type) {
            case TOKEN_CONSTANT:
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
        printf(",\tValue: '%s'}\n", tokens[i].value);
    }
}
#endif //DEBUG

#endif //CUDOSE_LEXER_H
