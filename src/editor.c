#include "editor.h"

#include "ik.h"
#include "buffer.h"

unsigned int tabstop = 4;

static buffer_t* curbuf;

static void insertnewline() { append_lbuf(curbuf, '\n'); }
static void inserttab() { append_lbuf(curbuf, '\t'); }
static void deleteleft() { cursor_delete_left(curbuf); }
static void moveright() { rbuf_prelij_lbuf(curbuf); }
static void moveleft() { lbuf_prelij_rbuf(curbuf); }
static void nextword() { cursor_to_start_of_word(curbuf); }
static void previousword() { cursor_to_start_of_word_backwards(curbuf); }
static void nextwordend() { cursor_to_end_of_word(curbuf); }
static void lineend() { cursor_to_end_of_line(curbuf); }
static void linestart() { cursor_to_start_of_line(curbuf); }
static void lineup() { cursor_to_previous_line(curbuf); }
static void linedown() { cursor_to_next_line(curbuf); }

typedef struct {
    void (*action)(void);
    unsigned int ik;
    bool repeat;
    bool with_ctrl;
    bool with_shift;
    bool with_alt;
} map_t;

static map_t map[] = {
    { insertnewline, IK_ENTER, true, false, false, false },
    { inserttab, IK_TAB, true, false, false, false },
    { deleteleft, IK_BACKSPACE, true, false, false, false },
    { moveright, IK_L, true, true, false, false },
    { moveleft, IK_H, true, true, false, false },
    { nextword, IK_L, true, false, false, true },
    { previousword, IK_H, true, false, false, true },
    { nextwordend, IK_E, true, false, false, true },
    { lineend, IK_E, true, true, false, false },
    { linestart, IK_A, true, true, false, false },
    { lineup, IK_K, true, true, false, false },
    { linedown, IK_J, true, true, false, false },
};

buffer_t b;
void editor_init() {
    b = buffer_create();
    curbuf = &b;
}

void editor_dispose() {
    buffer_destroy(b);
}

void editor_handle_charinput(uint32_t unicode) {
    append_lbuf(curbuf, unicode);
}

void editor_handle_keyinput(int key, bool ctrl_down, bool shift_down, bool alt_down) {
    for (int i = 0; i < (sizeof(map) / sizeof(map[0])); i++) {
        if (map[i].ik == key &&
            map[i].with_ctrl == ctrl_down &&
            map[i].with_shift == shift_down &&
            map[i].with_alt == alt_down) {
            map[i].action();
            break;
        }
    }
}

void editor_draw(rendererinfo_t rend, fontinfo_t font, draw_char_function draw_char, draw_cursor_function draw_cursor) {
    float x = 0;
    float line_number = 0;
    for (int i = 0; i < curbuf->lbuf_len + curbuf->rbuf_len + 1; i++) {
        if (i == curbuf->lbuf_len) {
            draw_cursor(x, line_number * (font.height + font.padding_horizontal));
        }
        if (i >= curbuf->lbuf_len + curbuf->rbuf_len) { break; }
        uint32_t c;
        if (i < curbuf->lbuf_len) {
            c = curbuf->lbuf[i];
        } else {
            c = curbuf->rbuf[curbuf->rbuf_len - (i - curbuf->lbuf_len) - 1];
        }
        if (c == '\n') {
            line_number++;
            x = 0;
            continue;
        }
        if (c == '\t') {
            for (int i = 0; i < tabstop; i++) {
                x += draw_char(' ', x, line_number * (font.height + font.padding_horizontal));
            }
            continue;
        }
        float advanceX = draw_char(c, x, line_number * (font.height + font.padding_horizontal));
        x += advanceX;
    }
}
