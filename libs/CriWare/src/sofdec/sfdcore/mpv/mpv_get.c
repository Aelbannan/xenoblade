#include <harness_catalog.h>

extern int MPVLIB_CheckHn(void*);
extern int MPVERR_SetCode(void*, int);

/* Sofdec MPV handle fields accessed by the MPV_Get* accessors. */
typedef struct MpfGetHd {
    u8 _00[0xB58];
    u32 picAtr[32];   /* 0xB58 */
    u8 _B58end[0xC48 - 0xBD8];
    u32 bitRate;      /* 0xC48 */
    u32 vbvBufSiz;    /* 0xC4C */
    u8 _C50[0x54 - 0x50];
    u32 linkFlg[2];   /* 0xC54, 0xC58 */
    u32 frameRate;    /* 0xC5C */
} MpfGetHd;

/* Copy picture attributes from handle to output buffer */
int MPV_GetPicAtr(void *handle, u32 *out) {
    if (MPVLIB_CheckHn(handle)) {
        return MPVERR_SetCode(NULL, 0xFF03020C);
    }

    {
        u32 *s = (u32 *)((u8 *)handle + 0xB58);
        u32 *d = out - 1;
        u32 n = 16;
        do {
            u32 v0 = *(s + 1);
            u32 v1 = *(s += 2);
            *(d + 1) = v0;
            *(d += 2) = v1;
        } while (--n != 0);
    }
    return 0;
}

/* Get bitrate from handle */
int MPV_GetBitRate(void* handle, u32* out) {
    MpfGetHd* h = (MpfGetHd*)handle;
    if (MPVLIB_CheckHn(h)) {
        return MPVERR_SetCode(NULL, 0xFF03020D);
    }
    *out = h->bitRate;
    return 0;
}

/* Get VBV buffer size */
int MPV_GetVbvBufSiz(void *handle, u32 *out_size, u32 *out_avg, u32 *out_max) {
    MpfGetHd *h = (MpfGetHd *)handle;
    if (MPVLIB_CheckHn(h)) {
        return MPVERR_SetCode(NULL, 0xFF03020F);
    }

    *out_size = h->vbvBufSiz << 11;
    *out_avg = h->frameRate;

    {
        u32 bitRate = h->bitRate;
        if ((u32)(bitRate - 0x30000) == 0xFFFF) {
            *out_max = (u32)-1;
        } else {
            /* 410 avg bitrate = (frameRate * bitRate * 0x91A2B3C5) >> 42, rounded */
            s32 m = (s32)h->frameRate * (s32)bitRate;
            *out_max = (u32)(((__mulhw((s32)0x91A2B3C5, m) + m) >> 10))
                     + ((u32)(((__mulhw((s32)0x91A2B3C5, m) + m) >> 10)) >> 31);
        }
    }
    return 0;
}

/* Get link flags */
int MPV_GetLinkFlg(void *handle, u32 *out_prev, u32 *out_next) {
    u8* h = (u8*)handle;
    if (MPVLIB_CheckHn(h)) {
        return MPVERR_SetCode(NULL, 0xFF03020E);
    }
    *out_next = *(u32 *)(h + 0xC58);
    *out_prev = *(u32 *)(h + 0xC54);
    return 0;
}
