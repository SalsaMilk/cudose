#define DEBUG
#define Byte unsigned char
#define MAX_TOKENS 30

#include <stdio.h>
#include <stdlib.h>

int tokenCount, nodeCount = 0;

#include "lexer.h"
#include "shuntingYard.h"
#include "tree.h"
#include "evaluate.h"
#include "gui.h"

Uint8 pressingShift() {
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    return keys[SDL_SCANCODE_LSHIFT] | keys[SDL_SCANCODE_RSHIFT];
}

Node* updateExpression(char *exp) {
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
    for (int i = 0; i < nodeCount; i++) {
        printf("%s ", transformedExpression[i].value);
    }

    printf("\n\n");
#endif

    expPtr = transformedExpression;
    Node *abstractSyntaxTree = tree();

#ifdef DEBUG
    displayTree(abstractSyntaxTree, 0);
#endif

    return abstractSyntaxTree;
}

int main(int argc, char *argv[]) {
    char expression[] = "(x-13.76765)/(2x-9.5)^3*7+x\0\0\0";

    Node* abstractSyntaxTree = updateExpression(expression);

#ifdef DEBUG
    printf("\nf(2)\t= %f", evaluate(abstractSyntaxTree, 2));
    printf("\nf(5)\t= %f", evaluate(abstractSyntaxTree, 5));
    printf("\nf(4.75)\t= %f\n", evaluate(abstractSyntaxTree, 4.75f));
#endif

    SDL_Window *window;
    SDL_Renderer *renderer;

    if (SDL_Init(SDL_INIT_EVERYTHING)) {
        fprintf(stderr, "Error initializing SDL");
        exit(-1);
    }

    window = SDL_CreateWindow("cudose",
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              WINDOW_WIDTH, WINDOW_HEIGHT, 0);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    if (TTF_Init()) {
        fprintf(stderr, "Error initializing SDL_ttf");
        exit(-1);
    }

    TTF_Font *font = TTF_OpenFont("../VCR_OSD_MONO.ttf", FONT_SIZE);
    if (font == NULL) {
        fprintf(stderr, "Error opening font");
        exit(-1);
    }

    TextBox textBox = textBox_new(renderer, FONT_SIZE, WINDOW_HEIGHT - FONT_SIZE * 3, expression, font);

    for (;;) {
        SDL_Event event;
        SDL_PollEvent(&event);

        if (event.type == SDL_QUIT) {
            break;
        }
        else if (event.type == SDL_MOUSEBUTTONDOWN) {
            textBox.cursorPos = textBox_whereIsMouse(&textBox);
        } SDL_SYSTEM_CURSOR_IBEAM
        else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.scancode) {
                case SDL_SCANCODE_0:
                case SDL_SCANCODE_8:
                case SDL_SCANCODE_9:
                    if (pressingShift()) {
                        textBox_insert(&textBox, (char) ('(' + (event.key.keysym.scancode - 35) % 3));
                        goto UPDATE;
                    }

                    goto INSERT_NUMBER;
                case SDL_SCANCODE_1:
                case SDL_SCANCODE_2:
                case SDL_SCANCODE_3:
                case SDL_SCANCODE_4:
                case SDL_SCANCODE_5:
                case SDL_SCANCODE_6:
                    if (pressingShift()) {
                        textBox_insert(&textBox, '^');
                        goto UPDATE;
                    }
                case SDL_SCANCODE_7:
                INSERT_NUMBER:
                    textBox_insert(&textBox, (char) ('0' + (event.key.keysym.scancode - 29) % 10));
                    goto UPDATE;
                case SDL_SCANCODE_KP_0:
                case SDL_SCANCODE_KP_1:
                case SDL_SCANCODE_KP_2:
                case SDL_SCANCODE_KP_3:
                case SDL_SCANCODE_KP_4:
                case SDL_SCANCODE_KP_5:
                case SDL_SCANCODE_KP_6:
                case SDL_SCANCODE_KP_7:
                case SDL_SCANCODE_KP_8:
                case SDL_SCANCODE_KP_9:
                    textBox_insert(&textBox, (char) ('0' + (event.key.keysym.scancode - 88) % 10));
                    goto UPDATE;
                case SDL_SCANCODE_X:
                    textBox_insert(&textBox, 'x');
                    goto UPDATE;
                case SDL_SCANCODE_PERIOD:
                case SDL_SCANCODE_KP_PERIOD:
                    textBox_insert(&textBox, '.');
                    goto UPDATE;
                case SDL_SCANCODE_EQUALS:
                    if (!pressingShift())
                        break;
                case SDL_SCANCODE_KP_PLUS:
                    textBox_insert(&textBox, '+');
                    goto UPDATE;
                case SDL_SCANCODE_KP_MULTIPLY:
                    textBox_insert(&textBox, '*');
                    goto UPDATE;
                case SDL_SCANCODE_SLASH:
                case SDL_SCANCODE_KP_DIVIDE:
                    textBox_insert(&textBox, '/');
                    goto UPDATE;
                case SDL_SCANCODE_MINUS:
                case SDL_SCANCODE_KP_MINUS:
                    textBox_insert(&textBox, '-');
                    goto UPDATE;
                case SDL_SCANCODE_BACKSPACE:
                    textBox_backspace(&textBox);
                    goto UPDATE;
                case SDL_SCANCODE_LEFT:
                    if (textBox.cursorPos == -1) {
                        char length = (char) strlen(expression);
                        textBox.cursorPos = (char) (length - (length > 0));
                        break;
                    }

                    if (textBox.cursorPos > 0) {
                        textBox.cursorPos--;
                    }

                    break;
                case SDL_SCANCODE_RIGHT:
                    if (textBox.cursorPos == -1) {
                        textBox.cursorPos = (char) strlen(expression);
                        break;
                    }

                    if (textBox.cursorPos < strlen(expression)) {
                        textBox.cursorPos++;
                    }

                    break;
                default:
                    break;
                UPDATE:
                    textBox_update(renderer, &textBox);
                    break;
            }
        }

        SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
        SDL_RenderClear(renderer);

        textBox_draw(renderer, &textBox);

        SDL_RenderPresent(renderer);

        SDL_Delay(10);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    TTF_Quit();
    SDL_Quit();
}
