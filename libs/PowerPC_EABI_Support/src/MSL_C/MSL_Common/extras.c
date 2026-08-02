#include <types.h>
#include <ctype.h>

int stricmp(const char* s1, const char* s2) {
    char c1, c2;
    do {
        c1 = tolower(*s1++);
        c2 = tolower(*s2++);
        if (c1 < c2) return -1;
        if (c1 > c2) return 1;
    } while (c1 != 0);
    return 0;
}

