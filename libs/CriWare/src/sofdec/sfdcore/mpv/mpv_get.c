#include <harness_catalog.h>

extern int MPVLIB_CheckHn(void *);
extern int MPVERR_SetCode(void *, int);

/* Copy picture attributes from handle to output buffer */
int MPV_GetPicAtr(void *handle, u32 *out) {
    u32 *src;
    u32 *dst;
    int i;

    if (MPVLIB_CheckHn(handle)) {
        return MPVERR_SetCode(NULL, 0xFF03020C);
    }

    src = (u32 *)((u8 *)handle + 0xB58);
    dst = out;
    do {
        u32 v0 = src[1];
        u32 v1 = *src;
        src += 2;
        *dst = v0;
        dst[1] = v1;
        dst += 2;
        i--;
    } while (i != 0);
    return 0;
}

/* Get bitrate from handle */
int MPV_GetBitRate(void *handle, u32 *out) {
    u32* o = out;
    void* h = handle;
    if (MPVLIB_CheckHn(h)) {
        return MPVERR_SetCode(NULL, 0xFF03020D);
    }
    *o = *(u32 *)((u8 *)h + 0xC48);
    return 0;
}

/* Get VBV buffer size */
int MPV_GetVbvBufSiz(void *handle, u32 *out_size, u32 *out_avg, u32 *out_max) {
    if (MPVLIB_CheckHn(handle)) {
        return MPVERR_SetCode(NULL, 0xFF03020F);
    }

    *out_size = *(u32 *)((u8 *)handle + 0xC4C) << 11;
    *out_avg = *(u32 *)((u8 *)handle + 0xC5C);
    
    {
        u32 bitrate = *(u32 *)((u8 *)handle + 0xC48);
        if ((u32)(bitrate - 0x30000) <= 0xFFFF) {
            *out_max = (u32)-1;
        } else {
            u32 vbv = *(u32 *)((u8 *)handle + 0xC5C);
            u32 m = vbv * bitrate;
            u32 hi = (u32)(((u64)0x91A3B3C5 * m) >> 32);
            u32 x = (hi + m) >> 10;
            *out_max = x + ((x << 1) & 0x80000000);
        }
    }
    return 0;
}

/* Get link flags */
int MPV_GetLinkFlg(void *handle, u32 *out_prev, u32 *out_next) {
    if (MPVLIB_CheckHn(handle)) {
        return MPVERR_SetCode(NULL, 0xFF03020E);
    }

    *out_prev = *(u32 *)((u8 *)handle + 0xC54);
    *out_next = *(u32 *)((u8 *)handle + 0xC58);
    return 0;
}
