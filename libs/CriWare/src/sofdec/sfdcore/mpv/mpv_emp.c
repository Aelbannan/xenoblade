#include <harness_catalog.h>

/* MPEG start-code type lookup (16-bit entries: low byte = bit length of the
 * value read at index `count`, high byte = the expected value). */
extern s16 lbl_eu_8051C190[];

extern int MPV_CheckDelim(const u8 *buf);
extern const u8 *MPV_SearchDelim(const u8 *start, int count, int flags);

/* Check that a B-picture region contains no coded macroblocks.
 *
 * `data`/`size` bound the picture payload; `flags` is the number of
 * macroblocks to tolerate as empty.  Bitstream is read MSB-first using a
 * two-word carry register (`cur` holds the unread bits at the top, `next` is
 * the prefetched following word, `bitpos` counts consumed bits in `cur`).
 */
int MPV_IsEmptyBpic(const u8 *data, int size, int flags) {
    u32 d = (u32)data;
    const u32 *p = (const u32 *)(d & ~3u);
    int bitpos = (int)((d - (u32)p) << 3);
    u32 w1 = p[1];
    u32 cur;
    u32 first = p[0] << bitpos;

    /* First 32 bits: slice_start_code | vertical_position == 0x101 */
    if (bitpos != 0) {
        cur = w1 << bitpos;
        first |= w1 >> (32 - bitpos);
    } else {
        cur = w1;
    }
    u32 next = p[2];
    p += 3;
    if (first != 0x101) return 0;

    /* quantiser_scale_code (5 bits) */
    if (bitpos >= 27) {
        bitpos -= 27;
        cur = next;
        if (bitpos != 0) cur = next << bitpos;
        next = *p++;
    } else {
        cur <<= 5;
        bitpos += 5;
    }

    /* extra_bit_slice (1 bit) must be 0 */
    {
        u32 bit = cur >> 31;
        if (bitpos == 31) { cur = next; next = *p++; bitpos = 0; }
        else { cur <<= 1; bitpos += 1; }
        if (bit != 0) return 0;
    }

    /* second flag bit must be 1 */
    {
        u32 bit = cur >> 31;
        if (bitpos == 31) { cur = next; next = *p++; bitpos = 0; }
        else { cur <<= 1; bitpos += 1; }
        if (bit == 0) return 0;
    }

    /* macroblock_type of first block (peek 6 bits) */
    {
        u32 v6 = cur >> 26;
        if (bitpos > 26) v6 |= next >> (58 - bitpos);
        u32 d22 = v6 - 22u;
        if (d22 <= 1u) {
            bitpos += 5;
            if (bitpos >= 32) { bitpos -= 32; cur = next << bitpos; next = *p++; }
            else cur <<= 5;
        } else if (v6 == 11u) {
            bitpos += 6;
            if (bitpos >= 32) { bitpos -= 32; cur = next << bitpos; next = *p++; }
            else cur <<= 6;
        } else {
            return 0;
        }
    }

    /* skip a run of "empty" macroblock codes */
    {
        int m = flags - 1;
        do {
            u32 v11 = cur >> 21;
            if (bitpos > 21) v11 |= next >> (53 - bitpos);
            if (v11 != 8u) break;
            bitpos += 11;
            if (bitpos >= 32) { bitpos -= 32; cur = next << bitpos; next = *p++; /* refill */ }
            else cur <<= 11;
            m -= 33;
        } while (m > 33);
        if ((u32)(m - 1) > 32u) return 0;

        /* read the trailing dynamic-length code from the lookup */
        {
            int tc = ((s16 *)lbl_eu_8051C190)[m];
            int rb = tc & 0xFF;
            int len = 32 - rb;
            u32 val;
            if (bitpos >= len) {
                bitpos -= len;
                if (bitpos != 0) {
                    cur |= next >> (rb - bitpos);
                    val = cur >> len;
                    cur = next << bitpos;
                } else {
                    val = cur >> len;
                    cur = next;
                }
                next = *p++;
            } else {
                val = cur >> len;
                bitpos += rb;
                cur <<= rb;
            }
            if (val != ((u32)tc >> 8)) return 0;
        }
    }

    /* one more macroblock_type (peek 6 bits). `cur`/`next` are dead after
     * this point: only the bit position and read pointer advance. */
    {
        u32 v6 = cur >> 26;
        if (bitpos > 26) v6 |= next >> (58 - bitpos);
        switch (v6) {
        case 22:
        case 23:
            bitpos += 5;
            if (bitpos >= 32) { bitpos -= 32; p++; }
            break;
        case 11:
            bitpos += 6;
            if (bitpos >= 32) { bitpos -= 32; p++; }
            break;
        default:
            return 0;
        }
    }

    /* byte offset of the next unconsumed bit must stay within `size` */
    {
        int consumed = (int)(((const u8 *)p) + ((bitpos + 7) >> 3) - 8 - data);
        return consumed <= size;
    }
}

