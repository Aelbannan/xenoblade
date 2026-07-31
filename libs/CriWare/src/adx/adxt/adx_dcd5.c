#include <harness_catalog.h>

/* Signed-nibble table: tbl[n] = n for 0..7, n-16 for 8..15 */
extern s32 lbl_eu_80517428[];
extern s32 lbl_eu_805F2AF0;

/* ADX version 4 mono decoder
 * in:   input block stream
 * nblocks: number of blocks
 * out:  decoded samples (32 per block)
 * prev: 2-sample filter state (written back)
 * c1, c2: filter coefficients
 * hist: predictor history (single s16, written back)
 * scale: predictor multiplier
 * pitch: predictor offset
 * Returns the number of the block that stopped the decode, or nblocks. */
int ADX_DecodeMono4(const u8 *in, int nblocks, s16 *out, s16 *prev,
                    s16 c1, s16 c2, s16 *hist, s16 scale, s16 pitch)
{
    int block, i;
    s32 prev_lo, prev_hi;
    s32 hist_val;
    s16 scale_val;
    s32 nib_hi;
    s32 pcm, pcm2;
    s32 tbl_val;

    prev_lo = prev[0];
    prev_hi = prev[1];

    for (block = 0; block < nblocks; block++) {
        s16 header = *(s16 *)in;

        if (header & 0x8000)
            return block;

        hist_val = hist[0];
        hist[0] = (s16)((pitch + hist_val * scale) & 0x7FFF);
        scale_val = (s16)(((header ^ hist_val) & 0x1FFF) + 1);

        in += 2;

        for (i = 0; i < 16; i++) {
            s32 byte = (s8)*in++;
            nib_hi = byte >> 4;

            pcm = ((c1 * prev_lo + c2 * prev_hi) >> 12) + nib_hi * scale_val;
            if ((u32)(pcm + 0x8000) > 0xFFFF) {
            if (pcm < -0x8000)
                pcm = -0x8000;
            else if (pcm > 0x7FFF)
                pcm = 0x7FFF;
        }
            out[0] = (s16)pcm;

            tbl_val = lbl_eu_80517428[byte & 0xF];
            pcm2 = ((c1 * pcm + c2 * prev_lo) >> 12) + tbl_val * scale_val;
            if ((u32)(pcm2 + 0x8000) > 0xFFFF) {
            if (pcm2 < -0x8000)
                pcm2 = -0x8000;
            else if (pcm2 > 0x7FFF)
                pcm2 = 0x7FFF;
        }
            out[1] = (s16)pcm2;

            prev_lo = pcm2;
            prev_hi = pcm;
            out += 2;
        }
    }

    prev[0] = (s16)prev_lo;
    prev[1] = (s16)prev_hi;
    return nblocks;
}

/* ADX version 4 stereo decoder, mixed to mono.
 * Decodes both channels and averages each pair as (ch1 + ch2) * 7 / 10;
 * the result is written to both outputs. */
