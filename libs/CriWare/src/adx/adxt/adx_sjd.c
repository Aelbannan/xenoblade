// Auto-scaffolded catalog TU for CriWare/src/adx/adxt/adx_sjd
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

/* External function declarations */
void ADXB_Init(void);
void* ADXB_Create(void* ctx, void* buf, u32 size, u32 totalSize);
void ADXB_Destroy(void* self);
void ADXB_Stop(void* self);
void ADXB_Start(void* self);
void ADXB_Reset(void* self);
void ADXB_ExecHndl(void* self);
s32 ADXB_GetStat(void* self);
s32 ADXB_GetFormat(void* self);
u32 ADXB_GetSfreq(void* self);
s32 ADXB_GetNumChan(void* self);
u32 ADXB_GetBlkSmpl(void* self);
u32 ADXB_GetTotalNumSmpl(void* self);
s32 ADXB_GetFmtBps(void* self);
u32 ADXB_GetDecDtLen(void* self);
u32 ADXB_GetDecNumSmpl(void* self);
u32 ADXB_GetOutBps(void* self);
s16 ADXB_GetNumLoop(void* self);
u32 ADXB_GetLpStartPos(void* self);
u32 ADXB_GetLpStartOfst(u32 val);
u32 ADXB_GetLpEndPos(void* self);
u32 ADXB_GetLpEndOfst(void* self);
s32 ADXB_GetAinfLen(void* self);
s32 ADXB_GetDefOutVol(void* self);
s32 ADXB_GetDefPan(void* self, s32 pan);
void ADXB_GetPcmBuf(void* self);
void ADXB_EntryData(void* self, void* data, u32 len);
void ADXB_EntryGetWrFunc(void* self, void* func, void* ctx);
void ADXB_SetLnkSw(void* self, u32 val);
void ADXB_SetDefFmt(void* self);
void ADXB_SetDefPrm(void* self);
void ADXB_SetAhxInSj(void* self);
void ADXB_SetAhxDecSmpl(void* self, u32 val);
void ADXB_AhxTermSupply(void* self);
s32 ADXB_DecodeHeader(void* self, void* data);
s32 ADXB_GetDecErrMode(void);
void ADXB_TakeSnapshot(void* self);
void ADXB_RestoreSnapshot(void* self);

void* SJRBF_GetBufPtr(void* self);
s32 SJRBF_GetBufSize(void* self);
s32 SJRBF_GetXtrSize(void* self);
void SJ_SplitChunk(void* src, s32 size, void* dst1, void* dst2);
s32 ADX_DecodeFooter(u8* data, s32 maxLen, s16* outLen);

void ADXCRS_Lock(void);
void ADXCRS_Unlock(void);
void ADXERR_CallErrFunc2_(void* a, void* b);
void criware_eu_8038A864(void* self);

extern volatile u32 lbl_eu_805E3340;
extern u8 lbl_eu_805E3358[0xB40];
extern u32 lbl_eu_805E3354;
extern u8 lbl_eu_805160B8[];

/* Forward declarations */
void adxsjd_decode_prep(void* self);
void adxsjd_get_wr(void* self, u32* outHalf, u32* outNumSmpl, u32* outTrap);
void adxsjd_decexec_end(void* self);
void adxsjd_insert_proc(void* self);
void adxsjd_discard_proc(void* self);
void ADXSJD_ExecServer(void);

/* Helper: call virtual function from C-style vtable at offset 0x0 of object */
/* The SJ/ADXB objects use a "class" pointer at +0x0 with function pointers at word offsets */

void ADXSJD_Init(void) {
    if (lbl_eu_805E3340 == 0) {
        ADXB_Init();
        memset(lbl_eu_805E3358, 0, 0xB40);
    }
    lbl_eu_805E3340++;
}

void ADXSJD_Finish(void) {
    lbl_eu_805E3340--;
    if (lbl_eu_805E3340 == 0) {
        memset(lbl_eu_805E3358, 0, 0xB40);
    }
}

