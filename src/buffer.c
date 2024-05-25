#include "buffer.h"

#include <stdlib.h>

// is tab or space
bool istos(unsigned int c) {
    return c == ' ' || c == '\t';
}

// is keyword character
bool iskwc(unsigned int c) {
    return (((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= 192 && c <= 687)) && c != 247 && c != 215);
}

buffer_t buffer_create(void) {
    buffer_t b = {
        .lbuf_len = 0,
        .rbuf_len = 0,
        .lbuf_size = 1000,
        .rbuf_size = 1000,
        .lbuf = NULL,
        .rbuf = NULL
    };
    b.lbuf = malloc(sizeof(b.lbuf[0]) * b.lbuf_size);
    b.rbuf = malloc(sizeof(b.rbuf[0]) * b.rbuf_size);
    return b;
}

void append_lbuf(buffer_t* b, uint32_t el) {
    b->lbuf[b->lbuf_len++] = el;
}

void append_rbuf(buffer_t* b, uint32_t el) {
    b->rbuf[b->rbuf_len++] = el;
}

// put last element from lbuf to rbuf
void lbuf_prelij_rbuf(buffer_t* b) {
    if (b->lbuf_len > 0) {
        append_rbuf(b, b->lbuf[--b->lbuf_len]);
    }
}

// put last element from rbuf to lbuf
void rbuf_prelij_lbuf(buffer_t* b) {
    if (b->rbuf_len > 0) {
        append_lbuf(b, b->rbuf[--b->rbuf_len]);
    }
}

void cursor_to_start_of_word(buffer_t* b) {
    while (b->rbuf_len > 0 && iskwc(b->rbuf[b->rbuf_len-1])) {
        rbuf_prelij_lbuf(b);
    }
    while (b->rbuf_len > 0 && !iskwc(b->rbuf[b->rbuf_len-1])) {
        rbuf_prelij_lbuf(b);
    }
}

void cursor_backspace(buffer_t* b) {
    if (b->lbuf_len == 0) {
        return;
    }
    b->lbuf_len--;
}

void cursor_to_start_of_word_backwards(buffer_t* b) {
    while (b->lbuf_len > 0 && !iskwc(b->lbuf[b->lbuf_len-1])) {
        lbuf_prelij_rbuf(b);
    }
    while (b->lbuf_len > 0 && iskwc(b->lbuf[b->lbuf_len-1])) {
        lbuf_prelij_rbuf(b);
    }
}

void cursor_to_end_of_word(buffer_t* b) {
    while (b->rbuf_len > 0 && !iskwc(b->rbuf[b->rbuf_len-1])) {
        rbuf_prelij_lbuf(b);
    }
    while (b->rbuf_len > 0 && iskwc(b->rbuf[b->rbuf_len-1])) {
        rbuf_prelij_lbuf(b);
    }
}

void cursor_to_end_of_line(buffer_t* b) {
    while (b->rbuf_len > 0 && b->rbuf[b->rbuf_len-1] != '\n') {
        rbuf_prelij_lbuf(b);
    }
}

void cursor_to_start_of_line(buffer_t* b) {
    while (b->lbuf_len > 0 && b->lbuf[b->lbuf_len-1] != '\n') {
        lbuf_prelij_rbuf(b);
    }
    // next while is almost cursor_to_start_of_word() except if already at start do nothing
    while (b->rbuf_len > 0 && istos(b->rbuf[b->rbuf_len-1])) {
        rbuf_prelij_lbuf(b);
    }
}

void cursor_to_complete_start_of_line(buffer_t* b) {
    while (b->lbuf_len > 0 && b->lbuf[b->lbuf_len-1] != '\n') {
        lbuf_prelij_rbuf(b);
    }
}

void cursor_to_previous_line(buffer_t* b) {
    cursor_to_complete_start_of_line(b);
    lbuf_prelij_rbuf(b);
    cursor_to_start_of_line(b);
}

void cursor_to_next_line(buffer_t* b) {
    cursor_to_end_of_line(b);
    rbuf_prelij_lbuf(b);
    cursor_to_start_of_line(b);
}
