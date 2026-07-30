#include <harness_catalog.h>

int UTY_MulDiv(int a, int b, int c) {
    if (c == 0) {
        return 0x7FFFFFFF + ((u32)(a ^ b) >> 31);
    }
    return (int)((s64)a * (s64)b / c);
}
