// AHX Bitstream Reader
// libs/CriWare/src/adx/ahx/ahx_bsr.c

#include <harness_catalog.h>
#include <string.h>

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
    /*0x24*/ u32 dataRemain;
    /*0x28*/ u8* dataPtr;
} AHXBSR;

extern u32 lbl_eu_805175A0[];

void* AHXBSR_Create(void* allocator, void* buf, s32 minSize) {
    AHXBSR* bsr;
    u32 size0, size1;

    if (minSize < 0x34) {
        return NULL;
    }

    bsr = (AHXBSR*)ROUND_UP_PTR(buf, 8);
    memset(bsr, 0, 0x2C);

    bsr->streamObj = allocator;
    bsr->position = 0;

    size0 = ((s32 (*)(void*, s32))(*(u32***)allocator)[9])(allocator, 0);
    size1 = ((s32 (*)(void*, s32))(*(u32***)allocator)[9])(allocator, 1);

    bsr->bufSize = size0 + size1;
    bsr->bufSizeWords = (size0 + size1) >> 2;

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

void AHXBSR_Restruct(void* self, void* allocator) {
    AHXBSR* bsr = (AHXBSR*)self;
    u32 size0, size1;

    if (bsr == NULL) return;

    memset(bsr, 0, 0x2C);
    bsr->streamObj = allocator;
    bsr->position = 0;

    size0 = ((s32 (*)(void*, s32))(*(u32***)allocator)[9])(allocator, 0);
    size1 = ((s32 (*)(void*, s32))(*(u32***)allocator)[9])(allocator, 1);

    bsr->bufSize = size0 + size1;
    bsr->bufSizeWords = (size0 + size1) >> 2;
    bsr->dataRemain = 0;
    bsr->readPos = 0;
    bsr->readLen = 0;
    bsr->dataPtr = NULL;
    bsr->initialized = 1;
}

static void ahxbsr_get_data(AHXBSR* bsr) {
    s32 needed = (32 - bsr->bitCnt) >> 3;
    u32 avail = bsr->dataRemain;
    u32 count;
    u32** vt;

    if (avail < 4) {
        u32 tmpPos = bsr->readPos;
        u32 tmpLen = bsr->readLen;

        if (tmpLen != 0) {
            u32 chunk = tmpLen - avail;
            if (tmpLen > chunk) {
                tmpLen = chunk;
            }
            u32 remain = tmpLen - chunk;
            u32 newPos;
            if (remain == 0) {
                newPos = 0;
            } else {
                newPos = tmpPos + chunk;
            }

            vt = *(u32***)bsr->streamObj;
            ((void (*)(void*, s32, u32*))vt[8])(bsr->streamObj, 0, &tmpPos);
            ((void (*)(void*, s32, u32*))vt[7])(bsr->streamObj, 1, &newPos);
        }

        vt = *(u32***)bsr->streamObj;
        ((void (*)(void*, s32, u32*, s32))vt[6])(bsr->streamObj, 1, &bsr->readPos, bsr->bufSizeWords);

        bsr->dataPtr = (u8*)bsr->readPos;
        bsr->dataRemain = bsr->readLen;
    }

    count = bsr->dataRemain;
    if (needed < (s32)count) {
        count = needed;
    }

    if (count == 3) {
        u8* src = bsr->dataPtr;
        bsr->bitBuf = (bsr->bitBuf << 8) | src[0];
        bsr->bitBuf = (bsr->bitBuf << 8) | src[1];
        bsr->bitBuf = (bsr->bitBuf << 8) | src[2];
        bsr->dataPtr = src + 3;
        bsr->bitCnt += 24;
        bsr->dataRemain -= 3;
    } else if (count == 2) {
        u8* src = bsr->dataPtr;
        bsr->bitBuf = (bsr->bitBuf << 8) | src[0];
        bsr->bitBuf = (bsr->bitBuf << 8) | src[1];
        bsr->dataPtr = src + 2;
        bsr->bitCnt += 16;
        bsr->dataRemain -= 2;
    } else if (count == 1) {
        u8* src = bsr->dataPtr;
        bsr->bitBuf = (bsr->bitBuf << 8) | src[0];
        bsr->dataPtr = src + 1;
        bsr->bitCnt += 8;
        bsr->dataRemain -= 1;
    } else if (count == 4) {
        u8* src = bsr->dataPtr;
        bsr->bitBuf = (bsr->bitBuf << 8) | src[0];
        bsr->bitBuf = (bsr->bitBuf << 8) | src[1];
        bsr->bitBuf = (bsr->bitBuf << 8) | src[2];
        bsr->bitBuf = (bsr->bitBuf << 8) | src[3];
        bsr->dataPtr = src + 4;
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
    u32 mask, result;
    s32 shift;

    if (bsr->bitCnt < bits) {
        ahxbsr_get_data(bsr);
    }

    if (bits > bsr->bitCnt) {
        bsr->position += bsr->bitCnt;
        bsr->bitCnt = 0;
        return 0;
    }

    mask = lbl_eu_805175A0[bits];
    shift = bsr->bitCnt - bits;
    result = (bsr->bitBuf >> shift) & mask;

    bsr->bitCnt -= bits;
    bsr->position += bits;

    return result;
}

s32 AHXBSR_Tell(AHXBSR* bsr) {
    return bsr->position;
}

s32 AHXBSR_SearchSync(AHXBSR* bsr) {
    s32 bits, skip, shift;
    u32** vt;
    s32 sync, val, count;
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
        shift = bsr->bitCnt - 12;
        sync = (bsr->bitBuf >> shift) & 0xFFF;
        bsr->bitCnt -= 12;
        bsr->position += 12;
    }

    count = 0;
    while (1) {
        val = sync & 0xFFF;
        if (val == 0xFFF) {
            return 1;
        }
        if ((val + 0x7FFF0000) == 0x8000000C) {
            return 2;
        }

        sync <<= 4;
        if (bsr->bitCnt < 4) {
            ahxbsr_get_data(bsr);
        }

        if (bsr->bitCnt < 4) {
            bsr->position += bsr->bitCnt;
            bsr->bitCnt = 0;
            val = 0;
        } else {
            shift = bsr->bitCnt - 4;
            val = (bsr->bitBuf >> shift) & 0xF;
            bsr->bitCnt -= 4;
            bsr->position += 4;
        }
        sync |= val;

        vt = *(u32***)bsr->streamObj;
        avail = ((s32 (*)(void*, s32))vt[9])(bsr->streamObj, 1);
        if (avail == 0 && bsr->bitCnt == 0 && bsr->dataRemain == 0) {
            break;
        }
    }

    return -1;
}