void* ADXSJD_Create(void* stream, s32 numChan, void* sjArray) {
    void* bufPtr;
    void* sj0;
    u8* sjd;
    u8* p;
    s32 i;
    s32 j;
    s32 halfBuf;
    void* adxb;

    /* Find a free slot in the 16-entry work area (0xB4 bytes per entry) */
    p = lbl_eu_805E3358;
    sj0 = *(void**)sjArray;
    for (i = 0; i < 16; i++) {
        if ((s8)*p == 0) break;
        p += 0xB4;
    }
    if (i == 16) return NULL;
    sjd = &lbl_eu_805E3358[i * 0xB4];

    bufPtr = SJRBF_GetBufPtr(sj0);
    halfBuf = SJRBF_GetBufSize(sj0) / 2;
    *(void**)((u8*)sjd + 0x04) = ADXB_Create((void*)numChan, bufPtr, halfBuf,
                                             halfBuf + SJRBF_GetXtrSize(sj0) / 2);
    adxb = *(void**)((u8*)sjd + 0x04);
    if (adxb == NULL) return NULL;

    ADXB_EntryGetWrFunc(adxb, adxsjd_get_wr, sjd);
    *(void**)((u8*)sjd + 0x08) = stream;
    *(s8*)((u8*)sjd + 0x02) = (s8)numChan;

    /* Copy channel SJ pointers */
    for (j = 0; j < numChan; j++) {
        ((u32*)sjd)[j + 3] = ((u32*)sjArray)[j];
    }

    /* Initialize state fields */
    *(u8*)((u8*)sjd + 0x01) = 0;          /* status */
    *(u32*)((u8*)sjd + 0xA0) = 0;
    *(u32*)((u8*)sjd + 0x2C) = 0;
    *(u32*)((u8*)sjd + 0x30) = 0;
    *(u32*)((u8*)sjd + 0x34) = 0;
    *(u32*)((u8*)sjd + 0x38) = 0x7FFFFFFF;
    *(u32*)((u8*)sjd + 0x3C) = (u32)-1;
    *(u32*)((u8*)sjd + 0x40) = 0;
    *(u32*)((u8*)sjd + 0x44) = 0;
    *(u8*)((u8*)sjd + 0x03) = 0;
    *(u32*)((u8*)sjd + 0xA8) = 0;
    *(u32*)((u8*)sjd + 0xAC) = 0;
    *(u32*)((u8*)sjd + 0x48) = 0;
    *(u32*)((u8*)sjd + 0x4C) = 0;
    *(u32*)((u8*)sjd + 0x58) = 0;
    *(u32*)((u8*)sjd + 0x5C) = 0;
    *(u8*)((u8*)sjd + 0x00) = 1;          /* inUse */

    return sjd;
}

void ADXSJD_Destroy(void* self) {
    void* adxb;
    if (self == NULL) return;

    adxb = *(void**)((u8*)self + 0x04);
    if (adxb != NULL) {
        *(void**)((u8*)self + 0x04) = NULL;
        ADXB_Destroy(adxb);
    }

    ADXCRS_Lock();
    memset(self, 0, 0xB4);
    ADXCRS_Unlock();
}

int ADXSJD_GetStat(void* self) {
    return (s32)*(s8*)((u8*)self + 0x01);
}

void ADXSJD_SetInSj(void* self, void* sj) {
    *(void**)((u8*)self + 0x08) = sj;
    ADXB_SetAhxInSj(*(void**)((u8*)self + 0x04));
}

void ADXSJD_SetMaxDecSmpl(void* self, u32 val) {
    *(u32*)((u8*)self + 0x38) = val;
    ADXB_SetAhxDecSmpl(*(void**)((u8*)self + 0x04), val);
}

void ADXSJD_TermSupply(void* self) {
    ADXB_AhxTermSupply(*(void**)((u8*)self + 0x04));
}

void ADXSJD_Start(void* self) {
    *(u32*)((u8*)self + 0xA0) = 0;
    *(u32*)((u8*)self + 0x2C) = 0;
    *(u32*)((u8*)self + 0x30) = 0;
    *(u32*)((u8*)self + 0x34) = 0;
    *(u32*)((u8*)self + 0x38) = 0x7FFFFFFF;
    *(u32*)((u8*)self + 0x3C) = (u32)-1;
    *(u32*)((u8*)self + 0x40) = 0;
    *(u32*)((u8*)self + 0x44) = 0;
    *(u8*)((u8*)self + 0x03) = 0;
    *(u32*)((u8*)self + 0xA8) = 0;
    *(u32*)((u8*)self + 0xAC) = 0;
    *(u8*)((u8*)self + 0x01) = 1;
}

void ADXSJD_Stop(void* self) {
    ADXB_Stop(*(void**)((u8*)self + 0x04));
    *(u8*)((u8*)self + 0x01) = 0;
}

