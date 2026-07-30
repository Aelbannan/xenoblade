#include <harness_catalog.h>
#include <string.h>

extern u8 lbl_eu_8051C090[];

/* Build MPEG start code type lookup table */
void criware_803A5908(u8 *table) {
    int i;
    for (i = 0; i < 256; i++) {
        u8 type = 0;
        u8 b = (u8)(i | 0x100);
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
        } else if (b == 0x1B8) {
            type = 0x08; /* group_start_code */
        }
        table[i] = type;
    }
}

int criware_803A59B0(void) {
    u8 table[256];
    criware_803A5908(table);
    return memcmp(table, lbl_eu_8051C090, 256) ? -1 : 0;
}

/* Check if buf starts with MPEG start code prefix 0x000001 */
int MPV_CheckDelim(const u8 *buf) {
    u32 code = ((u32)buf[0] << 16) | ((u32)buf[1] << 8) | buf[2];
    if (code != 0x000001) return 0;
    return lbl_eu_8051C090[buf[3]];
}

/* Search backward for MPEG start code (binary search style) */
const u8 *MPV_BsearchDelim(const u8 *end, int count, int flags) {
    int i;
    u32 state = 0xFFFFFF00;
    const u8 *p = end - 1;

    for (i = 0; i < count; i++) {
        u8 byte = *p--;
        state = (state | byte) << 8;
        if ((state & 0xFFFFFF00) == 0x00000100) {
            u8 type = lbl_eu_8051C090[(state >> 24) & 0xFF];
            if (type & flags) {
                return p + 1;
            }
        }
    }
    return NULL;
}

/* Search forward for MPEG start code */
const u8 *MPV_SearchDelim(const u8 *start, int count, int flags) {
    int i;
    u32 state = 0xFFFFFF00;

    for (i = 0; i < count; i++) {
        u8 byte = start[i];
        if (state == 0x00000100) {
            u8 type = lbl_eu_8051C090[byte];
            if (type & flags) {
                return &start[i - 3];
            }
        }
        state = ((state | byte) << 8) & 0xFFFFFF00;
    }
    return NULL;
}
