#include "buffer.h"

#include <stdlib.h>

bool iskwc(unsigned int c) {
    return (((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= 192 && c <= 687)) && c != 247 && c != 215);
}

gap_t gap_create(void) {
    gap_t gap = {
        .lbuf_len = 0,
        .rbuf_len = 0,
        .lbuf_size = 1000,
        .rbuf_size = 1000,
        .lbuf = NULL,
        .rbuf = NULL
    };
    gap.lbuf = malloc(sizeof(gap.lbuf[0]) * gap.lbuf_size);
    gap.rbuf = malloc(sizeof(gap.rbuf[0]) * gap.rbuf_size);
    return gap;
}

void append_lbuf(gap_t* gap, uint32_t el) {
    gap->lbuf[gap->lbuf_len++] = el;
}

void append_rbuf(gap_t* gap, uint32_t el) {
    gap->rbuf[gap->rbuf_len++] = el;
}

// put last element from lbuf to rbuf
void lbuf_prelij_rbuf(gap_t* gap) {
    if (gap->lbuf_len > 0) {
        append_rbuf(gap, gap->lbuf[--gap->lbuf_len]);
    }
}

// put last element from rbuf to lbuf
void rbuf_prelij_lbuf(gap_t* gap) {
    if (gap->rbuf_len > 0) {
        append_lbuf(gap, gap->rbuf[--gap->rbuf_len]);
    }
}

void cursor_to_start_of_word(gap_t* gap) {
    while (gap->rbuf_len > 0 && iskwc(gap->rbuf[gap->rbuf_len-1])) {
        rbuf_prelij_lbuf(gap);
    }
    while (gap->rbuf_len > 0 && !iskwc(gap->rbuf[gap->rbuf_len-1])) {
        rbuf_prelij_lbuf(gap);
    }
}

void cursor_backspace(gap_t* gap) {
    if (gap->lbuf_len == 0) {
        return;
    }
    gap->lbuf_len--;
}

void cursor_to_start_of_word_backwards(gap_t* gap) {
    while (gap->lbuf_len > 0 && !iskwc(gap->lbuf[gap->lbuf_len-1])) {
        lbuf_prelij_rbuf(gap);
    }
    while (gap->lbuf_len > 0 && iskwc(gap->lbuf[gap->lbuf_len-1])) {
        lbuf_prelij_rbuf(gap);
    }
}

void cursor_to_end_of_word(gap_t* gap) {
    while (gap->rbuf_len > 0 && !iskwc(gap->rbuf[gap->rbuf_len-1])) {
        rbuf_prelij_lbuf(gap);
    }
    while (gap->rbuf_len > 0 && iskwc(gap->rbuf[gap->rbuf_len-1])) {
        rbuf_prelij_lbuf(gap);
    }
}

void cursor_to_end_of_line(gap_t* gap) {
    while (gap->rbuf_len > 0 && gap->rbuf[gap->rbuf_len-1] != '\n') {
        rbuf_prelij_lbuf(gap);
    }
}

void cursor_to_start_of_line(gap_t* gap) {
    while (gap->lbuf_len > 0 && gap->lbuf[gap->lbuf_len-1] != '\n') {
        lbuf_prelij_rbuf(gap);
    }
    // almost cursor_to_start_of_word() except if already at start do nothing
    while (gap->rbuf_len > 0 && !iskwc(gap->rbuf[gap->rbuf_len-1])) {
        rbuf_prelij_lbuf(gap);
    }
}

void cursor_to_previous_line(gap_t* gap) {
    while (gap->lbuf_len > 0 && gap->lbuf[gap->lbuf_len-1] != '\n') {
        lbuf_prelij_rbuf(gap);
    }
    lbuf_prelij_rbuf(gap);
    cursor_to_start_of_line(gap);
}

void cursor_to_next_line(gap_t* gap) {
    cursor_to_end_of_line(gap);
    rbuf_prelij_lbuf(gap);
    cursor_to_start_of_line(gap);
}