/* Chunk pair returned by SJ_SplitChunk / consumed by SJ commit/unread */
typedef struct {
    void* ptr;
    s32 len;
} SjChunk;
void adxsjd_decode_prep(void* self) {
    void* adxb = *(void**)((u8*)self + 0x04);
    void* inSj = *(void**)((u8*)self + 0x08);
    struct {
        SjChunk tail; /* sp+0x08: remainder chunk */
        SjChunk head; /* sp+0x10: chunk consumed by the decoder */
    } chk;
    s32 hdrLen;
    s32 fmt;

    /* Read one chunk from the input SJ (vtable slot 6 takes a size limit) */
    ((void (*)(void*, s32, s32, void*))(*(void***)inSj)[6])(inSj, 1, 0xC800, &chk.head);

    s32 len = chk.head.len;

    /* Empty chunk: give it back */
    if (len <= 0) {
        ((void (*)(void*, s32, void*))(*(void***)inSj)[7])(inSj, 1, &chk.head);
        return;
    }

    /* Fresh non-AHX stream: skip zero padding before the header */
    if (*(s16*)((u8*)adxb + 0x9A) == 0 && *(s16*)((u8*)adxb + 0x98) != 0x0D) {
        s8* p = (s8*)chk.head.ptr;
        s32 idx;
        /* Condition-first loop: idx starts at 0 so the body never runs
         * (matches retail, which guards with a constant-false test) */
        for (idx = 0; idx < 0; idx++) {
            if (*p != 0) break;
            p++;
        }
        SJ_SplitChunk(&chk.head, idx, &chk.tail, &chk.head);
        ((void (*)(void*, s32, void*))(*(void***)inSj)[8])(inSj, 0, &chk.head);
        len = chk.head.len;
        if (len < 0x10) {
            /* Too small to hold a header */
            ((void (*)(void*, s32, void*))(*(void***)inSj)[7])(inSj, 1, &chk.head);
            return;
        }
    }

    if (*(s16*)((u8*)adxb + 0x9A) == 0 && *(s16*)((u8*)adxb + 0x98) != 0x0D) {
        hdrLen = ADXB_DecodeHeader(adxb, (s8*)chk.head.ptr);
        if (hdrLen == 0 || hdrLen > chk.head.len) {
            ((void (*)(void*, s32, void*))(*(void***)inSj)[7])(inSj, 1, &chk.head);
            return;
        } else if (hdrLen < 0) {
            ((void (*)(void*, s32, void*))(*(void***)inSj)[7])(inSj, 1, &chk.head);
            /* NOTE: retail emits no argument setup here */
            if (ADXB_GetDecErrMode() == 0) {
                u8* errStr = lbl_eu_805160B8;
                ADXERR_CallErrFunc2_(errStr, errStr + 0x1E);
            }
            *(u8*)((u8*)self + 0x01) = 4;
            return;
        }
    } else {
        if (*(s16*)((u8*)adxb + 0x98) == 0x0D) {
            ADXB_SetDefPrm(adxb);
            hdrLen = 0;
        } else {
            criware_eu_8038A864(adxb);
            hdrLen = 0;
        }
    }

    *(u32*)((u8*)self + 0xA0) = hdrLen;

    /* Notify the registered filter callback with the stream properties */
    if (*(u32*)((u8*)self + 0x50) != 0) {
        s32 fltFmt = ADXB_GetFormat(adxb);
        s32 fltCh = ADXB_GetNumChan(adxb);
        s32 fltSfreq = ADXB_GetSfreq(adxb);
        u32 fltTotal = ADXB_GetTotalNumSmpl(adxb);
        ((void (*)(void*, s32, s32, s32, u32))*(u32*)((u8*)self + 0x50))(
            *(void**)((u8*)self + 0x54), fltFmt, fltCh, fltSfreq, fltTotal);
    }

    /* AHX streams set a dedicated flag */
    if (ADXB_GetFormat(adxb) == 4) {
        *(u8*)((u8*)self + 0x03) = 1;
    }

    /* Format 2 carries inline SPSD info right after the header */
    if (ADXB_GetFormat(adxb) == 2) {
        memcpy((u8*)self + 0x60, chk.head.ptr,
               (chk.head.len < 0x40) ? chk.head.len : 0x40);
    }

    fmt = ADXB_GetFormat(adxb);
    if ((u32)(fmt - 0xA) <= 3 || fmt == 0x14 || fmt == 0xF) {
        /* These formats re-read everything themselves */
        ((void (*)(void*, s32, void*))(*(void***)inSj)[7])(inSj, 1, &chk.head);
    } else {
        /* Consume the header, queue the remainder for decoding */
        SJ_SplitChunk(&chk.head, hdrLen, &chk.head, &chk.tail);
        ((void (*)(void*, s32, void*))(*(void***)inSj)[8])(inSj, 0, &chk.head);
        ((void (*)(void*, s32, void*))(*(void***)inSj)[7])(inSj, 1, &chk.tail);
    }

    /* Link-switch hook, registered globally */
    if (*(u32*)((u8*)adxb + 0xE0) != 0) {
        void (*cb)(void*, u32) = *(void (**)(void*, u32))&lbl_eu_805E3354;
        if (cb != NULL) {
            cb(adxb, *(u32*)((u8*)adxb + 0x14));
        }
    }

    *(u8*)((u8*)self + 0x01) = 2;
}

