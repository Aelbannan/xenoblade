#include <harness_catalog.h>

/* SofDec MPV decoder handle accessors.
 *
 * These read fields out of the MPV decoder handle block. Only the fields
 * touched by the retail accessors are modelled; everything else stays as
 * anonymous padding so the struct layout (and thus every load/store
 * offset) matches the original library byte for byte.
 */
extern int MPVLIB_CheckHn(void*);
extern int MPVERR_SetCode(void*, int);

typedef struct MpfGetHd {
    u8 _00[0xB58];
    u8  picAtrBytes[0x80];   /* 0xB58 - kept as bytes so the copy loop is not
                           seen as a u32 array copy (retail loop is a
                           counted mtctr/bdnz 1x copy, not unrolled) */
    u8 _B58end[0xC48 - 0xBD8];
    u32 bitRate;      /* 0xC48 */
    u32 vbvBufSiz;    /* 0xC4C */
    u8 _C50[0x54 - 0x50];
    u32 linkFlg[2];   /* 0xC54, 0xC58 */
    u32 frameRate;    /* 0xC5C */
} MpfGetHd;

/* Copy picture attributes from handle to output buffer */
#pragma push
#pragma optimize_for_size on
int MPV_GetPicAtr(void *handle, u32 *out) {
    MpfGetHd *mh = (MpfGetHd *)handle;   // local handle copy: lives across the
                                         // check call, so MWCC parks it in r31
                                         // (retail reg split)
    if (MPVLIB_CheckHn(mh)) {
        return MPVERR_SetCode(NULL, 0xFF03020C);
    }

    // Retail loop is a counted mtctr/bdnz 1x loop: the `for (n = 16; n != 0;
    // n--)` form + whole-function #pragma optimize_for_size on (plain -O4,p
    // unrolls 8x; the do-while form emits addic./bne; a mid-function pragma
    // does not suppress the unroll).
    {
        u32 v1, v0;             // temporaries declared first so they color
                                // into r3/r0 like retail
        u32 *s;                 // s declared before d so s takes r4, d takes r5
        u32 *d = out - 1;
        u32 n;
        s = (u32 *)mh->picAtrBytes;
        for (n = 16; n != 0; n--) {
            v0 = *(s + 1);
            v1 = *(s += 2);
            *(d + 1) = v0;
            *(d += 2) = v1;
        }
    }
    return 0;
}
#pragma pop

/* Get bitrate from handle */
int MPV_GetBitRate(void* handle, u32* out) {
    MpfGetHd* h = (MpfGetHd*)handle;
    if (MPVLIB_CheckHn(h)) {
        return MPVERR_SetCode(NULL, 0xFF03020D);
    }
    *out = h->bitRate;
    return 0;
}

/* Get VBV buffer size (and derived average/max bitrates) */
int MPV_GetVbvBufSiz(void *handle, u32 *out_size, u32 *out_avg, u32 *out_max) {
    MpfGetHd *h = (MpfGetHd *)handle;
    s32 t;
    if (MPVLIB_CheckHn(h)) {
        return MPVERR_SetCode(NULL, 0xFF03020F);
    }

    *out_size = h->vbvBufSiz << 11;
    *out_avg = h->frameRate;

    // 0x30000 + 0xFFFF sentinel marks "unspecified" max bitrate
    t = h->bitRate;
    if ((u32)(t - 0x30000) == 0xFFFF) {
        *out_max = (u32)-1;
    } else {
        // max = frameRate*bitRate scaled via magic-multiply division,
        // rounded toward zero
        t = h->frameRate * t;
        t = (s32)__mulhw((s32)0x91A2B3C5, t) + t;
        t = t >> 10;
        *out_max = (u32)t + ((u32)t >> 31);
    }
    return 0;
}

/* Get link flags */
int MPV_GetLinkFlg(void *handle, u32 *out_prev, u32 *out_next) {
    u8* h = (u8*)handle;
    if (MPVLIB_CheckHn(h)) {
        return MPVERR_SetCode(NULL, 0xFF03020E);
    }
    *out_prev = *(u32 *)(h + 0xC54);
    *out_next = *(u32 *)(h + 0xC58);
    return 0;
}
