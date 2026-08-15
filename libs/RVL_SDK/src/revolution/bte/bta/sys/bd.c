// Auto-scaffolded catalog TU for RVL_SDK/src/revolution/bte/bta/sys/bd
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

/* Retail .sbss2 slice is 8 bytes (align 8): null BD_ADDR (6 bytes) + 2 pad
 * (same +2 shape as BT_BD_ANY in btu_init.c). Global const zero-fill -> .sbss2. */
const u8 bd_addr_null[8] = {0, 0, 0, 0, 0, 0, 0, 0};

void bdcpy(void *dst, const void *src) {
    ((unsigned char *)dst)[0] = ((const unsigned char *)src)[0];
    ((unsigned char *)dst)[1] = ((const unsigned char *)src)[1];
    ((unsigned char *)dst)[2] = ((const unsigned char *)src)[2];
    ((unsigned char *)dst)[3] = ((const unsigned char *)src)[3];
    ((unsigned char *)dst)[4] = ((const unsigned char *)src)[4];
    ((unsigned char *)dst)[5] = ((const unsigned char *)src)[5];
}

/* Compare two BD_ADDRs byte by byte; -1 on first mismatch, 0 when equal. */
int bdcmp(const unsigned char *p1, const unsigned char *p2) {
    unsigned char i;
    for (i = 0; i < 6; i++) {
        if (p1[i] != p2[i]) {
            return -1;
        }
    }
    return 0;
}
