#include <harness_catalog.h>

extern void ahxsbf_mult_flt_ex(s32 pcm, s32 coeff);
extern void *lbl_eu_805620E0;

void AHXSBF_Synthesize(s32 *base, s32 pcm, s32 idx, s32 *out) {
    s32 *p = base + idx;
    s32 v = p[1] - 0x40;
    s32 masked = v & 0x3FF;
    s32 v_shifted = v >> 6;
    s32 *r30 = (s32 *)((u8 *)base + (idx << 12) + 0x14);

    p[1] = masked;

    s32 r7 = base[3];
    if ((pcm & 0x1F) != 0 || (r7 & 0x1F) != 0) {
        /* spinlock/busy-wait - intentional infinite loop if alignment wrong */
        while (1) { }
    }

    s32 *table = (s32 *)((u8 *)&lbl_eu_805620E0 + (v_shifted << 2));
    void (*synth_func)(s32 *, s32, s32 *) = (void (*)(s32 *, s32, s32 *))table[0];
    synth_func(r30 + (masked << 2), r7, out);
}
