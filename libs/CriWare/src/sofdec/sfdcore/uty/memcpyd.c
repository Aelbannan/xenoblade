#include <harness_catalog.h>

void UTY_MemcpyDword(volatile u32 *dst, const volatile u32 *src, u32 n) {
    volatile u32 *d = dst;
    const volatile u32 *s = src;
    u32 t1, t2, t3;
    u32 tail_cnt;
    u32 main_cnt;

    /* Word-wise copy: tail loop handles the low 4 bits (n&0xF) one word at a
       time; the main loop copies n>>4 whole 64-byte blocks, each fully
       unrolled as 4 groups of 4 words (load-group, then store-group). */
    tail_cnt = (n & 0xF) + 1;
    goto tail_test;
tail_loop:
    *d = *s++;
    d++;
tail_test:
    if (--tail_cnt) goto tail_loop;

    main_cnt = (n >> 4) + 1;
    goto main_test;
main_loop:
    t1 = s[1]; t2 = s[2]; t3 = s[3];
    d[0] = s[0]; d[1] = t1; d[2] = t2; d[3] = t3;
    t1 = s[5]; t2 = s[6]; t3 = s[7];
    d[4] = s[4]; d[5] = t1; d[6] = t2; d[7] = t3;
    t1 = s[9]; t2 = s[10]; t3 = s[11];
    d[8] = s[8]; d[9] = t1; d[10] = t2; d[11] = t3;
    t1 = s[13]; t2 = s[14]; t3 = s[15];
    d[12] = s[12]; d[13] = t1; d[14] = t2; d[15] = t3;
    d += 16;
    s += 16;
main_test:
    if (--main_cnt) goto main_loop;
}