int ADX_DecodeSte4AsMono(const u8 *in, int nblocks, s16 *out1, s16 *prev1,
                         s16 *out2, s16 *prev2, s16 c1, s16 c2,
                         s16 *hist, s16 v1, s16 v2)
{
    int block_iters, block, i;
    s32 ch1_lo, ch1_hi, ch2_lo, ch2_hi;
    s32 hist_val;
    s16 scale1, scale2;
    s32 nib_hi1, nib_hi2;
    s32 pcm1a, pcm1b, pcm2a, pcm2b;
    s32 avg_a, avg_b;
    s32 tbl1, tbl2;

    block_iters = nblocks / 2;

    ch1_lo = prev1[0];
    ch1_hi = prev1[1];
    ch2_lo = prev2[0];
    ch2_hi = prev2[1];

    for (block = 0; block < block_iters; block++) {
        s16 header1 = *(s16 *)in;
        if (header1 & 0x8000)
            return block * 2;

        hist_val = hist[0];
        hist[0] = (s16)((v2 + hist_val * v1) & 0x7FFF);
        scale1 = (s16)(((header1 ^ hist_val) & 0x1FFF) + 1);

        s16 header2 = *(s16 *)(in + 0x12);
        if (header2 & 0x8000)
            return block * 2;

        hist_val = hist[0];
        hist[0] = (s16)((v2 + hist_val * v1) & 0x7FFF);
        scale2 = (s16)(((header2 ^ hist_val) & 0x1FFF) + 1);

        in += 2;

        for (i = 0; i < 16; i++) {
            s8 byte1 = (s8)in[0];
            s8 byte2 = (s8)in[0x12];
            in++;

            nib_hi1 = byte1 >> 4;
            nib_hi2 = byte2 >> 4;

            pcm1a = ((c1 * ch1_lo + c2 * ch1_hi) >> 12) + nib_hi1 * scale1;
            if ((u32)(pcm1a + 0x8000) > 0xFFFF) {
            if (pcm1a < -0x8000)
                pcm1a = -0x8000;
            else if (pcm1a > 0x7FFF)
                pcm1a = 0x7FFF;
        }

            pcm2a = ((c1 * ch2_lo + c2 * ch2_hi) >> 12) + nib_hi2 * scale2;
            if ((u32)(pcm2a + 0x8000) > 0xFFFF) {
            if (pcm2a < -0x8000)
                pcm2a = -0x8000;
            else if (pcm2a > 0x7FFF)
                pcm2a = 0x7FFF;
        }

            avg_a = (pcm1a + pcm2a) * 7 / 10;
            if ((u32)(avg_a + 0x8000) > 0xFFFF) {
            if (avg_a < -0x8000)
                avg_a = -0x8000;
            else if (avg_a > 0x7FFF)
                avg_a = 0x7FFF;
        }
            out1[0] = (s16)avg_a;
            out2[0] = (s16)avg_a;

            tbl1 = lbl_eu_80517428[byte1 & 0xF];
            pcm1b = ((c1 * pcm1a + c2 * ch1_lo) >> 12) + tbl1 * scale1;
            if ((u32)(pcm1b + 0x8000) > 0xFFFF) {
            if (pcm1b < -0x8000)
                pcm1b = -0x8000;
            else if (pcm1b > 0x7FFF)
                pcm1b = 0x7FFF;
        }

            tbl2 = lbl_eu_80517428[byte2 & 0xF];
            pcm2b = ((c1 * pcm2a + c2 * ch2_lo) >> 12) + tbl2 * scale2;
            if ((u32)(pcm2b + 0x8000) > 0xFFFF) {
            if (pcm2b < -0x8000)
                pcm2b = -0x8000;
            else if (pcm2b > 0x7FFF)
                pcm2b = 0x7FFF;
        }

            avg_b = (pcm1b + pcm2b) * 7 / 10;
            if ((u32)(avg_b + 0x8000) > 0xFFFF) {
            if (avg_b < -0x8000)
                avg_b = -0x8000;
            else if (avg_b > 0x7FFF)
                avg_b = 0x7FFF;
        }
            out1[1] = (s16)avg_b;
            out2[1] = (s16)avg_b;

            ch1_lo = pcm1b;
            ch1_hi = pcm1a;
            ch2_lo = pcm2b;
            ch2_hi = pcm2a;
            out1 += 2;
            out2 += 2;
        }

        in += 0x12;
    }

    prev1[0] = (s16)ch1_lo;
    prev1[1] = (s16)ch1_hi;
    prev2[0] = (s16)ch2_lo;
    prev2[1] = (s16)ch2_hi;
    return nblocks;
}

