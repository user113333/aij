#include <stdint.h>
#include <stdbool.h>

typedef struct {
    float width;
    float height;
    float padding_horizontal;
    float padding_vertical;
} fontinfo_t;

typedef struct {
    int width;
    int height;
} rendererinfo_t;

// returns: advanceX
typedef float (*draw_char_function)(uint32_t unicode, float x, float y);
typedef void (*draw_cursor_function)(float x, float y);

void editor_init();
void editor_dispose();
void editor_handle_charinput(uint32_t unicode);
void editor_handle_keyinput(int key, bool ctrl_down, bool shift_down, bool alt_down);
void editor_draw(rendererinfo_t, fontinfo_t, draw_char_function, draw_cursor_function);
