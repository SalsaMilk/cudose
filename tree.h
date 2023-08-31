#ifndef CUDOSE_TREE_H
#define CUDOSE_TREE_H

typedef struct Node Node;

struct Node {
    Token token;
    Node *left, *right;
};

Token* expPtr;
Node* tree() {
    Node* node = malloc(sizeof(Node));
    node->token = *expPtr;
    switch (expPtr++->type) {
        case TOKEN_VARIABLE:
        case TOKEN_CONSTANT:
            node->right = NULL;
            node->left = NULL;
            return node;
        case TOKEN_OPERATOR:
            node->right = tree();
            node->left = tree();
            return node;
        default:
            fprintf(stderr, "Unknown error");
            exit(-1);
    }
}

#ifdef DEBUG
void displayTree(Node* rootNode, Byte depth) {
    for (int i = 0; i < depth; i++)
        printf("\t");

    printf("%s\n", rootNode->token.value);

    if (rootNode->token.type == TOKEN_OPERATOR) {
        displayTree(rootNode->right, depth + 1);
        displayTree(rootNode->left, depth + 1);
    }
}
#endif

#endif //CUDOSE_TREE_H