#include <stdint.h>

int unicode_to_utf8(char* out, uint32_t unicode);
int utf8_to_unicode(uint32_t* out, char* utf);
