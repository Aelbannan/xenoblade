#include <harness_catalog.h>

/* ahxsbf_mult_flt_ex is defined in ahx_mflt_c.c with a 3-float-pointer
   signature; retail AHXSBF_Synthesize however calls it as (pcm, coeff) with
   two scalar args and ignores the result. Declare it locally the way the
   call site uses it so the emitted call matches retail without altering the
   already-matched mflt function. */
extern void ahxsbf_mult_flt_ex(s32 pcm, s32 coeff);

/* Function-pointer dispatch table at lbl_eu_805620E0, indexed by masked>>6. */
typedef void (*AHXSBF_SynthFunc)(s32 *dst, s32 arg, s32 *out);
extern AHXSBF_SynthFunc lbl_eu_805620E0[];

void AHXSBF_Synthesize(s32 *base, s32 pcm, s32 idx, s32 *out) {
    s32 *p = base + idx;
    s32 *buf = (s32 *)((u8 *)base + (idx << 12) + 0x14);
    s32 masked = (p[1] - 0x40) & 0x3FF;
    p[1] = masked;

    /* read after the store: p[1] (= base[idx+1]) may alias base[3] */
    s32 coeff = base[3];

    if ((pcm & 0x1F) != 0 || (coeff & 0x1F) != 0) {
        /* gate: spinloop while pcm/coeff pointers are misaligned */
        while (1) { }
    }

    ahxsbf_mult_flt_ex(pcm, coeff);

    /* p[1] == masked; reload it once and reuse for both the table index
       and the buffer offset (retail: lwz r4,4(r31)). */
    AHXSBF_SynthFunc synth = lbl_eu_805620E0[p[1] >> 6];
    synth(buf + p[1], base[4], out);
}