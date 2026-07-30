#include <harness_catalog.h>

/* AHX synthesis window functions.
 * Each applies a specific window to audio data using multiply-accumulate.
 * r3 = input/output buffer, r4 = coefficient table
 * Window coefficients are at lbl_eu_80517550.
 */

extern float lbl_eu_80517550[];

#define WIN(N) \
void ahxsbf_mult_float_win##N(float *buf, float *coeff) { \
    float sum; \
    int i; \
    for (i = 0; i < 32; i++) { \
        sum = coeff[i * 4 + 0] * buf[i * 0x80 / 4] \
            + coeff[i * 4 + 1] * buf[(i * 0x80 + 0x180) / 4] \
            + coeff[i * 4 + 2] * buf[(i * 0x80 + 0x200) / 4] \
            + coeff[i * 4 + 3] * buf[(i * 0x80 + 0x380) / 4]; \
        buf[i] = sum; \
    } \
}

WIN(0) WIN(1) WIN(2) WIN(3)
WIN(4) WIN(5) WIN(6) WIN(7)
WIN(8) WIN(9) WIN(10) WIN(11)
WIN(12) WIN(13) WIN(14) WIN(15)
