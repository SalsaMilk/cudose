// Implementation of the Shunting Yard algorithm
// https://en.wikipedia.org/wiki/Shunting_yard_algorithm

#ifndef CUDOSE_SHUNTINGYARD_H
#define CUDOSE_SHUNTINGYARD_H

Byte precedence(char operator) {
    switch (operator) {
        case '^':
            return 4;
            break;
        case '*':
        case '/':
            return 3;
            break;
        case '+':
        case '-':
            return 2;
            break;
        default:
            fprintf(stderr, "Unknown error.");
            exit(-1);
            break;
    }
}

struct Stack {
    int top;
    unsigned capacity;
    Token* array;
};

struct Stack* createStack(unsigned capacity)
{
    struct Stack* stack = (struct Stack*)malloc(sizeof(struct Stack));
    if (stack == NULL) {
        fprintf(stderr, "Memory allocation failed");
        exit(-1);
    }

    stack->capacity = capacity;
    stack->top = -1;
    stack->array = (Token*)malloc(stack->capacity * sizeof(Token));
    if (stack->array == NULL) {
        fprintf(stderr, "Memory allocation failed");
        exit(-1);
    }

    return stack;
}

void deleteStack(struct Stack* stack) {
    free(stack->array);
    free(stack);
}

int isFull(struct Stack* stack)
{
    return stack->top == stack->capacity - 1;
}

int isEmpty(struct Stack* stack)
{
    return stack->top == -1;
}

void push(struct Stack* stack, Token item)
{
    if (isFull(stack)) {
        fprintf(stderr, "Stack overflow");
        exit(-1);
    }

    stack->array[++stack->top] = item;
}

Token pop(struct Stack* stack)
{
    if (isEmpty(stack)) {
        fprintf(stderr, "Stack underflow");
        exit(-1);
    }

    return stack->array[stack->top--];
}

Token peek(struct Stack* stack)
{
    if (isEmpty(stack)) {
        fprintf(stderr, "Can't peek at empty stack");
        exit(-1);
    }

    return stack->array[stack->top];
}

void shuntingYard(Token* t_exp, Token* out) {
    struct Stack* operatorStack = createStack(MAX_TOKENS);
    for (int i = 0; i < tokenCount; i++) {
        switch (t_exp[i].type) {
            case TOKEN_CONSTANT:
            case TOKEN_VARIABLE:
                out[nodeCount++] = t_exp[i];
                continue;
            case TOKEN_OPERATOR:
                if (!isEmpty(operatorStack)) {
                    while ((*peek(operatorStack).value != '(')
                           && ((precedence(*peek(operatorStack).value) > precedence(*t_exp[i].value)) ||
                               (((precedence(*peek(operatorStack).value) == precedence(*t_exp[i].value)) &&
                                 (*t_exp[i].value != '^'))))) {
                        out[nodeCount++] = pop(operatorStack);
                        if (isEmpty(operatorStack)) break;
                    }
                }
                push(operatorStack, t_exp[i]);
                continue;
            case TOKEN_GROUPER:
                if (*t_exp[i].value == '(') {
                    push(operatorStack, t_exp[i]);
                    continue;
                }
                while (*peek(operatorStack).value != '(') {
                    if (isEmpty(operatorStack)) {
                        fprintf(stderr, "Mismatched parenthesis");
                        exit(-1);
                    }
                    out[nodeCount++] = pop(operatorStack);
                }
                pop(operatorStack);
        }
    }
    while (!isEmpty(operatorStack)) {
        if (*peek(operatorStack).value == '(') {
            fprintf(stderr, "Mismatched parenthesis");
            exit(-1);
        }

        out[nodeCount++] = pop(operatorStack);
    }

    deleteStack(operatorStack);
}

#endif //CUDOSE_SHUNTINGYARD_H