// AHX Bitstream Reader
// libs/CriWare/src/adx/ahx/ahx_bsr.c

#include <harness_catalog.h>
#include <string.h>

// CRI I/O object vtable (index 6-9 are the streaming queries used here).
typedef struct AHXBSR_IoVtbl {
    void (*field_0x00)();
    void (*field_0x04)();
    void (*field_0x08)();
    void (*field_0x0C)();
    void (*field_0x10)();
    void (*field_0x14)();
    void (*field_0x18)(void* self, s32 mode, s32 sizeWords, u32* pos); /* 0x18 */
    void (*field_0x1C)(void* self, s32 mode, u32* pos);           /* 0x1C */
    void (*seek)(void* self, s32 mode, u32* pos);                 /* 0x20 */
    s32 (*field_0x24)(void* self, s32 idx);                       /* 0x24 */
} AHXBSR_IoVtbl;

/* Local argument block for the partial-refill path; members stay in memory. */
typedef struct AHXBSR_ReadArgs {
    /*0x00*/ s32 newPos;
    /*0x04*/ s32 diff;
    /*0x08*/ s32 pos;
    /*0x0C*/ s32 len;
} AHXBSR_ReadArgs;

typedef struct AHXBSR {
    /*0x00*/ s32 initialized;
    /*0x04*/ void* streamObj;
    /*0x08*/ u32 bitBuf;
    /*0x0C*/ s32 bitCnt;
    /*0x10*/ s32 position;
    /*0x14*/ s32 bufSize;
    /*0x18*/ s32 bufSizeWords;
    /*0x1C*/ u32 readPos;
    /*0x20*/ u32 readLen;
    /*0x24*/ s32 dataRemain;
    /*0x28*/ u8* dataPtr;
} AHXBSR;

extern const u32 lbl_eu_805175A0[];

void* AHXBSR_Create(void* allocator, void* buf, s32 minSize) {
    AHXBSR* bsr;
    s32 size0, size1;

    if (minSize < 0x34) {
        return NULL;
    }

    bsr = (AHXBSR*)ROUND_UP_PTR(buf, 8);
    memset(bsr, 0, 0x2C);

    bsr->streamObj = allocator;
    bsr->position = 0;

    size0 = ((AHXBSR_IoVtbl*)*(void**)allocator)->field_0x24(allocator, 0);
    size1 = ((AHXBSR_IoVtbl*)*(void**)allocator)->field_0x24(allocator, 1);

    bsr->bufSize = size1 + size0;
    bsr->bufSizeWords = bsr->bufSize / 4;

    bsr->dataRemain = 0;
    bsr->readPos = 0;
    bsr->readLen = 0;
    bsr->dataPtr = NULL;
    bsr->initialized = 1;

    return bsr;
}

void AHXBSR_Destroy(void* self) {
    if (self == NULL) return;
    memset(self, 0, 0x2C);
}

void AHXBSR_Restruct(AHXBSR* bsr, void* allocator) {
    u32 size0, size1;

    if (bsr == NULL) return;

    memset(bsr, 0, 0x2C);
    bsr->streamObj = allocator;
    bsr->position = 0;

    size0 = ((AHXBSR_IoVtbl*)*(void**)allocator)->field_0x24(allocator, 0);
    size1 = ((AHXBSR_IoVtbl*)*(void**)allocator)->field_0x24(allocator, 1);

    bsr->bufSize = size1 + size0;
    bsr->bufSizeWords = bsr->bufSize / 4;
    bsr->dataRemain = 0;
    bsr->readPos = 0;
    bsr->readLen = 0;
    bsr->dataPtr = NULL;
    bsr->initialized = 1;
}

