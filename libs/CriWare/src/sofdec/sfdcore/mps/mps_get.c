// MPS header getters: copy the pack / system / packet header block the
// decoder filled in (work + 0x18 / 0x28 / 0xA8) back out to the caller.

#include <harness_catalog.h>

#include "libs/CriWare/src/sofdec/sfdcore/mps/mps_types.h"

extern s32 MPSLIB_CheckHn(void *handle);
extern s32 MPSLIB_SetErr(void *handle, u32 err_code);

void MPSGET_Init(void) {}

void MPSGET_Finish(void) {}

s32 MPS_GetPackHd(MPS_WORK *work, MPS_PACKHD *hd) {
    if (MPSLIB_CheckHn(work)) {
        return MPSLIB_SetErr(NULL, 0xFF020201);
    }
    {
        u32 hi0 = work->pack.scr_h;
        u32 lo0 = work->pack.scr_l;
        hd->scr_l = lo0;
        hd->scr_h = hi0;
        u32 hi1 = work->pack.flag;
        u32 lo1 = work->pack.rate;
        hd->rate = lo1;
        hd->flag = hi1;
    }
    return 0;
}

/* Retail copies the four saved system-header words two at a time from
 * work + index * 0x20 (the 0x48 slot offset stays a load displacement), which
 * only reproduces from the byte view: every struct-typed variant of this copy
 * re-colours the entry pointer (r4 instead of r5) and is a pure reg swap. */
s32 MPS_GetSysHd(void *handle, void *out, u32 index) {
    u32 hi0, lo0, hi1, lo1, hi2, lo2, hi3, lo3;
    u8 *entry;
    u32 *hd = (u32 *)out;
    if (MPSLIB_CheckHn(handle)) {
        return MPSLIB_SetErr(NULL, 0xFF020202);
    }
    entry = (u8 *)handle + (index << 5);
    hi0 = *(u32 *)(entry + 0x48);
    lo0 = *(u32 *)(entry + 0x4C);
    hd[1] = lo0;
    hd[0] = hi0;
    hi1 = *(u32 *)(entry + 0x50);
    lo1 = *(u32 *)(entry + 0x54);
    hd[3] = lo1;
    hd[2] = hi1;
    hi2 = *(u32 *)(entry + 0x58);
    lo2 = *(u32 *)(entry + 0x5C);
    hd[5] = lo2;
    hd[4] = hi2;
    hi3 = *(u32 *)(entry + 0x60);
    lo3 = *(u32 *)(entry + 0x64);
    hd[7] = lo3;
    hd[6] = hi3;
    return 0;
}

s32 MPS_GetLastSysHd(MPS_WORK *work, MPS_SYSHDR *hd) {
    if (MPSLIB_CheckHn(work)) {
        return MPSLIB_SetErr(NULL, 0xFF020202);
    }
    {
        u32 w0 = work->sys.w0;
        u32 w1 = work->sys.w1;
        hd->w1 = w1;
        hd->w0 = w0;
        u32 w2 = work->sys.w2;
        u32 w3 = work->sys.w3;
        hd->w3 = w3;
        hd->w2 = w2;
        u32 w4 = work->sys.w4;
        u32 w5 = work->sys.w5;
        hd->w5 = w5;
        hd->w4 = w4;
        u32 w6 = work->sys.w6;
        u32 w7 = work->sys.w7;
        hd->w7 = w7;
        hd->w6 = w6;
    }
    return 0;
}

s32 MPS_GetPketHd(MPS_WORK *work, MPS_PKETHD *hd) {
    if (MPSLIB_CheckHn(work)) {
        return MPSLIB_SetErr(NULL, 0xFF020203);
    }
    {
        u32 w0 = work->pket.pts_lo;
        u32 w1 = work->pket.pts_hi;
        hd->pts_hi = w1;
        hd->pts_lo = w0;
        u32 w2 = work->pket.dts_lo;
        u32 w3 = work->pket.dts_hi;
        hd->dts_hi = w3;
        hd->dts_lo = w2;
        u32 w4 = work->pket.stream_id;
        u32 w5 = work->pket.type;
        hd->type = w5;
        hd->stream_id = w4;
        u32 w6 = work->pket.sub;
        u32 w7 = work->pket.length;
        hd->length = w7;
        hd->sub = w6;
        u32 w8 = work->pket.mpeg1_pts;
        u32 w9 = work->pket.payload;
        hd->payload = w9;
        hd->mpeg1_pts = w8;
    }
    return 0;
}
