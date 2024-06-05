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

float draw_char(uint32_t unicode, float x, float y) {
    GlyphInfo gi = GetGlyphInfo(font, unicode);
    DrawTextCodepoint(
        font,
        unicode,
        (Vector2){ x, y },
        fontsize_default,
        WHITE
    );
    return gi.advanceX;
}

void draw_cursor(float x, float y) {
    DrawRectangle(x, y, 3, font.baseSize, WHITE);
}

int normal(void) { }

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

void draw(void) {
    int rw = GetRenderWidth();
    int rh = GetRenderWidth();
    editor_draw((rendererinfo_t){rw, rh}, (fontinfo_t){0, font.baseSize, 0, 0},  draw_char, draw_cursor);
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
