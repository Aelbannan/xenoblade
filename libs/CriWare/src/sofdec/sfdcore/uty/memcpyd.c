#include <harness_catalog.h>

void UTY_MemcpyDword(volatile u32 *dst, const volatile u32 *src, u32 n) {
    volatile u32 *d = dst;
    const volatile u32 *s = src;
    u32 t1, t2, t3, t0;
    u32 cnt;

    cnt = (n & 0xF) + 1;
    goto tail_test;
tail_loop:
    *d = *s++;
    d++;
tail_test:
    if (--cnt) goto tail_loop;

    cnt = (n >> 4) + 1;
    goto main_test;
main_loop:
    t1 = s[1];
    t2 = s[2];
    t3 = s[3];
    t0 = s[0];
    d[0] = t0; d[1] = t1; d[2] = t2; d[3] = t3;
    t1 = s[5];
    t2 = s[6];
    t3 = s[7];
    t0 = s[4];
    d[4] = t0; d[5] = t1; d[6] = t2; d[7] = t3;
    t1 = s[9];
    t2 = s[10];
    t3 = s[11];
    t0 = s[8];
    d[8] = t0; d[9] = t1; d[10] = t2; d[11] = t3;
    t1 = s[13];
    t2 = s[14];
    t3 = s[15];
    t0 = s[12];
    d[12] = t0; d[13] = t1; d[14] = t2; d[15] = t3;
    d += 16;
    s += 16;
main_test:
    if (--cnt) goto main_loop;
}