/* Check that a P-picture region is empty, then walk the following start codes
 * until the picture/sequence boundary expected of an empty region.
 */
int MPV_IsEmptyPpic(const u8 *data, int size, int flags) {
    u32 d = (u32)data;
    const u32 *p = (const u32 *)(d & ~3u);
    int bitpos = (int)((d - (u32)p) << 3);
    u32 w1 = p[1];
    u32 cur;
    u32 first = p[0] << bitpos;

    /* First 32 bits: slice_start_code | vertical_position == 0x101 */
    if (bitpos != 0) {
        cur = w1 << bitpos;
        first |= w1 >> (32 - bitpos);
    } else {
        cur = w1;
    }
    u32 next = p[2];
    p += 3;
    if (first != 0x101) return 0;

    /* quantiser_scale_code (5 bits) */
    if (bitpos >= 27) {
        bitpos -= 27;
        cur = next;
        if (bitpos != 0) cur = next << bitpos;
        next = *p++;
    } else {
        cur <<= 5;
        bitpos += 5;
    }

    /* extra_bit_slice (1 bit) must be 0 */
    {
        u32 bit = cur >> 31;
        if (bitpos == 31) { cur = next; next = *p++; bitpos = 0; }
        else { cur <<= 1; bitpos += 1; }
        if (bit != 0) return 0;
    }

    /* second flag bit must be 1 */
    {
        u32 bit = cur >> 31;
        if (bitpos == 31) { cur = next; next = *p++; bitpos = 0; }
        else { cur <<= 1; bitpos += 1; }
        if (bit == 0) return 0;
    }

    /* macroblock_type of first block (peek 5 bits) must be 7 */
    {
        u32 v5 = cur >> 27;
        if (bitpos > 27) v5 |= next >> (59 - bitpos);
        if (v5 != 7u) return 0;
        bitpos += 5;
        if (bitpos >= 32) { bitpos -= 32; cur = next << bitpos; next = *p++; }
        else cur <<= 5;
    }

    /* skip a run of "empty" macroblock codes */
    {
        int m = flags - 1;
        for (;;) {
            u32 v11 = cur >> 21;
            if (bitpos > 21) v11 |= next >> (53 - bitpos);
            if (v11 != 8u) break;
            bitpos += 11;
            if (bitpos >= 32) { bitpos -= 32; cur = next << bitpos; next = *p++; }
            else cur <<= 11;
            m -= 33;
            if (m <= 33) break;
        }
        if ((u32)(m - 1) > 32u) return 0;

        /* read the trailing dynamic-length code from the lookup */
        {
            int tc = ((s16 *)lbl_eu_8051C190)[m];
            int rb = tc & 0xFF;
            int len = 32 - rb;
            u32 val;
            if (bitpos >= len) {
                bitpos -= len;
                if (bitpos != 0) {
                    cur |= next >> (rb - bitpos);
                    val = cur >> len;
                    cur = next << bitpos;
                } else {
                    val = cur >> len;
                    cur = next;
                }
                next = *p++;
            } else {
                val = cur >> len;
                bitpos += rb;
                cur <<= rb;
            }
            if (val != ((u32)tc >> 8)) return 0;
        }
    }

    /* second macroblock_type (peek 5 bits) must be 7 */
    {
        u32 v5 = cur >> 27;
        if (bitpos > 27) v5 |= next >> (59 - bitpos);
        if (v5 != 7u) return 0;
        bitpos += 5;
        if (bitpos >= 32) { bitpos -= 32; cur = next << bitpos; next = *p++; }
        else cur <<= 5;
    }

    /* current bit position in bytes, then walk following start codes */
    {
        const u8 *addr = ((const u8 *)p) + ((bitpos + 7) >> 3) - 8;
        int consumed = (int)(addr - data);
        if (consumed > size) return 0;
        for (;;) {
            const u8 *d = (const u8 *)MPV_SearchDelim(addr, size - consumed, 0xcc);
            int cr;
            if (d == 0) return 0;
            cr = MPV_CheckDelim(d);
            if (cr & 0x4) {
                /* picture_start: keep walking while it is an extension-free,
                 * single-slice picture */
                u32 x = ((u32)(d[6] >> 7) << 24) | (u32)((d[5] >> 6) & 3);
                if (x == 3) {
                    addr = d + 1;
                    consumed = (int)(addr - data);
                    continue;
                }
                return 0;
            }
            if (cr & 0x40) {
                /* sequence_header: skip its 4-byte code and keep walking */
                addr = d + 1;
                consumed = (int)(addr - data);
                continue;
            }
            if (cr & 0x80) {
                /* sequence_end: region ends here if it fits */
                if ((int)((d + 4) - data) <= size) return 1;
                return 0;
            }
            if (cr & 0x8) {
                /* group_start: 7-byte code, but only if its extension bit is set */
                if ((d[7] & 0x40) != 0) {
                    if ((int)((d + 7) - data) <= size) return 1;
                }
                return 0;
            }
            return 0;
        }
    }
}