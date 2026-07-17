#include "PowerPC_EABI_Support/MetroTRK/string_TRK.h"

int TRK_strlen(const char* s) {
    const unsigned char* p = (const unsigned char*)s - 1;
    int len = -1;
    unsigned char c;
    do {
        c = *++p;
        len++;
    } while (c != 0);
    return len;
}
