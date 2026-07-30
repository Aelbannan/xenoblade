#include <harness_catalog.h>

void UTY_MemcpyDword(u32 *dst, const u32 *src, u32 n) {
    u32 tail, i;
    tail = (n & 0xF) + 1;
    for (i = 0; i < tail; i++) {
        *dst++ = *src++;
    }
    tail = (n >> 4) + 1;
    for (i = 0; i < tail; i++) {
        dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2]; dst[3] = src[3];
        dst[4] = src[4]; dst[5] = src[5]; dst[6] = src[6]; dst[7] = src[7];
        dst[8] = src[8]; dst[9] = src[9]; dst[10] = src[10]; dst[11] = src[11];
        dst[12] = src[12]; dst[13] = src[13]; dst[14] = src[14]; dst[15] = src[15];
        dst += 16;
        src += 16;
    }
}