/* ADX version 4 stereo decoder, full stereo output */
int ADX_DecodeSte4AsSte(const u8 *in, int nblocks, s16 *out1, s16 *prev1,
                        s16 *out2, s16 *prev2, s16 c1, s16 c2,
                        s16 *hist, s16 v1, s16 v2)
{
    int block_iters, block, i;
    s32 ch1_lo, ch1_hi, ch2_lo, ch2_hi;
    s32 hist_val;
    s16 scale1, scale2;
    s32 nib_hi1, nib_hi2;
    s32 pcm1a, pcm1b, pcm2a, pcm2b;
    s32 tbl1, tbl2;

    block_iters = nblocks / 2;

    ch1_lo = prev1[0];
    ch1_hi = prev1[1];
    ch2_lo = prev2[0];
    ch2_hi = prev2[1];

    for (block = 0; block < block_iters; block++) {
        s16 header1 = *(s16 *)in;
        if (header1 & 0x8000)
            return block * 2;

        hist_val = hist[0];
        hist[0] = (s16)((v2 + hist_val * v1) & 0x7FFF);
        scale1 = (s16)(((header1 ^ hist_val) & 0x1FFF) + 1);

        s16 header2 = *(s16 *)(in + 0x12);
        if (header2 & 0x8000)
            return block * 2;

        hist_val = hist[0];
        hist[0] = (s16)((v2 + hist_val * v1) & 0x7FFF);
        scale2 = (s16)(((header2 ^ hist_val) & 0x1FFF) + 1);

        in += 2;

        for (i = 0; i < 16; i++) {
            s8 byte1 = (s8)in[0];
            s8 byte2 = (s8)in[0x12];
            in++;

            nib_hi1 = byte1 >> 4;
            nib_hi2 = byte2 >> 4;

            pcm1a = ((c1 * ch1_lo + c2 * ch1_hi) >> 12) + nib_hi1 * scale1;
            if ((u32)(pcm1a + 0x8000) > 0xFFFF) {
            if (pcm1a < -0x8000)
                pcm1a = -0x8000;
            else if (pcm1a > 0x7FFF)
                pcm1a = 0x7FFF;
        }
            out1[0] = (s16)pcm1a;

            pcm2a = ((c1 * ch2_lo + c2 * ch2_hi) >> 12) + nib_hi2 * scale2;
            if ((u32)(pcm2a + 0x8000) > 0xFFFF) {
            if (pcm2a < -0x8000)
                pcm2a = -0x8000;
            else if (pcm2a > 0x7FFF)
                pcm2a = 0x7FFF;
        }
            out2[0] = (s16)pcm2a;

            tbl1 = lbl_eu_80517428[byte1 & 0xF];
            pcm1b = ((c1 * pcm1a + c2 * ch1_lo) >> 12) + tbl1 * scale1;
            if ((u32)(pcm1b + 0x8000) > 0xFFFF) {
            if (pcm1b < -0x8000)
                pcm1b = -0x8000;
            else if (pcm1b > 0x7FFF)
                pcm1b = 0x7FFF;
        }
            out1[1] = (s16)pcm1b;

            tbl2 = lbl_eu_80517428[byte2 & 0xF];
            pcm2b = ((c1 * pcm2a + c2 * ch2_lo) >> 12) + tbl2 * scale2;
            if ((u32)(pcm2b + 0x8000) > 0xFFFF) {
            if (pcm2b < -0x8000)
                pcm2b = -0x8000;
            else if (pcm2b > 0x7FFF)
                pcm2b = 0x7FFF;
        }
            out2[1] = (s16)pcm2b;

            ch1_lo = pcm1b;
            ch1_hi = pcm1a;
            ch2_lo = pcm2b;
            ch2_hi = pcm2a;
            out1 += 2;
            out2 += 2;
        }

        in += 0x12;
    }

    prev1[0] = (s16)ch1_lo;
    prev1[1] = (s16)ch1_hi;
    prev2[0] = (s16)ch2_lo;
    prev2[1] = (s16)ch2_hi;
    return nblocks;
}

/* ADX version 4 stereo dispatcher
 * Checks global flag to choose mono mix or full stereo output */
int ADX_DecodeSte4(const u8 *in, int nblocks, s16 *out1, s16 *prev1,
                   s16 *out2, s16 *prev2, s16 c1, s16 c2,
                   s16 *hist, s16 v1, s16 v2)
{
    if (lbl_eu_805F2AF0 == 0) {
        return ADX_DecodeSte4AsSte(in, nblocks, out1, prev1,
            out2, prev2, c1, c2, hist, v1, v2);
    } else {
        return ADX_DecodeSte4AsMono(in, nblocks, out1, prev1,
            out2, prev2, c1, c2, hist, v1, v2);
    }
}