static void ahxbsr_get_data(AHXBSR* bsr) {
    s32 needed = (32 - bsr->bitCnt) / 8;
    s32 count;

    if (bsr->dataRemain < 4) {
        AHXBSR_ReadArgs args;
        s32 tLen = bsr->readLen;
        s32 tPos = bsr->readPos;
        args.len = tLen;
        args.pos = tPos;

        if (args.len != 0) {
            s32 chunk;
            args.diff = args.len;
            chunk = args.diff - bsr->dataRemain;
            if (args.diff > chunk) {
                args.len = chunk;
            }
            /* New position only advances when the clamped length differs. */
            args.diff -= args.len;
            if (args.diff == 0) {
                args.newPos = 0;
            } else {
                args.newPos = args.pos + args.len;
            }
            ((AHXBSR_IoVtbl*)*(void**)bsr->streamObj)->seek(bsr->streamObj, 0, (u32*)&args.pos);
            ((AHXBSR_IoVtbl*)*(void**)bsr->streamObj)->field_0x1C(bsr->streamObj, 1, (u32*)&args.newPos);
        }

        ((AHXBSR_IoVtbl*)*(void**)bsr->streamObj)->field_0x18(bsr->streamObj, 1, bsr->bufSizeWords, &bsr->readPos);

        bsr->dataPtr = (u8*)bsr->readPos;
        bsr->dataRemain = bsr->readLen;
    }

    count = bsr->dataRemain;
    if (needed < count) {
        count = needed;
    }

    if (count == 3) {
        u8* src = bsr->dataPtr;
        u32 val = bsr->bitBuf;
        val = (val << 8) | src[0];
        val = (val << 8) | src[1];
        val = (val << 8) | src[2];
        bsr->dataPtr = src + 3;
        bsr->bitBuf = val;
        bsr->bitCnt += 24;
        bsr->dataRemain -= 3;
    } else if (count == 2) {
        u8* src = bsr->dataPtr;
        u32 val = bsr->bitBuf;
        val = (val << 8) | src[0];
        val = (val << 8) | src[1];
        bsr->dataPtr = src + 2;
        bsr->bitBuf = val;
        bsr->bitCnt += 16;
        bsr->dataRemain -= 2;
    } else if (count == 1) {
        u8* src = bsr->dataPtr;
        u32 val = bsr->bitBuf;
        val = (val << 8) | src[0];
        bsr->dataPtr = src + 1;
        bsr->bitBuf = val;
        bsr->bitCnt += 8;
        bsr->dataRemain -= 1;
    } else if (count == 4) {
        u8* src = bsr->dataPtr;
        u32 val = bsr->bitBuf;
        val = (val << 8) | src[0];
        val = (val << 8) | src[1];
        val = (val << 8) | src[2];
        val = (val << 8) | src[3];
        bsr->dataPtr = src + 4;
        bsr->bitBuf = val;
        bsr->bitCnt += 32;
        bsr->dataRemain -= 4;
    }
}

s32 AHXBSR_IsDataAvailable(AHXBSR* bsr, s32 bits) {
    if (bsr->bitCnt < bits) {
        ahxbsr_get_data(bsr);
    }
    if (bits > bsr->bitCnt) {
        return 0;
    }
    return 1;
}

u32 AHXBSR_GetBitStm(AHXBSR* bsr, s32 bits) {
    s32 shift;
    u32 result;

    if (bsr->bitCnt < bits) {
        ahxbsr_get_data(bsr);
    }

    shift = bsr->bitCnt;
    if (bits > shift) {
        bsr->position += bsr->bitCnt;
        bsr->bitCnt = 0;
        return 0;
    }

    shift = bsr->bitCnt - bits;
    result = lbl_eu_805175A0[bits] & ((s32)bsr->bitBuf >> shift);
    bsr->bitCnt -= bits;
    bsr->position += bits;
    return result;
}

s32 AHXBSR_Tell(AHXBSR* bsr) {
    return bsr->position;
}

s32 AHXBSR_SearchSync(AHXBSR* bsr) {
    u32 bits, sync, val, tmp;
    s32 skip, shift, count;
    s32 avail;

    bits = bsr->position & 7;
    if (bits != 0) {
        skip = 8 - bits;
        if (bsr->bitCnt < skip) {
            ahxbsr_get_data(bsr);
        }
        if (skip > bsr->bitCnt) {
            bsr->position += bsr->bitCnt;
            bsr->bitCnt = 0;
        } else {
            bsr->bitCnt -= skip;
            bsr->position += skip;
        }
    }

    if (bsr->bitCnt < 12) {
        ahxbsr_get_data(bsr);
    }

    if (bsr->bitCnt < 12) {
        bsr->position += bsr->bitCnt;
        bsr->bitCnt = 0;
        sync = 0;
    } else {
        tmp = bsr->bitBuf;
        shift = bsr->bitCnt - 12;
        tmp = (u32)((s32)tmp >> shift);
        sync = tmp & 0xFFF;
        bsr->bitCnt -= 12;
        bsr->position += 12;
    }

    /* Continue while any data remains in the stream object or the local buffer. */
    count = 0;
    while (avail = ((AHXBSR_IoVtbl*)*(void**)bsr->streamObj)->field_0x24(bsr->streamObj, 1),
           !(!(avail == 0) || !(bsr->bitCnt == 0) || !(bsr->dataRemain == 0))) {
        val = sync & 0xFFF;
        if (val == 0xFFF) {
            return 1;
        }
        if ((sync + 0x7FFF0000) == 0xC) {
            return 2;
        }

        sync <<= 4;
        if (bsr->bitCnt < 4) {
            ahxbsr_get_data(bsr);
        }

        if (bsr->bitCnt < 4) {
            bsr->position += bsr->bitCnt;
            bsr->bitCnt = count;
            val = 0;
        } else {
            tmp = bsr->bitBuf;
            shift = bsr->bitCnt - 4;
            tmp = (u32)((s32)tmp >> shift);
            val = tmp & 0xF;
            bsr->bitCnt -= 4;
            bsr->position += 4;
        }
        sync |= val;
    }

    return -1;
}
