#include <raylib.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "buffer.h"
#include "encodings.h"

const int font_size_default = 40;
static int font_size = font_size_default;
static Font font;

static int mode = 1; // 0: normal, 1: input, 2: visual
static gap_t gap;
static gap_t* curbuf = NULL;

#define IKPR(X) ((IsKeyPressed(X) || IsKeyPressedRepeat(X)) && !IsKeyDown(KEY_LEFT_CONTROL) && !IsKeyDown(KEY_LEFT_SHIFT) && !IsKeyDown(KEY_LEFT_ALT)) // Is Key Pressed and Repeated
#define IKPC(X) (IsKeyPressed(X) && IsKeyDown(KEY_LEFT_CONTROL) && !IsKeyDown(KEY_LEFT_SHIFT) && !IsKeyDown(KEY_LEFT_ALT)) // Is Key Pressed with left Control
#define IKPSR(X) ((IsKeyPressed(X) || IsKeyPressedRepeat(X)) && !IsKeyDown(KEY_LEFT_CONTROL) && IsKeyDown(KEY_LEFT_SHIFT) && !IsKeyDown(KEY_LEFT_ALT)) // Is Key Pressed with left Shift and Repeated
#define IKPCR(X) ((IsKeyPressed(X) || IsKeyPressedRepeat(X)) && IsKeyDown(KEY_LEFT_CONTROL) && !IsKeyDown(KEY_LEFT_SHIFT) && !IsKeyDown(KEY_LEFT_ALT)) // Is Key Pressed with left Control and Repeated
#define IKPCS(X) (IsKeyPressed(X) && IsKeyDown(KEY_LEFT_CONTROL) && IsKeyDown(KEY_LEFT_SHIFT) && !IsKeyDown(KEY_LEFT_ALT)) // Is Key Pressed with left Control and left Shift
#define IKPA(X) (IsKeyPressed(X) && !IsKeyDown(KEY_LEFT_CONTROL) && !IsKeyDown(KEY_LEFT_SHIFT) && IsKeyDown(KEY_LEFT_ALT)) // Is Key Pressed with left Alt
#define IKPAR(X) ((IsKeyPressed(X) || IsKeyPressedRepeat(X)) && !IsKeyDown(KEY_LEFT_CONTROL) && !IsKeyDown(KEY_LEFT_SHIFT) && IsKeyDown(KEY_LEFT_ALT)) // Is Key Pressed with left Alt and Repeated
#define IKPM(X) (IsKeyPressed(X) && !IsKeyDown(KEY_LEFT_CONTROL) && !IsKeyDown(KEY_LEFT_SHIFT) && !IsKeyDown(KEY_LEFT_ALT) && IsKeyDown(KEY_KB_MENU)) // Is Key Pressed with Menu

void reload_font() {
    UnloadFont(font);
    printf("%d\n", sizeof(latin2_mod));
    font = LoadFontEx("JetBrainsMono-Regular.ttf", font_size, latin2_mod, sizeof(latin2_mod) / sizeof(latin2_mod[0]));
}

void filesave(gap_t* b) {
    FILE* file = fopen("file.txt", "wb");
    for (int i = 0; i < b->lbuf_len + b->rbuf_len; i++) {
        uint32_t c = 0;
        if (i < b->lbuf_len) {
            c = b->lbuf[i];
        } else {
            c = b->rbuf[b->rbuf_len - (i - b->lbuf_len) - 1];
        }
        char utf8[4];
        int n = utf8_encode(utf8, c);
        fwrite(utf8, sizeof(char), n, file);
    }
    fclose(file);
}

void draw_cursor(float x, float y) {
    if (mode == 1) {
        DrawRectangle(x, y, 2, font.baseSize, WHITE);
    } else {
        GlyphInfo gi = GetGlyphInfo(font, 'a');
        DrawRectangle(x, y, gi.advanceX, font.baseSize, WHITE);
    }
}

