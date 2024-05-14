#include <raylib.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "latin2_unicode_table.h"

const int font_size_default = 40;

static int font_size = font_size_default;
static int mode = 1; // 0: normal, 1: input, 2: visual
static Font inter;
static int lbuf_len = 0, rbuf_len = 0;
static unsigned char lbuf[10000] = { 0 }; // latin2
static unsigned char rbuf[10000] = { 0 }; // latin2

#define APPEND_LBUF(X) lbuf[lbuf_len++] = (X);
#define APPEND_RBUF(X) rbuf[rbuf_len++] = (X);
#define LBUF_PRELIJ_RBUF if (lbuf_len > 0) { APPEND_RBUF(lbuf[--lbuf_len]) } // Put last uchar from lbuf to rbuf
#define RBUF_PRELIJ_LBUF if (rbuf_len > 0) { APPEND_LBUF(rbuf[--rbuf_len]) } // Put last uchar from rbuf to lbuf
#define IKPR(X) ((IsKeyPressed(X) || IsKeyPressedRepeat(X)) && !IsKeyDown(KEY_LEFT_CONTROL) && !IsKeyDown(KEY_LEFT_SHIFT) && !IsKeyDown(KEY_LEFT_ALT)) // Is Key Pressed and Repeated
#define IKPC(X) (IsKeyPressed(X) && IsKeyDown(KEY_LEFT_CONTROL) && !IsKeyDown(KEY_LEFT_SHIFT) && !IsKeyDown(KEY_LEFT_ALT)) // Is Key Pressed with left Control
#define IKPSR(X) ((IsKeyPressed(X) || IsKeyPressedRepeat(X)) && !IsKeyDown(KEY_LEFT_CONTROL) && IsKeyDown(KEY_LEFT_SHIFT) && !IsKeyDown(KEY_LEFT_ALT)) // Is Key Pressed with left Shift and Repeated
#define IKPCR(X) ((IsKeyPressed(X) || IsKeyPressedRepeat(X)) && IsKeyDown(KEY_LEFT_CONTROL) && !IsKeyDown(KEY_LEFT_SHIFT) && !IsKeyDown(KEY_LEFT_ALT)) // Is Key Pressed with left Control and Repeated
#define IKPCS(X) (IsKeyPressed(X) && IsKeyDown(KEY_LEFT_CONTROL) && IsKeyDown(KEY_LEFT_SHIFT) && !IsKeyDown(KEY_LEFT_ALT)) // Is Key Pressed with left Control and left Shift

unsigned char unicode_to_latin2(unsigned int);
unsigned int latin2_to_unicode(unsigned char);

bool ispunc(char c) {
    return (c == '.' || c == ',' || c == ';' || c == ' ' || c == ':');
}

void reload_font() {
    UnloadFont(inter);
    inter = LoadFontEx("JetBrainsMono-Regular.ttf", font_size, latin2_unicode_table, 256);
}

void filesave() {
    FILE* file;
    file = fopen("file.txt", "wb");
    fwrite(lbuf, sizeof(unsigned char), lbuf_len, file);
    fwrite(rbuf, sizeof(unsigned char), rbuf_len, file);
    fclose(file);
}

void draw_cursor(float x, float y, unsigned char next_l2) {
    if (!isgraph(next_l2)) {
        next_l2 = 'a';
    }
    if (mode == 1) {
        DrawRectangle(x, y, 2, inter.baseSize, WHITE);
    } else {
        GlyphInfo gi = GetGlyphInfo(inter, next_l2);
        DrawRectangle(x, y, gi.advanceX, inter.baseSize, WHITE);
    }
}

void draw_glyph(unsigned char l2) {
}