void adxsjd_get_wr(void* self, u32* outHalf, u32* outNumSmpl, u32* outTrap) {
    u8* wr;
    u8* p;
    /* Channel 0 SJ kept live across the loop: reused for the buffer-pointer query */
    void* chan0;
    int i;
    p = (u8*)self;
    wr = p + 0x1C;
    i = 0;
    chan0 = *(void**)((u8*)self + 0x0C);

    for (; i < ADXB_GetNumChan(*(void**)((u8*)self + 0x04)); i++) {
        void* chan = *(void**)(p + 0x0C);
        void (*getBufFn)(void*, s32, s32, void*) =
            (void (*)(void*, s32, s32, void*))(*(void***)chan)[6];
        getBufFn(chan, 0, 0x4000, wr);
        wr += 8;
        p += 4;
    }

    /* outHalf = distance of wr[0].ptr from the channel buffer start, halved */
    {
        s32 diff = *(u32*)((u8*)self + 0x1C) - (u32)SJRBF_GetBufPtr(chan0);
        *(s32*)outHalf = diff / 2;
    }

    /* outNumSmpl = min(wr[0].len / 2, maxDecSmpl) */
    {
        s32 half = *(s32*)((u8*)self + 0x20) / 2;
        u32 maxSmpl = *(u32*)((u8*)self + 0x38);
        if (half < (s32)maxSmpl) {
            maxSmpl = (u32)half;
        }
        *(u32*)outNumSmpl = maxSmpl;
    }

    /* outTrap = remaining samples until trap, or a large sentinel */
    {
        s32 trapNum = *(s32*)((u8*)self + 0x3C);
        if (trapNum >= 0) {
            *(u32*)outTrap = (u32)(trapNum - *(s32*)((u8*)self + 0x40));
        } else {
            *(u32*)outTrap = 0x1FFFFFFF;
        }
    }

    ADXB_GetPcmBuf(*(void**)((u8*)self + 0x04));
}

/* SJ decoder handle work area (0xB4 bytes per slot) */
typedef struct SjdWork {
    s8 inUse;       /* 0x00 */
    s8 status;      /* 0x01: decoder state */
    s8 numChan;     /* 0x02 */
    s8 ahxFlag;     /* 0x03 */
    void* adxb;     /* 0x04: bound ADXB handle */
    void* inSj;     /* 0x08: input stream journal */
    void* chan[2];  /* 0x0C: per-channel output SJs */
    SjChunk chunk;  /* 0x14: chunk currently staged for the decoder */
    u8 pad1C[0x10];
    u32 totalSamples; /* 0x2C */
    u32 totalDtLen;   /* 0x30 */
    u32 decPos;       /* 0x34 */
    u32 maxDecSmpl;   /* 0x38 */
    s32 trapNumSmpl;  /* 0x3C */
    u32 trapCnt;      /* 0x40 */
    u32 trapDtLen;    /* 0x44 */
    void* trapFunc;   /* 0x48 */
    void* trapCtx;    /* 0x4C */
    u8 pad50[0x50];
    u32 hdrLen;     /* 0xA0 */
    u32 lnkSw;      /* 0xA4 */
    s32 insertCnt;  /* 0xA8: pending insert (samples) */
    s32 discardCnt; /* 0xAC: pending discard (samples) */
    u32 pcmLoop;    /* 0xB0: non-zero keeps looping PCM playback alive */
} SjdWork;

/* Minimal view of the ADXB handle for direct field reads */
typedef struct AdxBView {
    u8 pad00[0x98];
    s16 format; /* 0x98: current codec format id */
} AdxBView;

/* Round-half-up signed division by 2, as emitted by MWCC for s32 x / 2:
 * (x + (u32)x >> 31) >> 1 */
