// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/sfd/sfd_pts
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include <string.h>

s32 SFLIB_CheckHn(void* h);
s32 SFLIB_SetErr(void* h, u32 err_code);

// PTS queue entry (16 bytes), stored at the queue buffer.
typedef struct SfdPtsEntry {
    u32 lo;   // 0x00 pts low
    u32 hi;   // 0x04 pts high
    u32 pos;  // 0x08 write position
    u32 size; // 0x0C byte size
} SfdPtsEntry; // 0x10 bytes

// PTS queue descriptor located at offset 0x13F0 of the stream handle.
typedef struct SfdPtsQue {
    SfdPtsEntry* entries; // 0x00
    s32 maxIdx;           // 0x04 ring capacity (entries)
    s32 count;            // 0x08 number of valid entries
    s32 unk0C;            // 0x0C
    s32 start;            // 0x10 read position
} SfdPtsQue;

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
    u8* aligned = (u8*)(((u32)pts + 7) & ~7);
    s32 n = size - (s32)(aligned - (u8*)pts);
    memset(aligned, 0, n);
    *(u32*)((u8*)self + 0x1464) = (u32)aligned;
    *(s32*)((u8*)self + 0x1468) = n / 16;
    *(u32*)((u8*)self + 0x146c) = 0;
    *(u32*)((u8*)self + 0x1470) = 0;
    *(u32*)((u8*)self + 0x1474) = 0;
    return 0;
}

s32 SFPTS_WritePtsQue(void* self, s32 idx, void* data, void* out) {
    u32* d = (u32*)data;
    u32* o = (u32*)out;
    SfdPtsQue* q;
    s32 r;
    o[0] = 0;
    if ((s64)(((u64)d[0] << 32) | (u64)d[1]) <= 0)
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
        SfdPtsEntry* e = q->entries + uc;
        e->lo = d[0];
        e->hi = d[1];
        e->pos = d[2];
        e->size = d[3];
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

s32 SFPTS_ReadPtsQue(void* self, s32 idx, u32 arg, void* out) {
    u8* base = (u8*)self + idx * 0x74;
    SfdPtsQue* q = (SfdPtsQue*)(base + 0x13f0);
    u32 win_start = *(u32*)(base + 0x13d0);
    u32 win_len = *(u32*)(base + 0x13d4);
    u32* o = (u32*)out;
    o[1] = -1;
    o[0] = -1;
    if (q->entries == 0)
        return 0;
    if (arg == 0) {
        if (q->count != 0) {
            SfdPtsEntry* e = &q->entries[q->start];
            o[0] = e->lo;
            o[1] = e->hi;
            o[2] = e->pos;
            o[3] = e->size;
            q->start = (q->start + 1 >= q->maxIdx) ? q->start + 1 - q->maxIdx : q->start + 1;
            q->count = q->count - 1;
        }
    } else {
        if (arg >= win_start + win_len)
            arg -= win_len;
        if (q->count != 0) {
            s32 idx2 = sfpts_SearchPtsQue(q, arg, win_start, win_len);
            if (idx2 != -1) {
                u32 np = (q->start + (u32)idx2 >= q->maxIdx)
                             ? q->start + (u32)idx2 - q->maxIdx
                             : q->start + (u32)idx2;
                q->count = q->count - (u32)idx2;
                q->start = np;
                {
                    SfdPtsEntry* e = &q->entries[np];
                    o[0] = e->lo;
                    o[1] = e->hi;
                    o[2] = e->pos;
                    o[3] = e->size;
                }
            }
        }
    }
    return 0;
}

s32 sfpts_SearchPtsQue(SfdPtsQue* q, u32 target, u32 win_start, u32 win_len) {
    s32 count = (s32)q->count;
    u32 win_end = win_start + win_len;
    s32 maxIdx = (s32)q->maxIdx;
    s32 i = 0;
    s32 start = (s32)q->start;

    for (; i < count; i++) {
        SfdPtsEntry* e = &q->entries[start];
        u32 entry_end = e->pos + e->size;

        if (entry_end <= win_end) {
            if (e->pos <= target && target < entry_end)
                return (s32)i;
        } else {
            if (e->pos <= target && target < win_end)
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
