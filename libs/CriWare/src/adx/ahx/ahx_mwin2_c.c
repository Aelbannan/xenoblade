#include <harness_catalog.h>

/* AHX synthesis window functions (mwin2).
 * Each applies a window multiply-accumulate over 16 taps and writes a
 * clamped s16 sample.  The accumulator seed and clamp limits come from the
 * shared window table lbl_eu_80517550.  Window N slides its last N taps one
 * 0x1000-byte block down so consecutive windows sweep across the input
 * history.
 */

extern float lbl_eu_80517550[];

/* tap offset in bytes for window 0; later windows shift taps k >= 16-N */
#define WIN_TAP(N, K, OFF) in[((OFF) - ((N) + (K) >= 16 ? 0x1000 : 0)) / 4]

#define WIN(N) \
void ahxsbf_mult_float_win##N(float *in, float *coeff, s16 *out) { \
    float *tbl = lbl_eu_80517550; \
    float sum; \
    float lo = tbl[2]; \
    float hi = tbl[1]; \
    s32 i = 32; \
    do { \
        sum = tbl[0]; \
        sum += coeff[0] * WIN_TAP(N, 0, 0x000); \
        sum += coeff[1] * WIN_TAP(N, 1, 0x180); \
        sum += coeff[2] * WIN_TAP(N, 2, 0x200); \
        sum += coeff[3] * WIN_TAP(N, 3, 0x380); \
        sum += coeff[4] * WIN_TAP(N, 4, 0x400); \
        sum += coeff[5] * WIN_TAP(N, 5, 0x580); \
        sum += coeff[6] * WIN_TAP(N, 6, 0x600); \
        sum += coeff[7] * WIN_TAP(N, 7, 0x780); \
        sum += coeff[8] * WIN_TAP(N, 8, 0x800); \
        sum += coeff[9] * WIN_TAP(N, 9, 0x980); \
        sum += coeff[10] * WIN_TAP(N, 10, 0xa00); \
        sum += coeff[11] * WIN_TAP(N, 11, 0xb80); \
        sum += coeff[12] * WIN_TAP(N, 12, 0xc00); \
        sum += coeff[13] * WIN_TAP(N, 13, 0xd80); \
        sum += coeff[14] * WIN_TAP(N, 14, 0xe00); \
        sum += coeff[15] * WIN_TAP(N, 15, 0xf80); \
        in++; \
        coeff += 16; \
        if (sum > hi) { \
            *out++ = 0x7FFF; \
        } else if (sum < lo) { \
            *out++ = -0x8000; \
        } else { \
            *out++ = (s16)((s32)sum >> 16); \
        } \
    } while (--i != 0); \
}

WIN(0) WIN(1) WIN(2) WIN(3)
WIN(4) WIN(5) WIN(6) WIN(7)
WIN(8) WIN(9) WIN(10) WIN(11)
WIN(12) WIN(13) WIN(14) WIN(15)