void adxsjd_decexec_start(SjdWork* sjd) {
    void* adxb = sjd->adxb;
    void* inSj = sjd->inSj;
    s32 flg = 0; /* end-of-stream flag; also holds the read-limit constant */
    SjChunk tail;   /* sp+0x18: remainder chunk given back to the input SJ */
    SjChunk mid;    /* sp+0x10: scratch chunk for the PCM truncation split */
    s16 footerLen;  /* sp+0x08 */
    s32 totalSmpl;

    /* Trap callback: fires once the decoded-sample counter reaches the trap point */
    {
        s32 trapNum = sjd->trapNumSmpl;
        if (trapNum >= 0 && (s32)sjd->trapCnt >= trapNum) {
            if (sjd->trapFunc != NULL) {
                ((void (*)(void*, s32))sjd->trapFunc)(sjd->trapCtx, trapNum);
            }
        }
    }

    /* AHX stream: stop once its dedicated server drained the input */
    if (sjd->ahxFlag == 1) {
        if (((s32 (*)(void*, s32))(*(void***)inSj)[9])(inSj, 1) == 0) {
            sjd->status = 3;
            return;
        }
    }

    /* Pull the next chunk into the handle's persistent chunk */
    ((void (*)(void*, s32, s32, void*))(*(void***)inSj)[6])(inSj, 1, 0x7FFFFFFF,
                                                            &sjd->chunk);

    /* Format not yet determined: sniff for an AHX footer tag */
    if (ADXB_GetFormat(adxb) == 0) {
        if ((s32)sjd->chunk.len >= 4) {
            if (*(u16*)sjd->chunk.ptr == 0x8001) {
                sjd->status = 3;
                if (ADX_DecodeFooter((u8*)sjd->chunk.ptr, (s32)sjd->chunk.len, &footerLen) == 0) {
                    if ((s32)footerLen > (s32)sjd->chunk.len) {
                        /* Not enough data for the footer yet: give the chunk back */
                        ((void (*)(void*, s32, void*))(*(void***)inSj)[7])(inSj, 1, &sjd->chunk);
                        return;
                    }
                    /* Consume the footer, queue the rest for decoding */
                    SJ_SplitChunk(&sjd->chunk, footerLen, &sjd->chunk, &tail);
                    ((void (*)(void*, s32, void*))(*(void***)inSj)[8])(inSj, 0, &sjd->chunk);
                    ((void (*)(void*, s32, void*))(*(void***)inSj)[7])(inSj, 1, &tail);
                }

                /* Link switch: drain leading zero padding between linked streams */
                if (sjd->lnkSw != 0) {
                    u32 dataLen;
                    s32 skip;

                    flg = (s32)0x80000000; /* read-limit constant */
                    do {
                        ((void (*)(void*, s32, s32, void*))(*(void***)inSj)[6])(
                            inSj, 1, (u32)flg - 1, &sjd->chunk);
                        dataLen = sjd->chunk.len;
                        if (dataLen == 0) break;
                        for (skip = 0; skip < (s32)dataLen; skip++) {
                            if (((s8*)sjd->chunk.ptr)[skip] != 0) break;
                        }
                        SJ_SplitChunk(&sjd->chunk, skip, &sjd->chunk, &tail);
                        ((void (*)(void*, s32, void*))(*(void***)inSj)[8])(inSj, 0, &sjd->chunk);
                        ((void (*)(void*, s32, void*))(*(void***)inSj)[7])(inSj, 1, &tail);
                    } while (skip >= (s32)dataLen);
                }
                return;
            }
        }
    }

    /* End-of-stream detection */
    totalSmpl = ADXB_GetTotalNumSmpl(sjd->adxb);
    if ((s32)sjd->decPos >= totalSmpl) {
        /* Looping PCM streams never end here */
        if (ADXB_GetFormat(adxb) != 1 || (s32)sjd->pcmLoop != 1) {
            flg = 1;
        }
    } else if (ADXB_GetFormat(adxb) == 0xA) {
        /* AHX: end shortly after the last block is consumed */
        s32 decPos = sjd->decPos;
        if (decPos > 0) {
            if (decPos + 0x240 >= totalSmpl) {
                flg = 1;
            }
        }
        if (totalSmpl <= 0) {
            flg = 1;
        }
    }

    if (flg != 0) {
        sjd->status = 3;
        ((void (*)(void*, s32, void*))(*(void***)inSj)[7])(inSj, 1, &sjd->chunk);
        return;
    }

    /* Wait until channel 0 has room for a whole block of samples */
    {
        u32 blkSmpl = ADXB_GetBlkSmpl(sjd->adxb);
        void* sj0 = sjd->chan[0];
        s32 freeSize = ((s32 (*)(void*, s32))(*(void***)sj0)[9])(sj0, 0);
        s32 avail = freeSize / 2;
        if (avail < (s32)blkSmpl) {
            ((void (*)(void*, s32, void*))(*(void***)inSj)[7])(inSj, 1, &sjd->chunk);
            return;
        }
    }

    /* Non-looping 16-bit PCM: trim the final partial block */
    if ((s32)sjd->pcmLoop != 1 && ADXB_GetFormat(adxb) == 1) {
        if (ADXB_GetFmtBps(adxb) == 0x10) {
            s32 nch = ADXB_GetNumChan(sjd->adxb);
            s32 half = ((s32)sjd->chunk.len / nch) / 2;
            s32 decPos = sjd->decPos;
            if (decPos + half > totalSmpl) {
                s32 remain = totalSmpl - decPos;
                u32 byteLen = nch * remain * 2;
                SJ_SplitChunk(&sjd->chunk, byteLen, &sjd->chunk, &mid);
                ((void (*)(void*, s32, void*))(*(void***)inSj)[7])(inSj, 1, &mid);
            }
        } else {
            ADXERR_CallErrFunc2_(lbl_eu_805160B8 + 0x3F, lbl_eu_805160B8 + 0x60);
        }
    }

    /* AHX: the decoder re-reads everything itself, give the chunk back */
    if (ADXB_GetFormat(adxb) == 0xA) {
        ((void (*)(void*, s32, void*))(*(void***)inSj)[7])(inSj, 1, &sjd->chunk);
    }

    ADXB_EntryData(adxb, sjd->chunk.ptr, sjd->chunk.len);
    ADXB_Start(adxb);
}

