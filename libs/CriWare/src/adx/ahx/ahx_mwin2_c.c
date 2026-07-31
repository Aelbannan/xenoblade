#include <harness_catalog.h>

/* AHX synthesis window functions (mwin2).
 * Each applies a window multiply-accumulate over 16 taps and writes a
 * clamped s16 sample.  The accumulator seed and clamp limits come from the
 * shared window table lbl_eu_80517550.
 */

extern float lbl_eu_80517550[];

#define WIN(N) \
void ahxsbf_mult_float_win##N(float *in, float *coeff, s16 *out) { \
    float sum = lbl_eu_80517550[0]; \
    float lo = lbl_eu_80517550[2]; \
    float hi = lbl_eu_80517550[1]; \
    s32 i; \
    for (i = 32; i != 0; i--) { \
        sum = lbl_eu_80517550[0]; \
        sum += coeff[0] * in[0x000 / 4]; \
        sum += coeff[1] * in[0x180 / 4]; \
        sum += coeff[2] * in[0x200 / 4]; \
        sum += coeff[3] * in[0x380 / 4]; \
        sum += coeff[4] * in[0x400 / 4]; \
        sum += coeff[5] * in[0x580 / 4]; \
        sum += coeff[6] * in[0x600 / 4]; \
        sum += coeff[7] * in[0x780 / 4]; \
        sum += coeff[8] * in[0x800 / 4]; \
        sum += coeff[9] * in[0x980 / 4]; \
        sum += coeff[10] * in[0xa00 / 4]; \
        sum += coeff[11] * in[0xb80 / 4]; \
        sum += coeff[12] * in[0xc00 / 4]; \
        sum += coeff[13] * in[0xd80 / 4]; \
        sum += coeff[14] * in[0xe00 / 4]; \
        sum += coeff[15] * in[0xf80 / 4]; \
        if (sum > hi) { \
            *out++ = 0x7FFF; \
        } else if (sum < lo) { \
            *out++ = (s16)-0x8000; \
        } else { \
            *out++ = (s16)sum; \
        } \
        in++; coeff += 16; \
    } \
}

WIN(0) WIN(1) WIN(2) WIN(3)
WIN(4) WIN(5) WIN(6) WIN(7)
WIN(8) WIN(9) WIN(10) WIN(11)
WIN(12) WIN(13) WIN(14) WIN(15)
