#include <harness_catalog.h>

extern int MPVLIB_CheckHn(void *);
extern void MPVERR_SetCode(void *, int);

/* Copy picture attributes from handle to output buffer */
int MPV_GetPicAtr(void *handle, u32 *out) {
    u32 *src;
    u32 *dst;
    int i;

    if (MPVLIB_CheckHn(handle)) {
        MPVERR_SetCode(NULL, 0xFF03020C);
        return 0;
    }

    src = (u32 *)((u8 *)handle + 0xB58);
    dst = out + 4;
    for (i = 0; i < 16; i++) {
        u32 v0, v1;
        v0 = src[1];
        v1 = *src;
        dst[1] = v0;
        *dst = v1;
        src += 2;
        dst += 2;
    }
    return 0;
}

/* Get bitrate from handle */
int MPV_GetBitRate(void *handle, u32 *out) {
    if (MPVLIB_CheckHn(handle)) {
        MPVERR_SetCode(NULL, 0xFF03020D);
        return 0;
    }

    *out = *(u32 *)((u8 *)handle + 0xC48);
    return 0;
}

/* Get VBV buffer size */
int MPV_GetVbvBufSiz(void *handle, u32 *out_size, u32 *out_avg, u32 *out_max) {
    if (MPVLIB_CheckHn(handle)) {
        MPVERR_SetCode(NULL, 0xFF03020F);
        return 0;
    }

    *out_size = *(u32 *)((u8 *)handle + 0xC4C) << 11;
    *out_avg = *(u32 *)((u8 *)handle + 0xC5C);
    
    {
        u32 bitrate = *(u32 *)((u8 *)handle + 0xC48);
        if ((bitrate & 0xFFFFFC00) == 0x300) {
            *out_max = (u32)-1;
        } else {
            u32 vbv = *(u32 *)((u8 *)handle + 0xC5C);
            *out_max = (u32)((s32)((s64)vbv * (s64)bitrate * (s64)0x91A3B3C5 >> 42));
        }
    }
    return 0;
}

/* Get link flags */
int MPV_GetLinkFlg(void *handle, u32 *out_prev, u32 *out_next) {
    if (MPVLIB_CheckHn(handle)) {
        MPVERR_SetCode(NULL, 0xFF03020E);
        return 0;
    }

    *out_prev = *(u32 *)((u8 *)handle + 0xC54);
    *out_next = *(u32 *)((u8 *)handle + 0xC58);
    return 0;
}
