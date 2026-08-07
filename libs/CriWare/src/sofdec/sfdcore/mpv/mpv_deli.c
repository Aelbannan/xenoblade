#include <harness_catalog.h>
#include <string.h>

extern u8 lbl_eu_8051C090[];

/* Build MPEG start code type lookup table */
void criware_803A5908(u8 *table) {
    int i;
    for (i = 0; i < 256; i++) {
        u8 type;
        u32 b = i | 0x100;
        if (b == 0x100) {
            type = 0x04; /* picture_start_code */
        } else if (b == 0x101) {
            type = 0x03; /* slice_start_code */
        } else if (b - 0x102 <= 0xAD) {
            type = 0x01; /* slice codes 0x102..0x1AF */
        } else if (b == 0x1B2) {
            type = 0x20; /* user_data_start_code */
        } else if (b == 0x1B3) {
            type = 0x40; /* sequence_header_code */
        } else if (b == 0x1B5) {
            type = 0x10; /* extension_start_code */
        } else if (b == 0x1B7) {
            type = 0x80; /* sequence_end_code */
        } else {
            type = 0;
            if (b == 0x1B8)
                type = 0x08; /* group_start_code */
        }
        table[i] = type;
    }
}

int criware_803A59B0(void) {
    u8 table[256];
    criware_803A5908(table);
    if (memcmp(table, lbl_eu_8051C090, 256) != 0)
        return -1;
    return 0;
}

/* Check if buf starts with MPEG start code prefix 0x000001 */
int MPV_CheckDelim(const u8 *buf) {
    int code;
    code = buf[0];
    code <<= 8;
    code |= buf[1];
    code <<= 8;
    code |= buf[2];
    if (code != 1) return 0;
    return lbl_eu_8051C090[buf[3]];
}

/* Search backward for MPEG start code (binary search style) */
const u8 *MPV_BsearchDelim(const u8 *end, int count, int flags) {
    const u8 *p = end - 1;
    int i;
    u32 state = 0xFFFFFF00;

    for (i = 0; i < count; i++) {
        const u8 *q = p - i;
        u8 byte = *q;
        state = state | byte;
        u32 check = state << 8;
        if (check == 0x01000000) {
            u8 type = lbl_eu_8051C090[state >> 24];
            if (flags & type) {
                return q;
            }
        }
        state = check;
    }
    return NULL;
}

/* Search forward for MPEG start code */
const u8 *MPV_SearchDelim(const u8 *start, int count, int flags) {
    int i;
    const u8 *q;
    u32 byte;
    u32 state;
    state = 0xFFFFFF00;
    for (i = 0; i < count; i++) {
        q = start + i;
        byte = *q;
        if (state == 0x100) {
            if (flags & lbl_eu_8051C090[byte]) {
                return q - 3;
            }
        }
        state = (state | byte) << 8;
    }
    return NULL;
}
