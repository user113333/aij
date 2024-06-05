#include <stdint.h>
#include <stdbool.h>

bool istos(unsigned int c);
bool iskwc(unsigned int c);

typedef struct {
    uint32_t lbuf_len;
    uint32_t rbuf_len;
    uint32_t lbuf_size;
    uint32_t rbuf_size;
    uint32_t* lbuf;
    uint32_t* rbuf;
} buffer_t;

buffer_t buffer_create(void);
void buffer_destroy(buffer_t b);
void append_lbuf(buffer_t* b, uint32_t el);
void append_rbuf(buffer_t* b, uint32_t el);
void lbuf_prelij_rbuf(buffer_t* b);
void rbuf_prelij_lbuf(buffer_t* b);

void cursor_delete_left(buffer_t* b);
void cursor_delete_rigth(buffer_t* b);
void cursor_to_start_of_word(buffer_t* b);
void cursor_to_start_of_word_backwards(buffer_t* b);
void cursor_to_end_of_word(buffer_t* b);
void cursor_to_end_of_line(buffer_t* b);
void cursor_to_start_of_line(buffer_t* b);
void cursor_to_complete_start_of_line(buffer_t* b);
void cursor_to_previous_line(buffer_t* b);
void cursor_to_next_line(buffer_t* b);
void curosr_to_previous_paragraph(buffer_t* b);
void cursor_to_next_paragraph(buffer_t* b);
