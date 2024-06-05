#include <stdint.h>
#include <stdbool.h>

// returns: advanceX
typedef float (*draw_char)(uint32_t unicode, float x, int line_number);

void editor_init();
void editor_dispose();
void editor_handle_charinput(uint32_t unicode);
void editor_handle_keyinput(int key, bool ctrl_down, bool shift_down, bool alt_down);
void editor_draw(int xscroll, int yscroll, draw_char dcf);
