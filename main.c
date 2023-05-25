/*
 * Experimenting with parsing expressions and abstract syntax trees
 */

#define DEBUG

#include <stdio.h>
#include <stdlib.h>

int tokenCount, nodeCount = 0;

#include "lexer.h"
#include "shuntingYard.h"
#include "tree.h"

int main() {
    const char* exp = "(x+2)*(x-9)^67*3+x";
    printf("Expression: %s", exp);

    Token tokens[30];
    tokenCount = tokenize(exp, tokens);

/*
#ifdef DEBUG
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
#endif
 */

    Token shuntingYardOutput[30];
    shuntingYard(tokens, shuntingYardOutput);

    // Reverse the array
    Token transformedExpression[30];
    for (int i = 0; i < nodeCount; i++) {
        transformedExpression[i] = shuntingYardOutput[nodeCount - i - 1];
    }

#ifdef DEBUG
    printf("\nTransformed expression: ");
    for (int i = 0; i < nodeCount; i++)
        printf("%s ", transformedExpression[i].value);
    printf("\n\n");
#endif

    expPtr = transformedExpression;
    Node* abstractSyntaxTree = tree();

#ifdef DEBUG
    displayTree(abstractSyntaxTree, 0);
#endif

    for (int i = 0; i < tokenCount; i++)
        free(tokens[i].value);

    return 0;
}