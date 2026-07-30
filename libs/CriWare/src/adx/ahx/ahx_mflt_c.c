#include <harness_catalog.h>

/* AHX synthesis filter: multiply-accumulate with filter coefficients
 * r3 = filter state array (r3+0x4C..0x6C are coefficients/state)
 * r4 = second filter state input
 * Returns filtered output sample */
f32 ahxsbf_mult_flt_ex(f32 *state1, f32 *state2) {
    f32 sum;
    int i;

    /* Filter computation: sum of products of coefficient pairs
     * from offsets 0x4C to 0x6C in both states */
    sum = 0.0f;

    for (i = 0; i < 9; i++) {
        sum += state1[0x4C/4 + i] * state2[0x4C/4 + i];
    }

    return sum;
}
