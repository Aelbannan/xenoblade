#include <harness_catalog.h>

/* ahxsbf_mult_flt_ex is defined in ahx_mflt_c.c as (float*, float*, float*);
   retail AHXSBF_Synthesize passes two descriptors + a sink buffer and ignores
   any result. */
extern void ahxsbf_mult_flt_ex(float *a, float *b, float *out);

/* Function-pointer dispatch table at lbl_eu_805620E0, indexed by masked>>6. */
typedef void (*AHXSBF_SynthFunc)(s32 *dst, s32 arg, s32 *out);
extern AHXSBF_SynthFunc lbl_eu_805620E0[];

/* Per-voice synth slot: 0x14 bytes of state, then the sample work buffer
   filling the rest of the 0x1000-byte stride. */
typedef struct {
    u8 pad_[0x14];
    s32 w[(0x1000 - 0x14) / sizeof(s32)];
} AHXSBF_SynthSlot;

void AHXSBF_Synthesize(s32 *base, s32 pcm, s32 idx, s32 *out) {
    s32 *p = base + idx;
    s32 v = p[1] - 0x40;
    p[1] = v & 0x3FF;
    /* work buffer at base + idx*0x1000 + 0x14 via the slot struct */
    s32 *buf = ((AHXSBF_SynthSlot *)base + idx)->w;
    /* sink for the filter multiply; formed from the raw v so MWCC fuses the
       mask+scale (clrlslwi) like retail. */
    s32 *dst = buf + (v & 0x3FF);
    s32 coeff = base[3];

    /* misalignment gate: pcm/coeff descriptors must be 32-byte aligned */
    if ((pcm & 0x1F) != 0 || (coeff & 0x1F) != 0) {
        while (1) { }
    }

    ahxsbf_mult_flt_ex((float *)pcm, (float *)coeff, (float *)dst);

    AHXSBF_SynthFunc synth = lbl_eu_805620E0[p[1] >> 6];
    synth(buf + p[1], base[4], out);
}
