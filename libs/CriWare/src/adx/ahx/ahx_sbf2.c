#include <harness_catalog.h>

/* ahxsbf_mult_flt_ex is defined in ahx_mflt_c.c as (float*, float*, float*);
   retail AHXSBF_Synthesize passes two descriptors + a sink buffer and ignores
   any result. */
extern void ahxsbf_mult_flt_ex(float *a, float *b, float *out);

/* Function-pointer dispatch table at lbl_eu_805620E0, indexed by masked>>6. */
typedef void (*AHXSBF_SynthFunc)(s32 *dst, s32 arg, s32 *out);
extern AHXSBF_SynthFunc lbl_eu_805620E0[];

void AHXSBF_Synthesize(s32 *base, s32 pcm, s32 idx, s32 *out) {
    s32 *p = base + idx;
    s32 *buf = (s32 *)((u8 *)base + (idx << 12) + 0x14);
    s32 masked = (p[1] - 0x40) & 0x3FF;
    p[1] = masked;
    s32 coeff = base[3];

    if ((pcm & 0x1F) != 0 || (coeff & 0x1F) != 0) {
        /* misalignment gate: pcm/coeff descriptors must be 32-byte aligned */
        while (1) { }
    }

    ahxsbf_mult_flt_ex((float *)pcm, (float *)coeff, (float *)(buf + masked));

    AHXSBF_SynthFunc synth = lbl_eu_805620E0[p[1] >> 6];
    synth(buf + p[1], base[4], out);
}