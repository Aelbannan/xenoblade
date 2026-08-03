#include <harness_catalog.h>

void adxhdr_get_base_info(u8 *src, u8 *dst) {
    dst[0] = src[0];
    dst[1] = src[1];
    dst[2] = src[2];
    dst[3] = src[3];
    *(u32 *)(dst + 4) = ((u32)src[4] << 24) | ((u32)src[5] << 16) | ((u32)src[6] << 8) | src[7];
    *(u32 *)(dst + 8) = ((u32)src[8] << 24) | ((u32)src[9] << 16) | ((u32)src[10] << 8) | src[11];
    *(u16 *)(dst + 12) = (u16)((src[12] << 8) | src[13]);
    dst[0x3C] = src[0x0E];
    dst[0x3D] = src[0x0F];
}

void adxhdr_get_ply_prm(u8 *src, u8 *dst) {
    u16 tmp = (u16)((src[0] << 8) | src[1]);
    *(u16 *)(dst + 0) = tmp;
    if ((s16)tmp > 0) {
        *(s16 *)(dst + 2) = (s16)((src[2] << 8) | src[3]);
    }
    if ((s8)(dst[4] = src[4]) > 0) {
        dst[5] = src[5];
    }
    if ((s8)(dst[6] = src[6]) > 0) {
        dst[7] = src[7];
    }
    if ((s8)(dst[8] = src[8]) > 0) {
        dst[9] = src[9];
    }
    if ((s8)(dst[10] = src[10]) > 0) {
        dst[11] = src[11];
    }
}

int ADX_DecodeHeader(u8 *data, s32 size, s32 *out_data_size, u8 *out_hdr) {
    s32 remaining;
    s32 i, ch, ch_count;
    s32 ch_info_size;

    if (out_data_size) {
        *out_data_size = 0;
    }

    if (size < 4) return -1;

    /* check sync word 0x8000 */
    if ((u16)((data[0] << 8) | data[1]) != 0x8000) return -4;

    /* header size */
    ch_info_size = (data[2] << 8) | data[3];
    if (out_data_size) *out_data_size = ch_info_size;

    if (!out_hdr) return 0;

    if (size < ch_info_size + 4) return -2;
    if (ch_info_size - 6 < 16) return -2;

    adxhdr_get_base_info(data + 4, out_hdr);

    /* parse channel info from offset 0x14 in src */
    i = 0;
    remaining = ch_info_size - 0x16;
    {
        u8 *p = data + 0x14;
        if (out_hdr[0x3C] == 4) {
            p += 0x0C;
            remaining -= 0x0C;
        }

        if (remaining < 4) return 0;

        /* number of channel entries */
        ch_count = (p[2] << 8) | p[3];
        *(s16 *)(out_hdr + 0x0E) = (s16)ch_count;
        p += 4;
        remaining -= 4;

        if (remaining < ch_count * 0x14) return 0;

        for (i = 0; i < ch_count; i++) {
            u8 *ch_dst = out_hdr + 0x10 + i * 0x14;
            *(u16 *)(ch_dst + 0) = (u16)((p[0] << 8) | p[1]);
            *(u16 *)(ch_dst + 2) = (u16)((p[2] << 8) | p[3]);
            *(u32 *)(ch_dst + 4) = ((u32)p[4] << 24) | ((u32)p[5] << 16) | ((u32)p[6] << 8) | p[7];
            *(u32 *)(ch_dst + 8) = ((u32)p[8] << 24) | ((u32)p[9] << 16) | ((u32)p[10] << 8) | p[11];
            *(u32 *)(ch_dst + 12) = ((u32)p[12] << 24) | ((u32)p[13] << 16) | ((u32)p[14] << 8) | p[15];
            *(u32 *)(ch_dst + 16) = ((u32)p[16] << 24) | ((u32)p[17] << 16) | ((u32)p[18] << 8) | p[19];
            p += 0x14;
        }

        /* parse play parameters */
        for (ch = 0; ch < (s8)out_hdr[3]; ch++) {
            if (remaining < 12) return 0;
            adxhdr_get_ply_prm(p, out_hdr + 0x24 + ch * 12);
            p += 12;
            remaining -= 12;
        }
    }

    return 0;
}