#define fontheight (font.baseSize + font.glyphPadding)
void draw_buffer(gap_t* b) {
    float x = 0;
    float y = 0;
    for (int i = 0; i < b->lbuf_len + b->rbuf_len + 1; i++) {
        if (i == b->lbuf_len) {
            draw_cursor(x, y);
        }
        if (i >= b->lbuf_len + b->rbuf_len) { break; }
        uint32_t c;
        if (i < b->lbuf_len) {
            c = b->lbuf[i];
        } else {
            c = b->rbuf[b->rbuf_len - (i - b->lbuf_len) - 1];
        }
        if (c == '\n') {
            y += fontheight;
            x = 0;
            continue;
        }
        if (c == '\t') {
            GlyphInfo gi = GetGlyphInfo(font, ' ');
            x += gi.advanceX * 4;
            continue;
        }
        DrawTextCodepoint(font, c, (Vector2){x,y}, font_size, WHITE);
        GlyphInfo gi = GetGlyphInfo(font, c);
        x += gi.advanceX;
    }

    while (y <= GetScreenHeight()) {
        y += fontheight;
        x = 0;
        DrawTextCodepoint(font, '~', (Vector2){x,y}, font_size, DARKGRAY);
    }
}

void normal(void) {
    if (IKPR(KEY_L)) {
        rbuf_prelij_lbuf(curbuf);
    }
    if (IKPR(KEY_H)) {
        lbuf_prelij_rbuf(curbuf);
    }
    if (IsKeyPressed(KEY_I)) {
        mode = 1;
    }
}

void input(void) {
    int c;
    while ((c = GetCharPressed())) {
        if ((c == ' ' && IsKeyDown(KEY_LEFT_SHIFT)) || IsKeyDown(KEY_KB_MENU)) { continue; }
        printf("d: %d, c: %c, %d\n", c, c, iskwc(c));
        append_lbuf(curbuf, c);
    }
    if (IKPR(KEY_ENTER)) {
        append_lbuf(curbuf, '\n');
    }
    if (IKPR(KEY_TAB)) {
        append_lbuf(curbuf, '\t');
    }
    if (IKPSR(KEY_SPACE)) {
        append_lbuf(curbuf, ' ');
        append_lbuf(curbuf, ' ');
    }
    if (IKPR(KEY_BACKSPACE)) {
        cursor_backspace(curbuf);
    }
    if (IKPC(KEY_EQUAL)) {
        font_size += 5;
        reload_font();
    }
    if (IKPC(KEY_MINUS)) {
        font_size -= 5;
        reload_font();
    }
    if (IKPCS(KEY_EQUAL)) {
        font_size = font_size_default;
        reload_font();
    }
    if (IKPC(KEY_S)) {
        filesave(curbuf);
    }
    if (IsKeyPressed(KEY_ESCAPE)) {
        /* mode = 0; */
    }
    if (IKPM(KEY_K)) {
        append_lbuf(curbuf, 0x2039);
    }
    if (IKPM(KEY_L)) {
        append_lbuf(curbuf, 0x203A);
    }
    if (IKPM(KEY_COMMA)) {
        append_lbuf(curbuf, 0xAB);
    }
    if (IKPM(KEY_PERIOD)) {
        append_lbuf(curbuf, 0xBB);
    }
    if (IKPCR(KEY_L)) {
        rbuf_prelij_lbuf(curbuf);
    }
    if (IKPCR(KEY_H)) {
        lbuf_prelij_rbuf(curbuf);
    }
    if (IKPAR(KEY_L)) {
        cursor_to_start_of_word(curbuf);
    }
    if (IKPAR(KEY_H)) {
        cursor_to_start_of_word_backwards(curbuf);
    }
    if (IKPAR(KEY_E)) {
        cursor_to_end_of_word(curbuf);
    }
    if (IKPC(KEY_E)) {
        cursor_to_end_of_line(curbuf);
    }
    if (IKPC(KEY_A)) {
        cursor_to_start_of_line(curbuf);
    }
}

void draw(void) {
    draw_buffer(curbuf);
}

int main(void) {
    gap = gap_create();
    curbuf = &gap;
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1600, 900, "aij.exe");
    SetExitKey(0);
    reload_font();

    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground((Color){ 20, 20, 30, 255 });
            switch(mode) {
                case 0: normal(); break;
                case 1: input(); break;
            }
            draw();
        EndDrawing();
    }

    UnloadFont(font);
    CloseWindow();
    return 0;
}
