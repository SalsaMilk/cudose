#define DEBUG
#define Byte unsigned char
#define MAX_TOKENS 30

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_gfxPrimitives.h>

int tokenCount, nodeCount = 0;

#include "lexer.h"
#include "shuntingYard.h"
#include "tree.h"
#include "evaluate.h"

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
#endif

int main() {
    const char* exp = "(x-13.76765)/(2x-9.5)^3*7+x";

#ifdef DEBUG
    printf("f(x) = %s\n\n", exp);
#endif

    Token tokens[MAX_TOKENS];
    tokenCount = tokenize(exp, tokens);

#ifdef DEBUG
    debug_print_tokens(tokens);
#endif

    Token shuntingYardOutput[MAX_TOKENS];
    shuntingYard(tokens, shuntingYardOutput);

    // Reverse the array
    Token transformedExpression[MAX_TOKENS];
    for (int i = 0; i < nodeCount; i++) {
        transformedExpression[i] = shuntingYardOutput[nodeCount - i - 1];
    }

#ifdef DEBUG
    printf("\nTokens in Łukasiewicz notation: ");
    for (int i = 0; i < nodeCount; i++)
        printf("%s ", transformedExpression[i].value);
    printf("\n\n");
#endif

    expPtr = transformedExpression;
    Node* abstractSyntaxTree = tree();

#ifdef DEBUG
    displayTree(abstractSyntaxTree, 0);

    printf("\nf(2) = %f", evaluate(abstractSyntaxTree, 2));
#endif

    for (int i = 0; i < tokenCount; i++)
        free(tokens[i].value);

    return 0;
}