#include <harness_catalog.h>

/* ahxsbf_mult_flt_ex is defined in ahx_mflt_c.c as (float*, float*, float*);
   retail AHXSBF_Synthesize passes two descriptors + a sink buffer and ignores
   any result. */
extern void ahxsbf_mult_flt_ex(float *a, float *b, float *out);

/* Function-pointer dispatch table at lbl_eu_805620E0, indexed by masked>>6.
   Data definition lives in port/data_defs.cpp. */
typedef void (*AHXSBF_SynthFunc)(s32 *dst, s32 arg, s32 *out);
extern AHXSBF_SynthFunc lbl_eu_805620E0[];

/*
 * Per-voice synth slot layout: a 0x14-byte state header followed by the
 * sample work buffer that fills the rest of the 0x1000-byte stride.
 *
 *   offset 0x00: base[0..3] - frame state; [1] is the sample cursor
 *                (10-bit, wraps at 0x400), [3] is the coefficient
 *                descriptor pointer, [4] is the synth argument
 *   offset 0x14: work buffer (s32 w[...])
 */
typedef struct {
    u8 pad_[0x14];
    s32 w[(0x1000 - 0x14) / sizeof(s32)];
} AHXSBF_SynthSlot;

/*
 * AHXSBF_Synthesize - decode one subband frame for voice `idx`.
 *
 * Steps:
 *  1. Advance the 10-bit sample cursor in the slot header, keeping the
 *     pre-mask value so the mask+scale fuses into a single clrlslwi.
 *  2. Validate that the PCM and coefficient descriptors are 32-byte
 *     aligned; spin forever on misalignment (retail behaviour).
 *  3. Run the synthesis filter multiply into the work buffer at the
 *     cursor offset.
 *  4. Dispatch through the function table on cursor>>6 to finish the
 *     block into `out`.
 */
void AHXSBF_Synthesize(s32 *base, s32 pcm, s32 idx, s32 *out) {
    s32 *hdr = base + idx;
    s32 cursor = hdr[1] - 0x40;
    hdr[1] = cursor & 0x3FF;
    /* work buffer at base + idx*0x1000 + 0x14 via the slot struct */
    s32 *buf = ((AHXSBF_SynthSlot *)base + idx)->w;
    /* sink for the filter multiply; formed from the raw cursor value so
       MWCC fuses the mask+scale (clrlslwi) like retail. */
    s32 *dst = buf + (cursor & 0x3FF);
    s32 coeff = base[3];

    /* misalignment gate: pcm/coeff descriptors must be 32-byte aligned */
    if ((pcm & 0x1F) != 0 || (coeff & 0x1F) != 0) {
        while (1) { }
    }

    ahxsbf_mult_flt_ex((float *)pcm, (float *)coeff, (float *)dst);

    AHXSBF_SynthFunc synth = lbl_eu_805620E0[hdr[1] >> 6];
    synth(buf + hdr[1], base[4], out);
}
