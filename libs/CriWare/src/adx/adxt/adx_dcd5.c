#include <harness_catalog.h>

extern s32 lbl_eu_80517428[];
extern s32 lbl_eu_805F2AF0;

/* ADX version 4 mono decoder */
int ADX_DecodeMono4(const u8 *in, int nblocks, s16 *out,
    s16 *prev, s16 c1, s16 c2, s16 *hist, s16 scale)
{
    int block, i;
    s32 prev_lo, prev_hi;
    s16 header;
    s32 hist_val, pred, scale_idx, scale_val;
    s8 byte;
    s32 nib_hi, nib_lo;
    s32 pcm, pcm2;
    s32 table_val;

    prev_lo = prev[0];
    prev_hi = prev[1];

    for (block = 0; block < nblocks; block++) {
        header = (s16)((in[0] << 8) | in[1]);
        if (header & 0x8000)
            return block;

        hist_val = hist[0];
        pred = prev_hi + (hist_val * scale);
        pred &= 0xFFFE;
        hist[0] = (s16)pred;

        scale_idx = ((u32)(header ^ (s32)hist_val) & 0x1F) + 1;
        scale_val = (s16)scale_idx;
        if (scale_val == 0) scale_val = 1;

        in += 2;

        for (i = 0; i < 16; i++) {
            byte = (s8)*in++;
            nib_hi = byte >> 4;
            nib_lo = byte & 0xF;

            pcm = c1 * prev_lo + c2 * prev_hi + nib_hi * scale_val;
            pcm >>= 12;
            if (pcm > 0x7FFF) pcm = 0x7FFF;
            else if (pcm < -0x8000) pcm = -0x8000;
            out[0] = (s16)pcm;

            table_val = lbl_eu_80517428[(u32)(nib_hi << 2)];
            pcm2 = c1 * pcm + c2 * prev_lo + (u32)(nib_lo & 0xF) * scale_val;
            pcm2 >>= 12;
            if (pcm2 > 0x7FFF) pcm2 = 0x7FFF;
            else if (pcm2 < -0x8000) pcm2 = -0x8000;
            out[1] = (s16)pcm2;

            prev_lo = pcm;
            prev_hi = pcm2;
            c1 = table_val;
            table_val = lbl_eu_80517428[((u32)nib_lo << 2)];
            out += 2;
        }
    }

    prev[0] = (s16)prev_lo;
    prev[1] = (s16)prev_hi;
    return nblocks;
}

/* ADX version 4 stereo decoder, mixed to mono
 * Decodes both channels, averages each pair: out = (ch1 + ch2) / 2 */