#define sum_y ((ccd_y * inter.baseSize) + (ccd_y * inter.glyphPadding))
void draw_buffer(void) {
    bool cursor_drawn = false; // has cursor been drawn in the current iteration of while loop?
    int ccd_x = 0, ccd_y = 0; // count charcters in directions
    float sum_x = 0; // sum for glyph x pos
    int i = 0; // iterator
    while (true) {
        cursor_drawn = false;
        unsigned char l2, pl2, nl2;
        unsigned int cp, pcp, ncp;
        if (i < lbuf_len) {
            l2 = lbuf[i];
        } else {
            l2 = rbuf[rbuf_len - (i - lbuf_len) - 1];
        }
        cp = latin2_to_unicode(l2);
        if (i > 0) {
            // calc previous cp and l2
            int pi = i - 1;
            if (pi < lbuf_len) {
                pl2 = lbuf[pi];
            } else {
                pl2 = rbuf[rbuf_len - (pi - lbuf_len) - 1];
            }
            pcp = latin2_to_unicode(pl2);
        }
        if (i+1 < lbuf_len + rbuf_len) {
            // calc next cp and l2
            int ni = i - 1;
            if (ni < lbuf_len) {
                nl2 = lbuf[ni];
            } else {
                nl2 = rbuf[rbuf_len - (ni - lbuf_len) - 1];
            }
            ncp = latin2_to_unicode(nl2);
        }

        if (i == lbuf_len) {
            draw_cursor(sum_x, (ccd_y * inter.baseSize) + (ccd_y * inter.glyphPadding), l2);
            /* cursor_drawn = true; TODO */
        }
        if (i >= lbuf_len + rbuf_len) {
            break;
        }
        i++;
        if (cp == '\n') {
            ccd_y += 1;
            ccd_x = 0;
            sum_x = 0;
            continue;
        }
        if (cp == '\t') {
            ccd_x++;
            GlyphInfo gi = GetGlyphInfo(inter, ' ');
            sum_x += gi.advanceX * 4;
            continue;
        }

        bool newline_after = false;
        GlyphInfo gi = GetGlyphInfo(inter, cp);
        if (sum_x + gi.advanceX * 2 > GetScreenWidth()) {
            cursor_drawn = true;
            newline_after = true;
            if (!ispunc(pcp) && !ispunc(cp)) {
                cp = '-';
                i--;
            } else if (ispunc(pcp) && !ispunc(cp)) {
                cp = '~';
                i--;
            } else {
                cursor_drawn = false;
            }
        }

        ccd_x++;
        Vector2 pos;
        pos.x = sum_x;
        pos.y = sum_y;
        sum_x += gi.advanceX;
        DrawTextCodepoint(inter, cp, pos, font_size, cursor_drawn ? DARKGRAY : WHITE);
        /* DrawTextCodepoint(inter, cp, pos, font_size, cursor_drawn && mode == 0 ? BLACK : WHITE); */

        if (newline_after == true) {
            ccd_y += 1;
            ccd_x = 0;
            sum_x = 0;
        }
    }
    // add virtual new line (on filesave it is added anyway)
    ccd_y += 1;
    ccd_x = 0;
    sum_x = 0;
    while (sum_y <= GetScreenHeight()) {
        DrawTextCodepoint(inter, '~', (Vector2){sum_x, sum_y}, font_size, DARKGRAY);
        ccd_y++;
    }
}

void normal(void) {
    if (IKPR(KEY_L)) {
        RBUF_PRELIJ_LBUF
    }
    if (IKPR(KEY_H)) {
        LBUF_PRELIJ_RBUF
    }
    if (IsKeyPressed(KEY_I)) {
        mode = 1;
    }
}

void input(void) {
    int c;
    while ((c = GetCharPressed())) {
        if (c == ' ' && IsKeyDown(KEY_LEFT_SHIFT)) { continue; }
        unsigned int l2 = unicode_to_latin2(c);
        printf("c: %c, d: %d, l2: %d, 0xl2: %X\n", c, c, l2, l2);
        APPEND_LBUF(l2);
    }
    if (IKPR(KEY_ENTER)) {
        APPEND_LBUF('\n');
    }
    if (IKPR(KEY_TAB)) {
        APPEND_LBUF('\t');
    }
    if (IKPSR(KEY_SPACE)) {
        APPEND_LBUF(' ');
        APPEND_LBUF(' ');
    }
    if (IKPR(KEY_BACKSPACE) && lbuf_len > 0) {
        lbuf_len--;
    }
    if (IKPCR(KEY_F)) {
        RBUF_PRELIJ_LBUF
    }
    if (IKPCR(KEY_B)) {
        LBUF_PRELIJ_RBUF
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
        int temp = rbuf_len;
        for (int i = 0; i < temp; i++) {
            RBUF_PRELIJ_LBUF
        }
        filesave();
    }
    if (IsKeyPressed(KEY_ESCAPE)) {
        /* mode = 0; */
    }
}

void draw(void) {
    draw_buffer();
}

int main(void) {
    /* unsigned char* hw_program =  "#include <stdio.h>\n\nint main(void) {\n\tprintf(\"hello world\\n\");\n\treturn 0;\n}"; */
    unsigned char* hw_program =  "And blood black nothingness began to spin, a system of cells.";
    lbuf_len = strlen(hw_program);
    memcpy(lbuf, hw_program, lbuf_len);
    /* for (int i = 0; i < lbuf; i++ ) { LBUF_PRELIJ_RBUF } */

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1600, 900, "aij.exe");
    SetExitKey(0);
    inter = LoadFontEx("JetBrainsMono-Regular.ttf", font_size, latin2_unicode_table, 256);

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

    UnloadFont(inter);
    CloseWindow();
    return 0;
}

unsigned int latin2_to_unicode(unsigned char l2) {
    return latin2_unicode_table[l2];
}

unsigned char unicode_to_latin2(unsigned int unicode) {
    for (int i = 0; i < 256; i++) {
        if (latin2_unicode_table[i] == unicode) {
            return i;
        }
    }
    return 0;
}
