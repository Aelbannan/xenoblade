#include <harness_catalog.h>

void UTY_MemsetDword(u32 *dst, u32 val, u32 n) {
    u32 *p = dst + n;
    u32 cnt;
    
    cnt = (n & 0xF) + 1;
    do {
        *--p = val;
    } while (--cnt);
    
    cnt = (n >> 4) + 1;
    do {
        p[-1] = val; p[-2] = val; p[-3] = val; p[-4] = val;
        p[-5] = val; p[-6] = val; p[-7] = val; p[-8] = val;
        p[-9] = val; p[-10] = val; p[-11] = val; p[-12] = val;
        p[-13] = val; p[-14] = val; p[-15] = val;
        p -= 16;
        p[0] = val;
    } while (--cnt);
}