int ADX_DecodeSte4AsMono(const u8 *in, int nblocks, s16 *out1, s16 *prev1,
    s16 *out2, s16 *prev2, s16 c1, s16 c2,
    void *ext, s16 v1, s16 v2)
{
    int block_iters, block, i;
    s32 ch1_lo, ch1_hi, ch2_lo, ch2_hi;
    s32 ch1_scale_lo, ch1_scale_hi;
    s16 header1, header2;
    s32 pred1, pred2;
    s32 scale_val1, scale_val2;
    s8 byte1, byte2;
    s32 nib_hi1, nib_lo1, nib_hi2, nib_lo2;
    s32 pcm1a, pcm1b, pcm2a, pcm2b;
    s32 avg_a, avg_b;
    s32 tbl1, tbl2;

    block_iters = (nblocks + 1) >> 1;  /* ceil(nblocks/2) */

    ch1_lo = prev1[0];
    ch1_hi = prev1[1];
    ch2_lo = prev2[0];
    ch2_hi = prev2[1];

    for (block = 0; block < block_iters; block++) {
        header1 = (s16)((in[0] << 8) | in[1]);
        header2 = (s16)((in[2] << 8) | in[3]);

        if ((header1 & 0x8000) || (header2 & 0x8000))
            return block;

        /* scale from header XOR with history */
        scale_val1 = ((u32)(header1 ^ (s32)ch1_hi) & 0x1F) + 1;
        if ((s16)scale_val1 == 0) scale_val1 = 1;
        scale_val2 = ((u32)(header2 ^ (s32)ch2_hi) & 0x1F) + 1;
        if ((s16)scale_val2 == 0) scale_val2 = 1;

        in += 4;

        for (i = 0; i < 16; i++) {
            byte1 = (s8)*in++;
            byte2 = (s8)*in++;

            nib_hi1 = byte1 >> 4;
            nib_lo1 = byte1 & 0xF;
            nib_hi2 = byte2 >> 4;
            nib_lo2 = byte2 & 0xF;

            /* Channel 1 sample A */
            pcm1a = c1 * ch1_lo + c2 * ch1_hi + nib_hi1 * scale_val1;
            pcm1a >>= 12;
            if (pcm1a > 0x7FFF) pcm1a = 0x7FFF;
            else if (pcm1a < -0x8000) pcm1a = -0x8000;

            /* Channel 2 sample A */
            pcm2a = c1 * ch2_lo + c2 * ch2_hi + nib_hi2 * scale_val2;
            pcm2a >>= 12;
            if (pcm2a > 0x7FFF) pcm2a = 0x7FFF;
            else if (pcm2a < -0x8000) pcm2a = -0x8000;

            /* Average: (ch1 + ch2) * 7 / 10 */
            avg_a = (s32)(((s64)(pcm1a + pcm2a) * 7) / 10);

            /* Channel 1 sample B */
            tbl1 = lbl_eu_80517428[(u32)(nib_hi1 << 2)];
            pcm1b = c1 * pcm1a + c2 * ch1_lo + (u32)(nib_lo1 & 0xF) * scale_val1;
            pcm1b >>= 12;
            if (pcm1b > 0x7FFF) pcm1b = 0x7FFF;
            else if (pcm1b < -0x8000) pcm1b = -0x8000;

            /* Channel 2 sample B */
            tbl2 = lbl_eu_80517428[(u32)(nib_hi2 << 2)];
            pcm2b = c1 * pcm2a + c2 * ch2_lo + (u32)(nib_lo2 & 0xF) * scale_val2;
            pcm2b >>= 12;
            if (pcm2b > 0x7FFF) pcm2b = 0x7FFF;
            else if (pcm2b < -0x8000) pcm2b = -0x8000;

            /* Average: (ch1 + ch2) * 7 / 10 */
            avg_b = (s32)(((s64)(pcm1b + pcm2b) * 7) / 10);

            out1[0] = (s16)avg_a;
            out1[1] = (s16)avg_b;
            out2[0] = (s16)avg_a;
            out2[1] = (s16)avg_b;

            ch1_lo = pcm1a;
            ch1_hi = pcm1b;
            ch2_lo = pcm2a;
            ch2_hi = pcm2b;
            c1 = tbl1;
            out1 += 2;
            out2 += 2;
        }
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
    void *ext, s16 v1, s16 v2)
{
    int block_iters, block, i;
    s32 ch1_lo, ch1_hi, ch2_lo, ch2_hi;
    s16 header1, header2;
    s32 scale_val1, scale_val2;
    s8 byte1, byte2;
    s32 nib_hi1, nib_lo1, nib_hi2, nib_lo2;
    s32 pcm1a, pcm1b, pcm2a, pcm2b;
    s32 tbl1, tbl2;

    block_iters = (nblocks + 1) >> 1;

    ch1_lo = prev1[0];
    ch1_hi = prev1[1];
    ch2_lo = prev2[0];
    ch2_hi = prev2[1];

    for (block = 0; block < block_iters; block++) {
        header1 = (s16)((in[0] << 8) | in[1]);
        header2 = (s16)((in[2] << 8) | in[3]);

        if ((header1 & 0x8000) || (header2 & 0x8000))
            return block;

        scale_val1 = ((u32)(header1 ^ (s32)ch1_hi) & 0x1F) + 1;
        if ((s16)scale_val1 == 0) scale_val1 = 1;
        scale_val2 = ((u32)(header2 ^ (s32)ch2_hi) & 0x1F) + 1;
        if ((s16)scale_val2 == 0) scale_val2 = 1;

        in += 4;

        for (i = 0; i < 16; i++) {
            byte1 = (s8)*in++;
            byte2 = (s8)*in++;

            nib_hi1 = byte1 >> 4;
            nib_lo1 = byte1 & 0xF;
            nib_hi2 = byte2 >> 4;
            nib_lo2 = byte2 & 0xF;

            pcm1a = c1 * ch1_lo + c2 * ch1_hi + nib_hi1 * scale_val1;
            pcm1a >>= 12;
            if (pcm1a > 0x7FFF) pcm1a = 0x7FFF;
            else if (pcm1a < -0x8000) pcm1a = -0x8000;

            pcm2a = c1 * ch2_lo + c2 * ch2_hi + nib_hi2 * scale_val2;
            pcm2a >>= 12;
            if (pcm2a > 0x7FFF) pcm2a = 0x7FFF;
            else if (pcm2a < -0x8000) pcm2a = -0x8000;

            tbl1 = lbl_eu_80517428[(u32)(nib_hi1 << 2)];
            pcm1b = c1 * pcm1a + c2 * ch1_lo + (u32)(nib_lo1 & 0xF) * scale_val1;
            pcm1b >>= 12;
            if (pcm1b > 0x7FFF) pcm1b = 0x7FFF;
            else if (pcm1b < -0x8000) pcm1b = -0x8000;

            tbl2 = lbl_eu_80517428[(u32)(nib_hi2 << 2)];
            pcm2b = c1 * pcm2a + c2 * ch2_lo + (u32)(nib_lo2 & 0xF) * scale_val2;
            pcm2b >>= 12;
            if (pcm2b > 0x7FFF) pcm2b = 0x7FFF;
            else if (pcm2b < -0x8000) pcm2b = -0x8000;

            out1[0] = (s16)pcm1a;
            out1[1] = (s16)pcm1b;
            out2[0] = (s16)pcm2a;
            out2[1] = (s16)pcm2b;

            ch1_lo = pcm1a;
            ch1_hi = pcm1b;
            ch2_lo = pcm2a;
            ch2_hi = pcm2b;
            c1 = tbl1;
            out1 += 2;
            out2 += 2;
        }
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
    void *ext, s16 v1, s16 v2)
{
    if (lbl_eu_805F2AF0 == 0) {
        return ADX_DecodeSte4AsSte(in, nblocks, out1, prev1,
            out2, prev2, c1, c2, ext, v1, v2);
    } else {
        return ADX_DecodeSte4AsMono(in, nblocks, out1, prev1,
            out2, prev2, c1, c2, ext, v1, v2);
    }
}
