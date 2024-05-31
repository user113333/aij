#include "utf8.h"

// from https://gist.github.com/MightyPork/52eda3e5677b4b03524e40c9f0ab1da5
int unicode_to_utf8(char* out, uint32_t unicode) {
    if (unicode <= 0x7F) {
        // Plain ASCII
        out[0] = (char) unicode;
        out[1] = 0;
        return 1;
    }
    else if (unicode <= 0x07FF) {
        // 2-byte unicode
        out[0] = (char) (((unicode >> 6) & 0x1F) | 0xC0);
        out[1] = (char) (((unicode >> 0) & 0x3F) | 0x80);
        out[2] = 0;
        return 2;
    }
    else if (unicode <= 0xFFFF) {
        // 3-byte unicode
        out[0] = (char) (((unicode >> 12) & 0x0F) | 0xE0);
        out[1] = (char) (((unicode >>  6) & 0x3F) | 0x80);
        out[2] = (char) (((unicode >>  0) & 0x3F) | 0x80);
        out[3] = 0;
        return 3;
    }
    else if (unicode <= 0x10FFFF) {
        // 4-byte unicode
        out[0] = (char) (((unicode >> 18) & 0x07) | 0xF0);
        out[1] = (char) (((unicode >> 12) & 0x3F) | 0x80);
        out[2] = (char) (((unicode >>  6) & 0x3F) | 0x80);
        out[3] = (char) (((unicode >>  0) & 0x3F) | 0x80);
        out[4] = 0;
        return 4;
    }
    else { 
        // error - use replacement character
        out[0] = (char) 0xEF;  
        out[1] = (char) 0xBF;
        out[2] = (char) 0xBD;
        out[3] = 0;
        return 0;
    }
}
