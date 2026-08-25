// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/sfd/sfd_pts
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include <string.h>

s32 SFLIB_CheckHn(void* h);
s32 SFLIB_SetErr(void* h, u32 err_code);

// PTS queue entry / input-output record (16 bytes).
typedef struct SfdPtsEntry {
    union {
        s64 pts;   // 0x00 64-bit PTS
        u32 hw[2]; // 0x00 raw halves
    } u;
    u32 pos;  // 0x08 byte position
    u32 size; // 0x0C byte size
} SfdPtsEntry; // 0x10 bytes

// PTS queue descriptor located at offset 0x13F0 of the stream handle.
typedef struct SfdPtsQue {
    SfdPtsEntry* entries; // 0x00
    s32 maxIdx;           // 0x04 ring capacity (entries)
    s32 count;            // 0x08 number of valid entries
    s32 unk0C;            // 0x0C write index
    s32 start;            // 0x10 read position
} SfdPtsQue;

// PTS input record passed by the demuxer.
typedef struct SfdPtsData {
    union {
        s64 pts;   // 0x00 64-bit PTS
        u32 hw[2]; // 0x00 raw halves (hw[0] = high)
    } u;
    u32 pos;   // 0x08 byte position
    u32 size;  // 0x0C byte size
} SfdPtsData;

s32 sfpts_SearchPtsQue(SfdPtsQue* q, u32 target, u32 win_start, u32 win_len);

void SFPTS_InitPtsQue(void* self) {
    *(u32*)((u8*)self) = 0;
    *(u32*)((u8*)self + 4) = 0;
    *(u32*)((u8*)self + 8) = 0;
    *(u32*)((u8*)self + 0xc) = 0;
    *(u32*)((u8*)self + 0x10) = 0;
}

void SFPTS_ResetPtsQue(void* self) {
    *(u32*)((u8*)self + 0x08) = 0;
    *(u32*)((u8*)self + 0x0c) = 0;
    *(u32*)((u8*)self + 0x10) = 0;
}

s32 SFD_SetVideoPts(void* self, void* pts, s32 size) {
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
    *(u32*)((u8*)self + 0x1464) = (u32)aligned;
    *(s32*)((u8*)self + 0x1468) = count / 16;
    *(u32*)((u8*)self + 0x146c) = 0;
    *(u32*)((u8*)self + 0x1470) = 0;
    *(u32*)((u8*)self + 0x1474) = 0;
    return 0;
}

s32 SFPTS_WritePtsQue(void* self, s32 idx, void* data, u32* o) {
    u32* d = (u32*)data;
    SfdPtsQue* q;
    s32 r;
    o[0] = 0;
    if (((SfdPtsData*)data)->u.pts < 0)
        return 0;
    q = (SfdPtsQue*)((u8*)self + idx * 0x74 + 0x13f0);
    if (q->entries == 0)
        return 0;
    if (q->count == q->maxIdx) {
        o[0] = 1;
        r = -1;
    } else {
        s32 uc = q->unk0C;
        s32 next = uc + 1;
        SfdPtsEntry* e = &q->entries[uc];
        e->u.pts = ((SfdPtsData*)data)->u.pts;
        e->pos = ((SfdPtsData*)data)->pos;
        e->size = ((SfdPtsData*)data)->size;
        /* wrap write index: speculative subtract, override when no wrap */
        s32 nn = (next < q->maxIdx) ? next : next - q->maxIdx;
        s32 c2 = q->count + 1;
        q->count = c2;
        q->unk0C = nn;
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

// Per-stream PTS control block (one per demux index, stride 0x74).
typedef struct SfdPtsCtrl {
    u8 pad[0x13D0];
    u32 winStart; // 0x13D0 search window start
    u32 winLen;   // 0x13D4 search window length
    u8 pad2[0x18];
    SfdPtsQue que; // 0x13F0
} SfdPtsCtrl;

s32 SFPTS_ReadPtsQue(void* self, s32 idx, u32 arg, void* out) {
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
            SfdPtsEntry* e = &q->entries[q->start];
            *(SfdPtsEntry*)out = *e;
            /* advance read index with ring wraparound */
            s32 next = q->start + 1;
            q->start = (next < q->maxIdx) ? next : next - q->maxIdx;
            q->count--;
        }
    } else {
        /* target position past the window end wraps back into it */
        if (arg >= win_start + win_len)
            arg -= win_len;
        if (q->count != 0) {
            s32 found = sfpts_SearchPtsQue(q, arg, win_start, win_len);
            if (found != -1) {
                s32 ns = q->start + found;
                ns = (ns < q->maxIdx) ? ns : ns - q->maxIdx;
                q->count -= found;
                q->start = ns;
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
    start = (s32)q->start;
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

int SFPTS_IsPtsQueFull(void* self, int idx) {
    u8* p = (u8*)self + idx * 0x74;
    if (*(u32*)(p + 0x13f0) == 0) return 0;
    s32 a = *(s32*)(p + 0x13f8);
    s32 b = *(s32*)(p + 0x13f4);
    return ((s64)a - (s64)b) >> 32;
}
