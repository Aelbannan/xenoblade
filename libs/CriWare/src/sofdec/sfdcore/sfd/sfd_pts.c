// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/sfd/sfd_pts
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include <string.h>
#include "libs/CriWare/src/sofdec/sfdcore/sfd/sfd_types.h"

s32 SFLIB_CheckHn(void* h);
s32 SFLIB_SetErr(void* h, u32 err_code);

// PTS queue descriptor lives in SfdBuf::pts (handle+0x13f0 + idx*0x74); the
// queue record the demuxer hands in is the same shape as SfdPtsEntry.

s32 sfpts_SearchPtsQue(SfdPtsQue* q, u32 target, u32 win_start, u32 win_len);

void SFPTS_InitPtsQue(SfdPtsQue* q) {
    q->entries = NULL;
    q->maxIdx = 0;
    q->count = 0;
    q->wr = 0;
    q->rd = 0;
}

void SFPTS_ResetPtsQue(SfdPtsQue* q) {
    q->count = 0;
    q->wr = 0;
    q->rd = 0;
}

s32 SFD_SetVideoPts(SfdHandle* self, void* pts, s32 size) {
    if (pts == 0 || size <= 0)
        return 0;
    if (SFLIB_CheckHn(self)) {
        return SFLIB_SetErr(0, 0xFF000165);
    }
    s32 count;
    u8* aligned;
    aligned = (u8*)(((u32)pts + 7) & ~7);
    count = size - (s32)(aligned - (u8*)pts);
    memset(aligned, 0, count);
    self->buf[1].pts.entries = (SfdPtsEntry*)aligned;
    self->buf[1].pts.maxIdx = count / 16;
    self->buf[1].pts.count = 0;
    self->buf[1].pts.wr = 0;
    self->buf[1].pts.rd = 0;
    return 0;
}

s32 SFPTS_WritePtsQue(SfdHandle* self, s32 idx, void* data, u32* o) {
    u32* d = (u32*)data;
    SfdPtsQue* q;
    s32 r;
    o[0] = 0;
    if (((SfdPtsEntry*)data)->u.pts < 0)
        return 0;
    q = (SfdPtsQue*)((u8*)self + idx * 0x74 + 0x13f0);
    if (q->entries == 0)
        return 0;
    if (q->count == q->maxIdx) {
        o[0] = 1;
        r = -1;
    } else {
        /* NOTE: halves + lo temp are load-bearing for MWCC scheduling:
           hi-first halves let the first store use indexed stwx (base+index
           stay live when e takes a fresh reg); pre-loading lo avoids an
           alias-kill reload of d[1] after the hi store. */
        s32 uc = q->wr;
        SfdPtsEntry* e = &q->entries[uc];
        s32 next = uc + 1;
        u32 lo = d[1];
        u32 size;
        u32 pos;
        e->u.hw[0] = d[0];
        e->u.hw[1] = lo;
        size = d[3];
        pos = d[2];
        e->pos = pos;
        e->size = size;
        /* wrap write index: speculative subtract, override when no wrap */
        s32 nn = (next < q->maxIdx) ? next : next - q->maxIdx;
        s32 c2 = q->count + 1;
        q->count = c2;
        q->wr = nn;
        if (c2 >= q->maxIdx)
            o[0] = 1;
        else
            o[0] = 0;
        r = 0;
    }
    if (r == -1)
        return SFLIB_SetErr(self, 0xFF000421);
    return 0;
}

s32 SFPTS_ReadPtsQue(SfdHandle* self, s32 idx, u32 arg, void* out) {
    u32* o = (u32*)out;
    SfdPtsQue* q;
    u32 win_start;
    u32 win_len;
    o[1] = -1;
    o[0] = -1;
    q = (SfdPtsQue*)((u8*)self + idx * 0x74 + 0x13f0);
    win_start = *(u32*)((u8*)self + idx * 0x74 + 0x13d0);
    win_len = *(u32*)((u8*)self + idx * 0x74 + 0x13d4);
    if (q->entries == 0)
        return 0;

    if (arg == 0) {
        if (q->count != 0) {
            SfdPtsEntry* e = &q->entries[q->rd];
            *(SfdPtsEntry*)out = *e;
            /* advance read index with ring wraparound */
            s32 next = q->rd + 1;
            q->rd = (next < q->maxIdx) ? next : next - q->maxIdx;
            q->count--;
        }
    } else {
        /* target position past the window end wraps back into it */
        if (arg >= win_start + win_len)
            arg -= win_len;
        if (q->count != 0) {
            s32 found = sfpts_SearchPtsQue(q, arg, win_start, win_len);
            if (found != -1) {
                s32 ns = q->rd + found;
                ns = (ns < q->maxIdx) ? ns : ns - q->maxIdx;
                q->count -= found;
                q->rd = ns;
                SfdPtsEntry* e = &q->entries[ns];
                *(SfdPtsEntry*)out = *e;
            }
        }
    }
    return 0;
}

s32 sfpts_SearchPtsQue(SfdPtsQue* q, u32 target, u32 win_start, u32 win_len) {
    s32 start;
    s32 i;
    s32 maxIdx;
    u32 win_end;
    s32 count;
    win_end = win_start + win_len;
    count = (s32)q->count;
    maxIdx = (s32)q->maxIdx;
    start = q->rd;
    i = 0;

    for (; i < count; i++) {
        u32 entry_pos = q->entries[start].pos;
        u32 entry_size = q->entries[start].size;
        u32 entry_end = entry_pos + entry_size;

        if (entry_end <= win_end) {
            if (entry_pos <= target && target < entry_end)
                return (s32)i;
        } else {
            if (entry_pos <= target && target < win_end)
                goto found;
            if (win_start <= target && target < entry_end - win_len) {
            found:
                return (s32)i;
            }
        }

        start = (start + 1 < maxIdx) ? start + 1 : start + 1 - maxIdx;
    }
    return -1;
}

int SFPTS_IsPtsQueFull(SfdHandle* self, int idx) {
    if (self->buf[idx].pts.entries == 0) return 0;
    return self->buf[idx].pts.count >= self->buf[idx].pts.maxIdx;
}
