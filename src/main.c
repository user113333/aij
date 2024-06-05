#include <raylib.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "editor.h"

static int mode = 1;
const int fontsize_default = 40;
static Font font;

void reload_font() {
    UnloadFont(font);
    font = LoadFontEx("JetBrainsMono-Regular.ttf", fontsize_default, NULL, 0);
}

// TODO
/* void filesave(buffer_t* b) { */
/*     FILE* file = fopen("file.txt", "wb"); */
/*     for (int i = 0; i < b->lbuf_len + b->rbuf_len; i++) { */
/*         uint32_t c = 0; */
/*         if (i < b->lbuf_len) { */
/*             c = b->lbuf[i]; */
/*         } else { */
/*             c = b->rbuf[b->rbuf_len - (i - b->lbuf_len) - 1]; */
/*         } */
/*         char utf8[4]; */
/*         int n = unicode_to_utf8(utf8, c); */
/*         fwrite(utf8, sizeof(char), n, file); */
/*     } */
/*     fclose(file); */
/* } */

// TODO
/* void draw_cursor(float x, float y) { */
/*     if (mode == 1) { */
/*         DrawRectangle(x, y, 2, font.baseSize, WHITE); */
/*     } else { */
/*         GlyphInfo gi = GetGlyphInfo(font, 'a'); */
/*         DrawRectangle(x, y, gi.advanceX, font.baseSize, WHITE); */
/*     } */
/* } */

// TODO
/* #define fontheight (font.baseSize + font.glyphPadding) */
/* void draw_buffer(buffer_t* b) { */
/*     float x = 0; */
/*     float y = 0; */
/*     for (int i = 0; i < b->lbuf_len + b->rbuf_len + 1; i++) { */
/*         if (i == b->lbuf_len) { */
/*             draw_cursor(x, y); */
/*         } */
/*         if (i >= b->lbuf_len + b->rbuf_len) { break; } */
/*         uint32_t c; */
/*         if (i < b->lbuf_len) { */
/*             c = b->lbuf[i]; */
/*         } else { */
/*             c = b->rbuf[b->rbuf_len - (i - b->lbuf_len) - 1]; */
/*         } */
/*         if (c == '\n') { */
/*             y += fontheight; */
/*             x = 0; */
/*             continue; */
/*         } */
/*         if (c == '\t') { */
/*             GlyphInfo gi = GetGlyphInfo(font, ' '); */
/*             x += gi.advanceX * 4; */
/*             continue; */
/*         } */
/*         DrawTextCodepoint(font, c, (Vector2){x,y}, font_size, WHITE); */
/*         GlyphInfo gi = GetGlyphInfo(font, c); */
/*         x += gi.advanceX; */
/*     } */
/*     while (y <= GetScreenHeight()) { */
/*         y += fontheight; */
/*         x = 0; */
/*         DrawTextCodepoint(font, '~', (Vector2){x,y}, font_size, DARKGRAY); */
/*     } */
/* } */

float draw_codepoint(uint32_t unicode, float x, int line_number) {
    int font_height = font.baseSize;
    GlyphInfo gi = GetGlyphInfo(font, unicode);
    DrawTextCodepoint(
        font,
        unicode,
        (Vector2){ x, line_number*font_height },
        fontsize_default,
        WHITE
    );
    return gi.advanceX;
}

void input(void) {
    int c;
    while ((c = GetCharPressed())) {
        if (c == ' ' && IsKeyDown(KEY_LEFT_SHIFT)) { continue; }
        printf("c: '%c', %d\n", c, c);
        editor_handle_charinput(c);
    }
    int k;
    while ((k = GetKeyPressed())) {
        if (k == KEY_LEFT_CONTROL) { continue; }
        if (k == KEY_LEFT_SHIFT) { continue; }
        if (k == KEY_LEFT_ALT) { continue; }
        printf("k: %d\n", k);
        editor_handle_keyinput(
            k,
            IsKeyDown(KEY_LEFT_CONTROL),
            IsKeyDown(KEY_LEFT_SHIFT),
            IsKeyDown(KEY_LEFT_ALT)
        );
    }
}

void normal(void) {
    // vim normal mode
}

void draw(void) {
    editor_draw(0, 0, draw_codepoint);
}

int main(void) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1600, 900, "aij.exe");
    SetExitKey(0);
    reload_font();
    editor_init();

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
