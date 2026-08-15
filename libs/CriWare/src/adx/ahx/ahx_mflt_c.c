#include <harness_catalog.h>

/* AHX SBF filter multiply (extended).
 * a   = 32-tap filter coefficient array (fixed across both passes)
 * b   = signal history buffer (32-float blocks; block 0 is the current frame)
 * out = 64-float result buffer
 *
 * Result layout (float indices):
 *   out[0..15]   = dot(a, b + 32*i)              i = 0..15
 *   out[16]      = shared window-table constant (lbl_eu_80517548[0])
 *   out[17..32]  = -dot(a, b + 32*i), reversed    out[32-i] = -out[i]
 *   out[33..47]  = dot(a, b + 32*(33+i))         i = 0..14
 *   out[48]      = -sum(a[0..31])
 *   out[49..63]  = dot(a, b + 32*(33+i)), reversed
 *
 * The 32-tap dot product is written as 20 individual accumulate statements
 * followed by a 12-tap chained expression. That statement boundary keeps
 * MWCC's scheduler window shallow (loads run ~6 ahead like retail); a single
 * 32-tap chain makes MWCC hoist ~10 loads ahead, which overflows the FPR
 * scratch pool and forces extra callee-saved FPR claims (frame 0xB0 vs 0x90).
 * The retail is a single f0 chain; the residual (+2 instr per pass) comes from
 * the second statement's chain accumulating in a fresh register and copying
 * back to f0.
 */

extern float lbl_eu_80517548[];

#define AHX_DOT(sum, a, bb) \
    sum = (a)[0] * (bb)[0]; \
    sum += (a)[1] * (bb)[1]; \
    sum += (a)[2] * (bb)[2]; \
    sum += (a)[3] * (bb)[3]; \
    sum += (a)[4] * (bb)[4]; \
    sum += (a)[5] * (bb)[5]; \
    sum += (a)[6] * (bb)[6]; \
    sum += (a)[7] * (bb)[7]; \
    sum += (a)[8] * (bb)[8]; \
    sum += (a)[9] * (bb)[9]; \
    sum += (a)[10] * (bb)[10]; \
    sum += (a)[11] * (bb)[11]; \
    sum += (a)[12] * (bb)[12]; \
    sum += (a)[13] * (bb)[13]; \
    sum += (a)[14] * (bb)[14]; \
    sum += (a)[15] * (bb)[15]; \
    sum += (a)[16] * (bb)[16]; \
    sum += (a)[17] * (bb)[17]; \
    sum += (a)[18] * (bb)[18]; \
    sum += (a)[19] * (bb)[19]; \
    sum += (a)[20] * (bb)[20] + (a)[21] * (bb)[21] + (a)[22] * (bb)[22] + \
           (a)[23] * (bb)[23] + (a)[24] * (bb)[24] + (a)[25] * (bb)[25] + \
           (a)[26] * (bb)[26] + (a)[27] * (bb)[27] + (a)[28] * (bb)[28] + \
           (a)[29] * (bb)[29] + (a)[30] * (bb)[30] + (a)[31] * (bb)[31];

void ahxsbf_mult_flt_ex(float *a, float *b, float *out) {
    float *ofwd = out;
    float *bb = b;
    float *orev = out + 0x20;
    float *ifwd = out + 0x21;
    float *irev = out + 0x3F;
    float sum;
    float s;
    s32 i = 0x10;

    do {
        AHX_DOT(sum, a, bb)
        *ofwd++ = sum;
        *orev-- = -sum;
        bb += 0x20;
    } while (--i != 0);

    out[0x10] = lbl_eu_80517548[0];

    bb = b + 0x420;
    i = 0xF;
    do {
        AHX_DOT(sum, a, bb)
        *ifwd++ = sum;
        *irev-- = sum;
        bb += 0x20;
    } while (--i != 0);

    s = a[0];
    s += a[1];
    s += a[2];
    s += a[3];
    s += a[4];
    s += a[5];
    s += a[6];
    s += a[7];
    s += a[8];
    s += a[9];
    s += a[10];
    s += a[11];
    s += a[12];
    s += a[13];
    s += a[14];
    s += a[15];
    s += a[16];
    s += a[17];
    s += a[18];
    s += a[19];
    s += a[20];
    s += a[21];
    s += a[22];
    s += a[23];
    s += a[24];
    s += a[25];
    s += a[26];
    s += a[27];
    s += a[28];
    s += a[29];
    s += a[30];
    s += a[31];
    out[0x30] = -s;
}