void adxsjd_decexec_end(void* self) {
    s32 decDtLen;
    s32 decNumSmpl;
    u8* wr;
    void** chanP;
    s32 smplBytes;
    void* adxb;
    s32 totalSmpl;
    s32 i;
    void* inSj;
    SjChunk headChunk;   /* sp+0x10: first split result */
    SjChunk tailChunk;   /* sp+0x08: second split result */

    inSj = *(void**)((u8*)self + 0x08);
    adxb = *(void**)((u8*)self + 0x04);
    totalSmpl = ADXB_GetTotalNumSmpl(adxb);
    decDtLen = ADXB_GetDecDtLen(adxb);
    decNumSmpl = ADXB_GetDecNumSmpl(adxb);

    /* Limit decoded samples unless looping PCM playback */
    if (ADXB_GetFormat(adxb) != 1 || *(s32*)((u8*)self + 0xB0) != 1) {
        s32 remaining = totalSmpl - *(u32*)((u8*)self + 0x34);
        if (decNumSmpl < remaining) {
            remaining = decNumSmpl;
        }
        decNumSmpl = remaining;
    }

    SJ_SplitChunk((u8*)self + 0x14, decDtLen, &headChunk, &tailChunk);

    {
        void** vtbl = *(void***)inSj;
        void (*commitFn)(void*, s32, void*) = (void (*)(void*, s32, void*))vtbl[8];
        commitFn(inSj, 0, &headChunk);
    }
    {
        void** vtbl = *(void***)inSj;
        void (*unreadFn)(void*, s32, void*) = (void (*)(void*, s32, void*))vtbl[7];
        unreadFn(inSj, 1, &tailChunk);
    }

    /* Per-channel: split decoded samples, notify callback, commit to channel SJ */
    chanP = (void**)(u8*)self;
    wr = (u8*)self + 0x1C;
    smplBytes = decNumSmpl * 2;
    for (i = 0; i < ADXB_GetNumChan(*(void**)((u8*)self + 0x04)); i++) {
        SJ_SplitChunk(wr, smplBytes, &headChunk, &tailChunk);

        if (*(u32*)((u8*)self + 0x58) != 0) {
            void (*cbFn)(void*, s32, void*, u32) =
                *(void (**)(void*, s32, void*, u32))((u8*)self + 0x58);
            cbFn(*(void**)((u8*)self + 0x5C), i, headChunk.ptr, headChunk.len);
        }

        {
            void* chan = *(void**)((u8*)chanP + 0x0C);
            void** vtbl = *(void***)chan;
            void (*commitFn)(void*, s32, void*) = (void (*)(void*, s32, void*))vtbl[8];
            commitFn(chan, 1, &headChunk);
        }
        {
            void* chan = *(void**)((u8*)chanP + 0x0C);
            void** vtbl = *(void***)chan;
            void (*unreadFn)(void*, s32, void*) = (void (*)(void*, s32, void*))vtbl[7];
            unreadFn(chan, 0, &tailChunk);
        }
        wr += 8;
        chanP = (void**)((u8*)chanP + 4);
    }

    *(u32*)((u8*)self + 0x2C) += decNumSmpl;
    *(u32*)((u8*)self + 0x30) += decDtLen;
    *(u32*)((u8*)self + 0x34) += decNumSmpl;
    *(u32*)((u8*)self + 0x40) += decNumSmpl;
    *(u32*)((u8*)self + 0x44) += decDtLen;
    ADXB_Reset(adxb);
}

void ADXSJD_ExecHndl(SjdWork* sjd) {
    /* Insert proc: runs under the resource lock */
    if (sjd->insertCnt > 0) {
        ADXCRS_Lock();
        adxsjd_insert_proc(sjd);
        ADXCRS_Unlock();
    }

    /* State machine */
    if (sjd->status == 2) {
        void* adxb = sjd->adxb;
        if (ADXB_GetStat(adxb) == 0) {
            adxsjd_decexec_start(sjd);
        }
        ADXB_ExecHndl(adxb);
        if (ADXB_GetStat(adxb) == 3) {
            adxsjd_decexec_end(sjd);
        }
        {
            /* Formats 0x0A-0x0C, 0x0F and 0x14 decode without decexec_end:
             * accumulate their block progress directly */
            s16 fmt = ((AdxBView*)adxb)->format;
            if ((u16)(fmt - 0x0A) <= 2 || fmt == 0x14 || fmt == 0x0F) {
                /* Re-read the handle: decexec_end may have rebound it */
                adxb = sjd->adxb;
                u32 dtLen = ADXB_GetDecDtLen(adxb);
                u32 numSmpl = ADXB_GetDecNumSmpl(adxb);
                sjd->totalSamples += numSmpl;
                sjd->totalDtLen += dtLen;
                sjd->decPos += numSmpl;
            }
        }
    } else if (sjd->status == 1) {
        adxsjd_decode_prep(sjd);
    }

    /* Discard proc: runs under the resource lock */
    if (sjd->discardCnt > 0) {
        ADXCRS_Lock();
        adxsjd_discard_proc(sjd);
        ADXCRS_Unlock();
    }
}

