#include <harness_catalog.h>

int UTY_CmpTime(s32 a, s32 b, s32 c, s32 d) {
    s64 p1 = (s64)a * (s64)d;
    s64 p2 = (s64)c * (s64)b;
    return p2 >= p1 ? 1 : 0;
}
