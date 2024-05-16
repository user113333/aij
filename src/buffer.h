#include <stdint.h>
#include <stdbool.h>

// is keyword character
bool iskwc(unsigned int c);

typedef struct {
    uint32_t lbuf_len;
    uint32_t rbuf_len;
    uint32_t lbuf_size;
    uint32_t rbuf_size;
    uint32_t* lbuf;
    uint32_t* rbuf;
} gap_t;

gap_t gap_create(void);
void append_lbuf(gap_t* gap, uint32_t el);
void append_rbuf(gap_t* gap, uint32_t el);
void lbuf_prelij_rbuf(gap_t* gap);
void rbuf_prelij_lbuf(gap_t* gap);

//cursor is refering to the end of gap.lbuf
void cursor_backspace(gap_t* gap);
void cursor_to_start_of_word(gap_t* gap);
void cursor_to_start_of_word_backwards(gap_t* gap);
void cursor_to_end_of_word(gap_t* gap);
void cursor_to_end_of_line(gap_t* gap);
void cursor_to_start_of_line(gap_t* gap);
void cursor_to_previous_line(gap_t* gap);
void cursor_to_next_line(gap_t* gap);
