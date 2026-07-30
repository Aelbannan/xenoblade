#include <harness_catalog.h>

int UTY_CmpTime(s32 a, s32 b, s32 c, s32 d) {
    s64 ad = (s64)a * (s64)d;
    s64 cb = (s64)c * (s64)b + ad - ad;
    return cb >= ad ? 1 : 0;
}