void adxsjd_insert_proc(void* self) {
    u32 wrBuf[2];
    s32 i;
    void** p;
    s32 maxInsert;
    s32 half;

    p = (void**)self;
    i = 0;
    /* Pending insert samples converted to byte count */
    maxInsert = *(u32*)((u8*)self + 0xA8) * 2;

    /* Phase 1: query free space per channel and take the minimum */
    for (; i < (s32)*(s8*)((u8*)self + 0x02); i++) {
        ((void (*)(void*, s32, s32, void*))(*(void***)*(void**)((u8*)p + 0x0C))[6])(
            *(void**)((u8*)p + 0x0C), 0, 0x7FFFFFFF, wrBuf);
        maxInsert = (maxInsert < (s32)wrBuf[1]) ? maxInsert : (s32)wrBuf[1];
        {
            void* chan = *(void**)((u8*)p + 0x0C);
            ((void (*)(void*, s32, void*))(*(void***)chan)[7])(chan, 0, wrBuf);
        }
        p++;
    }

    /* Guard uses the same halving as below; MWCC folds (/2)*2 onto the
     * sign-biased intermediate */
    half = maxInsert / 2;
    if ((s32)(half * 2) <= 0) return;

    /* Phase 2: insert silence of the aligned byte count into every channel */
    i = 0;
    p = (void**)self;
    for (; i < (s32)*(s8*)((u8*)self + 0x02); i++) {
        ((void (*)(void*, s32, s32, void*))(*(void***)*(void**)((u8*)p + 0x0C))[6])(
            *(void**)((u8*)p + 0x0C), 0, half * 2, wrBuf);
        memset((void*)wrBuf[0], 0, half * 2);
        {
            void* chan = *(void**)((u8*)p + 0x0C);
            ((void (*)(void*, s32, void*))(*(void***)chan)[8])(chan, 1, wrBuf);
        }
        p++;
    }

    *(s32*)((u8*)self + 0xA8) -= half;
}

void adxsjd_discard_proc(void* self) {
    SjChunk chunk;
    void** chanP;
    s32 i;
    s32 maxDiscard;
    s32 half;

    chanP = (void**)self;
    i = 0;
    maxDiscard = *(s32*)((u8*)self + 0xAC) * 2;

    /* Phase 1: query free space per channel and take the minimum */
    for (; i < (s32)*(s8*)((u8*)self + 0x02); i++) {
        ((void (*)(void*, s32, s32, void*))(*(void***)*(void**)((u8*)chanP + 0x0C))[6])(
            *(void**)((u8*)chanP + 0x0C), 1, 0x7FFFFFFF, &chunk);
        maxDiscard = (maxDiscard < (s32)chunk.len) ? maxDiscard : (s32)chunk.len;
        {
            void* chan = *(void**)((u8*)chanP + 0x0C);
            ((void (*)(void*, s32, void*))(*(void***)chan)[7])(chan, 1, &chunk);
        }
        chanP++;
    }

    /* Guard uses the same halving as below; MWCC folds (/2)*2 onto the
     * sign-biased intermediate */
    half = maxDiscard / 2;
    if ((s32)(half * 2) <= 0) return;

    /* Phase 2: discard the aligned byte count from every channel */
    chanP = (void**)self;
    i = 0;
    for (; i < (s32)*(s8*)((u8*)self + 0x02); i++) {
        void* chan = *(void**)((u8*)chanP + 0x0C);
        ((void (*)(void*, s32, s32, void*))(*(void***)chan)[6])(chan, 1, half * 2, &chunk);
        {
            void* chan2 = *(void**)((u8*)chanP + 0x0C);
            ((void (*)(void*, s32, void*))(*(void***)chan2)[8])(chan2, 0, &chunk);
        }
        chanP++;
    }

    *(s32*)((u8*)self + 0xAC) -= half;
}

void ADXSJD_ExecServer(void) {
    u8* work = (u8*)&lbl_eu_805E3340;
    int i;
    u8* sjd;

    /* Pre-server callback (indirect through stored function pointer) */
    {
        void (**preCb)(void*) = (void (**)(void*))(work + 0x04);
        if (*preCb != NULL) {
            (*preCb)(*(void**)(work + 0x08));
        }
    }

    /* Process each handle slot */
    for (i = 0; i < 16; i++) {
        u8* sjd2 = work + 0x18 + i * 0xB4;
        if (*(s8*)sjd2 == 1) {
            /* Pending insert runs under the resource lock */
            if (*(s32*)(sjd2 + 0xA8) > 0) {
                ADXCRS_Lock();
                adxsjd_insert_proc(sjd2);
                ADXCRS_Unlock();
            }

            if (*(s8*)(sjd2 + 0x01) == 2) {
                void* adxb = *(void**)(sjd2 + 0x04);
                if (ADXB_GetStat(adxb) == 0) {
                    adxsjd_decexec_start((SjdWork*)sjd2);
                }
                ADXB_ExecHndl(adxb);
                if (ADXB_GetStat(adxb) == 3) {
                    adxsjd_decexec_end(sjd2);
                }
                {
                    /* Formats 0x0A-0x0C, 0x0F and 0x14 accumulate block
                     * progress directly instead of via decexec_end */
                    s16 fmt = ((AdxBView*)adxb)->format;
                    if ((u16)(fmt - 0x0A) <= 2 || fmt == 0x14 || fmt == 0x0F) {
                        u32 dtLen;
                        u32 numSmpl;
                        void* adxb2 = *(void**)(sjd2 + 0x04);
                        dtLen = ADXB_GetDecDtLen(adxb2);
                        numSmpl = ADXB_GetDecNumSmpl(adxb2);
                        *(u32*)(sjd2 + 0x2C) += numSmpl;
                        *(u32*)(sjd2 + 0x30) += dtLen;
                        *(u32*)(sjd2 + 0x34) += numSmpl;
                    }
                }
            } else if (*(s8*)(sjd2 + 0x01) == 1) {
                adxsjd_decode_prep(sjd2);
            }

            /* Pending discard runs under the resource lock */
            if (*(s32*)(sjd2 + 0xAC) > 0) {
                ADXCRS_Lock();
                adxsjd_discard_proc(sjd2);
                ADXCRS_Unlock();
            }
        }
    }

    /* Post-server callback */
    {
        void (**postCb)(void*) = (void (**)(void*))(work + 0x0C);
        if (*postCb != NULL) {
            (*postCb)(*(void**)(work + 0x10));
        }
    }
}

