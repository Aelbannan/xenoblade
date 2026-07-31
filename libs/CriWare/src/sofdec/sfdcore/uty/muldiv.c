#include <harness_catalog.h>

int UTY_MulDiv(int a, int b, int c) {
    int r;
    if (c == 0) {
        r = (s32)((u32)(a ^ b) >> 31) - 1 + 0x80000000;
    } else {
        r = (int)((s64)a * (s64)b / c);
    }
    return r;
}
