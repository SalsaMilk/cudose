#include <math.h>

float evaluate(Node* root, float x) {
    if (root == NULL)
        return 0.0f;

    if (root->token.type == TOKEN_CONSTANT)
        return strtof(root->token.value, NULL);

    if (root->token.type == TOKEN_VARIABLE)
        return x;

    if (root->token.type == TOKEN_OPERATOR) {
        switch (root->token.value[0]) {
            case '+':
                return evaluate(root->left, x) + evaluate(root->right, x);
            case '-':
                return evaluate(root->left, x) - evaluate(root->right, x);
            case '*':
                return evaluate(root->left, x) * evaluate(root->right, x);
            case '/':
                return evaluate(root->left, x) / evaluate(root->right, x);
            case '^':
                return powf(evaluate(root->left, x), evaluate(root->right, x));
            default:
                return 0.0f;
        }
    }

    return 0.0f;
}