u32 ADXSJD_GetDecDtLen(void* self) { return *(u32*)((u8*)self + 0x30); }

u32 ADXSJD_GetDecNumSmpl(void* self) { return *(u32*)((u8*)self + 0x2C); }

void ADXSJD_SetDecPos(void* self, u32 val) { *(u32*)((u8*)self + 0x34) = val; }

void ADXSJD_SetLnkSw(void* self, u32 val) {
    *(u32*)((u8*)self + 0xA4) = val;
    ADXB_SetLnkSw(*(void**)((u8*)self + 0x04), val);
}

void ADXSJD_SetDefFmt(void* self) {
    ADXB_SetDefFmt(*(void**)((u8*)self + 0x04));
}

void ADXSJD_EntryFltFunc(void* self, void* func, void* ctx) {
    *(void**)((u8*)self + 0x50) = func;
    *(void**)((u8*)self + 0x54) = ctx;
}

void ADXSJD_EntryTrapFunc(void* self, void* func, void* ctx) {
    *(void**)((u8*)self + 0x48) = func;
    *(void**)((u8*)self + 0x4C) = ctx;
}

void ADXSJD_SetTrapNumSmpl(void* self, u32 val) { *(u32*)((u8*)self + 0x3C) = val; }

void ADXSJD_SetTrapCnt(void* self, u32 val) { *(u32*)((u8*)self + 0x40) = val; }

void ADXSJD_SetTrapDtLen(void* self, u32 val) { *(u32*)((u8*)self + 0x44) = val; }

s32 ADXSJD_GetFormat(void* self) {
    return ADXB_GetFormat(*(void**)((u8*)self + 0x04));
}

u32 ADXSJD_GetSfreq(void* self) {
    return ADXB_GetSfreq(*(void**)((u8*)self + 0x04));
}

void ADXSJD_GetNumChan(void* self) {
    ADXB_GetNumChan(*(void**)((u8*)self + 0x04));
}

void ADXSJD_GetOutBps(void* self) {
    ADXB_GetOutBps(*(void**)((u8*)self + 0x04));
}

u32 ADXSJD_GetBlkSmpl(void* self) {
    return ADXB_GetBlkSmpl(*(void**)((u8*)self + 0x04));
}

u32 ADXSJD_GetTotalNumSmpl(void* self) {
    return ADXB_GetTotalNumSmpl(*(void**)((u8*)self + 0x04));
}

s16 ADXSJD_GetNumLoop(void* self) {
    return ADXB_GetNumLoop(*(void**)((u8*)self + 0x04));
}

u32 ADXSJD_GetLpStartPos(void* self) {
    return ADXB_GetLpStartPos(*(void**)((u8*)self + 0x04));
}

u32 ADXSJD_GetLpStartOfst(void* self) {
    if (self == NULL) return 0;
    return ADXB_GetLpStartOfst(*(u32*)((u8*)self + 0x04));
}

u32 ADXSJD_GetLpEndPos(void* self) {
    return ADXB_GetLpEndPos(*(void**)((u8*)self + 0x04));
}

u32 ADXSJD_GetLpEndOfst(void* self) {
    return ADXB_GetLpEndOfst(*(void**)((u8*)self + 0x04));
}

s32 ADXSJD_GetDefOutVol(void* self) {
    if (ADXB_GetAinfLen(*(void**)((u8*)self + 0x04)) > 0) {
        u8 st = *(u8*)((u8*)self + 0x01);
        if ((u8)(st - 2) <= 1) {
            return ADXB_GetDefOutVol(*(void**)((u8*)self + 0x04));
        }
    }
    return 0;
}

s32 ADXSJD_GetDefPan(void* self, s32 pan) {
    if (ADXB_GetAinfLen(*(void**)((u8*)self + 0x04)) > 0) {
        u8 st = *(u8*)((u8*)self + 0x01);
        if ((u8)(st - 2) <= 1) {
            return ADXB_GetDefPan(*(void**)((u8*)self + 0x04), pan);
        }
    }
    return -128;
}

void* ADXSJD_GetSpsdInfo(void* self) { return (void*)((u8*)self + 0x60); }

void ADXSJD_TakeSnapshot(void* self) {
    ADXB_TakeSnapshot(*(void**)((u8*)self + 0x04));
}

void ADXSJD_RestoreSnapshot(void* self) {
    ADXB_RestoreSnapshot(*(void**)((u8*)self + 0x04));
}
