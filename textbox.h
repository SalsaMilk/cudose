#ifndef CUDOSE_TEXTBOX_H
#define CUDOSE_TEXTBOX_H

typedef struct {
    SDL_Rect rect;
    SDL_Rect textRect;
    SDL_Texture* texture;
    TTF_Font* font;

    struct {
        Byte r, g, b, a;
    } color;

    char* text;
    char cursorPos;
} TextBox;

TextBox textBox_new(SDL_Renderer* renderer, int x, int y, char* text, TTF_Font* font) {
    SDL_Color white = {255, 255, 255, 255};
    TextBox textBox = {
            .rect = { .x = x, .y = y },
            .texture = SDL_CreateTextureFromSurface(renderer, TTF_RenderText_Solid(font, text, white)),
            .font = font,
            .color = { .r = 15, .g = 15, .b = 15, .a = 255 },
            .text = text,
            .cursorPos = -1
    };
    SDL_QueryTexture(textBox.texture, NULL, NULL, &textBox.textRect.w, &textBox.textRect.h);
    textBox.textRect.y = textBox.rect.y + (textBox.textRect.h / 2);
    textBox.textRect.x = textBox.rect.x + FONT_SIZE / 2;
    textBox.rect.w = (FONT_SIZE / 2 + 2) * (MAX_TOKENS + 1) + (FONT_SIZE / 2);
    textBox.rect.h = textBox.textRect.h * 2;
    return textBox;
}

Byte textBox_isInside(TextBox *textBox, int x, int y) {
    return x >= textBox->rect.x &&
        x <= (textBox->rect.x + textBox->rect.w) &&
        y >= textBox->rect.y &&
        y <= (textBox->rect.y + textBox->rect.h);
}

char textBox_whereIsMouse(TextBox *textBox) {
    SDL_Point cursorPos;
    SDL_GetMouseState(&cursorPos.x, &cursorPos.y);

    if (!textBox_isInside(textBox, cursorPos.x, cursorPos.y)) {
        return -1;
    }

    char pos = (char) (1.645 * (cursorPos.x - textBox->textRect.x - 1) / FONT_SIZE + 0.5);

    if (pos < 0)
        return 0;
    else {
        char len = (char) strlen(textBox->text);
        if (pos > len)
            return len;
    }

    return pos;
}

void textBox_insert(TextBox *textBox, char character) {
    uint length = strlen(textBox->text);

    if (length >= MAX_TOKENS) {
        return;
    }

    if (textBox->cursorPos == -1 || textBox->cursorPos == (char) length) {
        textBox->cursorPos = (char) length;

        textBox->text[length] = character;
        textBox->text[length + 1] = '\0';
        textBox->cursorPos++;

        return;
    }

    memcpy(textBox->text + textBox->cursorPos + 1, textBox->text + textBox->cursorPos, length - textBox->cursorPos + 1);
    textBox->text[textBox->cursorPos] = character;

    textBox->cursorPos++;
}

void textBox_backspace(TextBox *textBox) {
    uint length = strlen(textBox->text);

    if (length < 1) {
        return;
    }

    if (textBox->cursorPos == -1) {
        textBox->cursorPos = (char)length;
    }

    memcpy(textBox->text + textBox->cursorPos - 1, textBox->text + textBox->cursorPos, length - textBox->cursorPos + 1);

    textBox->cursorPos--;
}

void textBox_update(SDL_Renderer *renderer, TextBox *textBox) {
    SDL_Color white = {255, 255, 255, 255};
    textBox->texture = SDL_CreateTextureFromSurface(renderer,
                                                    TTF_RenderText_Solid(textBox->font, textBox->text, white));
    SDL_QueryTexture(textBox->texture, NULL, NULL, &textBox->textRect.w, &textBox->textRect.h);
}

void textBox_draw(SDL_Renderer *renderer, TextBox *textBox) {
    SDL_SetRenderDrawColor(renderer, textBox->color.r, textBox->color.g, textBox->color.b, textBox->color.a);
    SDL_RenderFillRect(renderer, &textBox->rect);

    SDL_SetRenderDrawColor(renderer, 190, 190, 190, 50);
    SDL_RenderDrawRect(renderer, &textBox->rect);
    SDL_RenderCopy(renderer, textBox->texture, NULL, &textBox->textRect);

    if (textBox->cursorPos == -1)
        return;

    if (floorf(sinf(0.01f * (float) SDL_GetTicks()))) {
        Sint16 cursorOffset = (Sint16) (((float) textBox->cursorPos * FONT_SIZE / 1.645f) + 1);
        vlineRGBA(renderer, (Sint16) (textBox->textRect.x + cursorOffset),
                            (Sint16) textBox->textRect.y, (Sint16) (textBox->textRect.y + FONT_SIZE),
                            255, 190, 190, 255);
    }
}

#endif //CUDOSE_TEXTBOX_H
