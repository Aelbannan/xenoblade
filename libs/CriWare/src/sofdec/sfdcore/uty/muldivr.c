#include <harness_catalog.h>

s64 UTY_MulDivRound64(s64 a, s64 b, s64 c) {
    s32 sign = 1;
    u64 n;
    if (c == 0) {
        return ((a ^ b) < 0) ? 0x8000000000000000LL : 0x7FFFFFFFFFFFFFFFLL;
    }
    if (a < 0) {
        a = -a;
        sign = -sign;
    }
    if (b < 0) {
        b = -b;
        sign = -sign;
    }
    if (c < 0) {
        c = -c;
        sign = -sign;
    }
    n = (u64)a * (u64)b;
    n += (u64)(c >> 1) + (c > 0);
    n = (s64)n / c;
    if (sign < 0)
        n = (u64)(-(s64)n);
    return (s64)n;
}
