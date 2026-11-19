// Auto-scaffolded catalog TU for CriWare/src/adx/adxt/adx_sjd
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include "libs/CriWare/src/adx/adxt/adx_sjd.h"

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
__declspec(section ".rodata") __attribute__((aligned(8))) u8 lbl_eu_805160B8[0x90] = {0x45,0x30,0x33,0x30,0x31,0x30,0x39,0x30,0x31,0x20,0x41,0x44,0x58,0x42,0x5F,0x44,0x65,0x63,0x6F,0x64,0x65,0x48,0x65,0x61,0x64,0x65,0x72,0x3A,0x20,0x00,0x43,0x61,0x6E,0x20,0x6E,0x6F,0x74,0x20,0x64,0x65,0x63,0x6F,0x64,0x65,0x20,0x74,0x68,0x69,0x73,0x20,0x66,0x69,0x6C,0x65,0x20,0x66,0x6F,0x72,0x6D,0x61,0x74,0x2E,0x00,0x45,0x30,0x37,0x30,0x32,0x31,0x39,0x30,0x31,0x20,0x61,0x64,0x78,0x73,0x6A,0x64,0x5F,0x64,0x65,0x63,0x65,0x78,0x65,0x63,0x5F,0x73,0x74,0x61,0x72,0x74,0x3A,0x20,0x00,0x38,0x20,0x6F,0x72,0x20,0x34,0x62,0x69,0x74,0x20,0x57,0x41,0x56,0x20,0x66,0x69,0x6C,0x65,0x20,0x63,0x61,0x6E,0x27,0x74,0x20,0x70,0x6C,0x61,0x79,0x62,0x61,0x63,0x6B,0x20,0x63,0x6F,0x6E,0x74,0x69,0x6E,0x75,0x6F,0x75,0x73,0x6C,0x79,0x2E,0x00};

/* Forward declarations */
void adxsjd_decode_prep(ADXSJD* self);
void adxsjd_get_wr(ADXSJD* self, u32* outHalf, u32* outNumSmpl, u32* outTrap);
void adxsjd_decexec_end(ADXSJD* self);
void adxsjd_insert_proc(ADXSJD* self);
void adxsjd_discard_proc(ADXSJD* self);
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
    ADXSJD* sjd;
    ADXSJD* p;
    s32 i;
    s32 j;
    s32 halfBuf;
    ADXB* adxb;

    /* Find a free slot in the 16-entry work area (0xB4 bytes per entry) */
    p = (ADXSJD*)lbl_eu_805E3358;
    sj0 = *(void**)sjArray;
    for (i = 0; i < 16; i++) {
        if (p->inUse == 0) break;
        p++;
    }
    if (i == 16) return NULL;
    sjd = (ADXSJD*)lbl_eu_805E3358 + i;

    bufPtr = SJRBF_GetBufPtr(sj0);
    halfBuf = SJRBF_GetBufSize(sj0) / 2;
    sjd->adxb = ADXB_Create((void*)numChan, bufPtr, halfBuf,
                                             halfBuf + SJRBF_GetXtrSize(sj0) / 2);
    adxb = sjd->adxb;
    if (adxb == NULL) return NULL;

    ADXB_EntryGetWrFunc(adxb, adxsjd_get_wr, sjd);
    sjd->inSj = stream;
    sjd->numChan = (s8)numChan;

    /* Copy channel SJ pointers */
    for (j = 0; j < numChan; j++) {
        sjd->chan[j] = (void*)((u32*)sjArray)[j];
    }

    /* Initialize state fields */
    sjd->status = 0;          /* status */
    sjd->hdrLen = 0;
    sjd->totalSmpl = 0;
    sjd->totalDtLen = 0;
    sjd->decPos = 0;
    sjd->maxDecSmpl = 0x7FFFFFFF;
    sjd->trapNumSmpl = -1;
    sjd->trapCnt = 0;
    sjd->trapDtLen = 0;
    sjd->ahxFlag = 0;
    sjd->insertCnt = 0;
    sjd->discardCnt = 0;
    sjd->trapFunc = 0;
    sjd->trapCtx = 0;
    sjd->dataCb = 0;
    sjd->dataCbCtx = 0;
    sjd->inUse = 1;          /* inUse */

    return sjd;
}

void ADXSJD_Destroy(ADXSJD* self) {
    ADXB* adxb;
    if (self == NULL) return;

    adxb = self->adxb;
    if (adxb != NULL) {
        self->adxb = NULL;
        ADXB_Destroy(adxb);
    }

    ADXCRS_Lock();
    memset(self, 0, 0xB4);
    ADXCRS_Unlock();
}

int ADXSJD_GetStat(ADXSJD* self) {
    return (s32)self->status;
}

void ADXSJD_SetInSj(ADXSJD* self, void* sj) {
    self->inSj = sj;
    ADXB_SetAhxInSj(self->adxb);
}

void ADXSJD_SetMaxDecSmpl(ADXSJD* self, u32 val) {
    self->maxDecSmpl = val;
    ADXB_SetAhxDecSmpl(self->adxb, val);
}

void ADXSJD_TermSupply(ADXSJD* self) {
    ADXB_AhxTermSupply(self->adxb);
}

void ADXSJD_Start(ADXSJD* self) {
    self->hdrLen = 0;
    self->totalSmpl = 0;
    self->totalDtLen = 0;
    self->decPos = 0;
    self->maxDecSmpl = 0x7FFFFFFF;
    self->trapNumSmpl = (u32)-1;
    self->trapCnt = 0;
    self->trapDtLen = 0;
    self->ahxFlag = 0;
    self->insertCnt = 0;
    self->discardCnt = 0;
    self->status = 1;
}

void ADXSJD_Stop(ADXSJD* self) {
    ADXB_Stop(self->adxb);
    self->status = 0;
}

void adxsjd_decode_prep(ADXSJD* self) {
    ADXB* adxb = self->adxb;
    void* inSj = self->inSj;
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
    if (adxb->defFmt == 0 && adxb->fmt != 0x0D) {
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

    if (adxb->defFmt == 0 && adxb->fmt != 0x0D) {
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
            self->status = 4;
            return;
        }
    } else {
        if (adxb->fmt == 0x0D) {
            ADXB_SetDefPrm(adxb);
            hdrLen = 0;
        } else {
            criware_eu_8038A864(adxb);
            hdrLen = 0;
        }
    }

    self->hdrLen = hdrLen;

    /* Notify the registered filter callback with the stream properties */
    if ((u32)self->fltFunc != 0) {
        s32 fltFmt = ADXB_GetFormat(adxb);
        s32 fltCh = ADXB_GetNumChan(adxb);
        s32 fltSfreq = ADXB_GetSfreq(adxb);
        u32 fltTotal = ADXB_GetTotalNumSmpl(adxb);
        ((void (*)(void*, s32, s32, s32, u32))(u32)self->fltFunc)(
            self->fltCtx, fltFmt, fltCh, fltSfreq, fltTotal);
    }

    /* AHX streams set a dedicated flag */
    if (ADXB_GetFormat(adxb) == 4) {
        self->ahxFlag = 1;
    }

    /* Format 2 carries inline SPSD info right after the header */
    if (ADXB_GetFormat(adxb) == 2) {
        memcpy(&self->spsd[0], chk.head.ptr,
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
    if (adxb->field_0xe0 != 0) {
        void (*cb)(void*, u32) = *(void (**)(void*, u32))&lbl_eu_805E3354;
        if (cb != NULL) {
            cb(adxb, adxb->sfreq);
        }
    }

    self->status = 2;
}

void adxsjd_get_wr(ADXSJD* self, u32* outHalf, u32* outNumSmpl, u32* outTrap) {
    u8* wr;
    u8* p;
    /* Channel 0 SJ kept live across the loop: reused for the buffer-pointer query */
    void* chan0;
    int i;
    p = (u8*)self;
    wr = (u8*)&self->chanChunk[0];
    i = 0;
    chan0 = self->chan[0];

    for (; i < ADXB_GetNumChan(self->adxb); i++) {
        void* chan = *(void**)(p + 0x0C);
        void (*getBufFn)(void*, s32, s32, void*) =
            (void (*)(void*, s32, s32, void*))(*(void***)chan)[6];
        getBufFn(chan, 0, 0x4000, wr);
        wr += 8;
        p += 4;
    }

    /* outHalf = distance of wr[0].ptr from the channel buffer start, halved */
    {
        s32 diff = (u32)self->chunk.ptr - (u32)SJRBF_GetBufPtr(chan0);
        *(s32*)outHalf = diff / 2;
    }

    /* outNumSmpl = min(wr[0].len / 2, maxDecSmpl) */
    {
        s32 half = self->chunk.len / 2;
        u32 maxSmpl = self->maxDecSmpl;
        if (half < (s32)maxSmpl) {
            maxSmpl = (u32)half;
        }
        *(u32*)outNumSmpl = maxSmpl;
    }

    /* outTrap = remaining samples until trap, or a large sentinel */
    {
        s32 trapNum = self->trapNumSmpl;
        if (trapNum >= 0) {
            *(u32*)outTrap = (u32)(trapNum - (s32)self->trapCnt);
        } else {
            *(u32*)outTrap = 0x1FFFFFFF;
        }
    }

    ADXB_GetPcmBuf(self->adxb);
}

/* Round-half-up signed division by 2, as emitted by MWCC for s32 x / 2:
 * (x + (u32)x >> 31) >> 1 */
void adxsjd_decexec_start(ADXSJD* sjd) {
    ADXB* adxb = sjd->adxb;
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

void adxsjd_decexec_end(ADXSJD* self) {
    s32 decDtLen;
    s32 decNumSmpl;
    u8* wr;
    void** chanP;
    s32 smplBytes;
    ADXB* adxb;
    s32 totalSmpl;
    s32 i;
    void* inSj;
    SjChunk headChunk;   /* sp+0x10: first split result */
    SjChunk tailChunk;   /* sp+0x08: second split result */

    inSj = self->inSj;
    adxb = self->adxb;
    totalSmpl = ADXB_GetTotalNumSmpl(adxb);
    decDtLen = ADXB_GetDecDtLen(adxb);
    decNumSmpl = ADXB_GetDecNumSmpl(adxb);

    /* Limit decoded samples unless looping PCM playback */
    if (ADXB_GetFormat(adxb) != 1 || (s32)self->pcmLoop != 1) {
        s32 remaining = totalSmpl - self->decPos;
        if (decNumSmpl < remaining) {
            remaining = decNumSmpl;
        }
        decNumSmpl = remaining;
    }

    SJ_SplitChunk(&self->chunk, decDtLen, &headChunk, &tailChunk);

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
    chanP = (void**)self;
    wr = (u8*)&self->chanChunk[0];
    smplBytes = decNumSmpl * 2;
    for (i = 0; i < ADXB_GetNumChan(self->adxb); i++) {
        SJ_SplitChunk(wr, smplBytes, &headChunk, &tailChunk);

        if (self->dataCb != NULL) {
            void (*cbFn)(void*, s32, void*, u32) =
                (void (*)(void*, s32, void*, u32))self->dataCb;
            cbFn(self->dataCbCtx, i, headChunk.ptr, headChunk.len);
        }

        {
            void* chan = chanP[3];
            void** vtbl = *(void***)chan;
            void (*commitFn)(void*, s32, void*) = (void (*)(void*, s32, void*))vtbl[8];
            commitFn(chan, 1, &headChunk);
        }
        {
            void* chan = chanP[3];
            void** vtbl = *(void***)chan;
            void (*unreadFn)(void*, s32, void*) = (void (*)(void*, s32, void*))vtbl[7];
            unreadFn(chan, 0, &tailChunk);
        }
        wr += 8;
        chanP++;
    }

    self->totalSmpl += decNumSmpl;
    self->totalDtLen += decDtLen;
    self->decPos += decNumSmpl;
    self->trapCnt += decNumSmpl;
    self->trapDtLen += decDtLen;
    ADXB_Reset(adxb);
}

void ADXSJD_ExecHndl(ADXSJD* sjd) {
    /* Insert proc: runs under the resource lock */
    if (sjd->insertCnt > 0) {
        ADXCRS_Lock();
        adxsjd_insert_proc(sjd);
        ADXCRS_Unlock();
    }

    /* State machine */
    if (sjd->status == 2) {
        ADXB* adxb = sjd->adxb;
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
            s16 fmt = adxb->fmt;
            if ((u16)(fmt - 0x0A) <= 2 || fmt == 0x14 || fmt == 0x0F) {
                /* Re-read the handle: decexec_end may have rebound it */
                adxb = sjd->adxb;
                u32 dtLen = ADXB_GetDecDtLen(adxb);
                u32 numSmpl = ADXB_GetDecNumSmpl(adxb);
                sjd->totalSmpl += numSmpl;
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

void adxsjd_insert_proc(ADXSJD* self) {
    u32 wrBuf[2];
    s32 i;
    void** p;
    s32 maxInsert;
    s32 half;

    p = (void**)self;
    i = 0;
    /* Pending insert samples converted to byte count */
    maxInsert = (u32)self->insertCnt * 2;

    /* Phase 1: query free space per channel and take the minimum */
    for (; i < (s32)self->numChan; i++) {
        ((void (*)(void*, s32, s32, void*))(*(void***)p[3])[6])(
            p[3], 0, 0x7FFFFFFF, wrBuf);
        maxInsert = (maxInsert < (s32)wrBuf[1]) ? maxInsert : (s32)wrBuf[1];
        {
            void* chan = p[3];
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
    for (; i < (s32)self->numChan; i++) {
        ((void (*)(void*, s32, s32, void*))(*(void***)p[3])[6])(
            p[3], 0, half * 2, wrBuf);
        memset((void*)wrBuf[0], 0, half * 2);
        {
            void* chan = p[3];
            ((void (*)(void*, s32, void*))(*(void***)chan)[8])(chan, 1, wrBuf);
        }
        p++;
    }

    self->insertCnt -= half;
}

void adxsjd_discard_proc(ADXSJD* self) {
    SjChunk chunk;
    void** chanP;
    s32 i;
    s32 maxDiscard;
    s32 half;

    chanP = (void**)self;
    i = 0;
    maxDiscard = self->discardCnt * 2;

    /* Phase 1: query free space per channel and take the minimum */
    for (; i < (s32)self->numChan; i++) {
        ((void (*)(void*, s32, s32, void*))(*(void***)chanP[3])[6])(
            chanP[3], 1, 0x7FFFFFFF, &chunk);
        maxDiscard = (maxDiscard < (s32)chunk.len) ? maxDiscard : (s32)chunk.len;
        {
            void* chan = chanP[3];
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
    for (; i < (s32)self->numChan; i++) {
        void* chan = chanP[3];
        ((void (*)(void*, s32, s32, void*))(*(void***)chan)[6])(chan, 1, half * 2, &chunk);
        {
            void* chan2 = chanP[3];
            ((void (*)(void*, s32, void*))(*(void***)chan2)[8])(chan2, 0, &chunk);
        }
        chanP++;
    }

    self->discardCnt -= half;
}

void ADXSJD_ExecServer(void) {
    u8* work = (u8*)&lbl_eu_805E3340;
    int i;

    /* Pre-server callback (indirect through stored function pointer) */
    {
        void (**preCb)(void*) = (void (**)(void*))(work + 0x04);
        if (*preCb != NULL) {
            (*preCb)(*(void**)(work + 0x08));
        }
    }

    /* Process each handle slot */
    for (i = 0; i < 16; i++) {
        ADXSJD* sjd2 = (ADXSJD*)(work + 0x18) + i;
        if (sjd2->inUse == 1) {
            /* Pending insert runs under the resource lock */
            if (sjd2->insertCnt > 0) {
                ADXCRS_Lock();
                adxsjd_insert_proc(sjd2);
                ADXCRS_Unlock();
            }

            if (sjd2->status == 2) {
                ADXB* adxb = sjd2->adxb;
                if (ADXB_GetStat(adxb) == 0) {
                    adxsjd_decexec_start(sjd2);
                }
                ADXB_ExecHndl(adxb);
                if (ADXB_GetStat(adxb) == 3) {
                    adxsjd_decexec_end(sjd2);
                }
                {
                    /* Formats 0x0A-0x0C, 0x0F and 0x14 accumulate block
                     * progress directly instead of via decexec_end */
                    s16 fmt = adxb->fmt;
                    if ((u16)(fmt - 0x0A) <= 2 || fmt == 0x14 || fmt == 0x0F) {
                        u32 dtLen;
                        u32 numSmpl;
                        ADXB* adxb2 = sjd2->adxb;
                        dtLen = ADXB_GetDecDtLen(adxb2);
                        numSmpl = ADXB_GetDecNumSmpl(adxb2);
                        sjd2->totalSmpl += numSmpl;
                        sjd2->totalDtLen += dtLen;
                        sjd2->decPos += numSmpl;
                    }
                }
            } else if (sjd2->status == 1) {
                adxsjd_decode_prep(sjd2);
            }

            /* Pending discard runs under the resource lock */
            if (sjd2->discardCnt > 0) {
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

u32 ADXSJD_GetDecDtLen(ADXSJD* self) { return self->totalDtLen; }

u32 ADXSJD_GetDecNumSmpl(ADXSJD* self) { return self->totalSmpl; }

void ADXSJD_SetDecPos(ADXSJD* self, u32 val) { self->decPos = val; }

void ADXSJD_SetLnkSw(ADXSJD* self, u32 val) {
    self->lnkSw = val;
    ADXB_SetLnkSw(self->adxb, val);
}

void ADXSJD_SetDefFmt(ADXSJD* self) {
    ADXB_SetDefFmt(self->adxb);
}

void ADXSJD_EntryFltFunc(ADXSJD* self, void* func, void* ctx) {
    self->fltFunc = func;
    self->fltCtx = ctx;
}

void ADXSJD_EntryTrapFunc(ADXSJD* self, void* func, void* ctx) {
    self->trapFunc = func;
    self->trapCtx = ctx;
}

void ADXSJD_SetTrapNumSmpl(ADXSJD* self, u32 val) { self->trapNumSmpl = val; }

void ADXSJD_SetTrapCnt(ADXSJD* self, u32 val) { self->trapCnt = val; }

void ADXSJD_SetTrapDtLen(ADXSJD* self, u32 val) { self->trapDtLen = val; }

s32 ADXSJD_GetFormat(ADXSJD* self) {
    return ADXB_GetFormat(self->adxb);
}

u32 ADXSJD_GetSfreq(ADXSJD* self) {
    return ADXB_GetSfreq(self->adxb);
}

void ADXSJD_GetNumChan(ADXSJD* self) {
    ADXB_GetNumChan(self->adxb);
}

void ADXSJD_GetOutBps(ADXSJD* self) {
    ADXB_GetOutBps(self->adxb);
}

u32 ADXSJD_GetBlkSmpl(ADXSJD* self) {
    return ADXB_GetBlkSmpl(self->adxb);
}

u32 ADXSJD_GetTotalNumSmpl(ADXSJD* self) {
    return ADXB_GetTotalNumSmpl(self->adxb);
}

s16 ADXSJD_GetNumLoop(ADXSJD* self) {
    return ADXB_GetNumLoop(self->adxb);
}

u32 ADXSJD_GetLpStartPos(ADXSJD* self) {
    return ADXB_GetLpStartPos(self->adxb);
}

u32 ADXSJD_GetLpStartOfst(ADXSJD* self) {
    if (self == NULL) return 0;
    return ADXB_GetLpStartOfst((u32)self->adxb);
}

u32 ADXSJD_GetLpEndPos(ADXSJD* self) {
    return ADXB_GetLpEndPos(self->adxb);
}

u32 ADXSJD_GetLpEndOfst(ADXSJD* self) {
    return ADXB_GetLpEndOfst(self->adxb);
}

s32 ADXSJD_GetDefOutVol(ADXSJD* self) {
    if (ADXB_GetAinfLen(self->adxb) > 0) {
        u8 st = (u8)self->status;
        if ((u8)(st - 2) <= 1) {
            return ADXB_GetDefOutVol(self->adxb);
        }
    }
    return 0;
}

s32 ADXSJD_GetDefPan(ADXSJD* self, s32 pan) {
    if (ADXB_GetAinfLen(self->adxb) > 0) {
        u8 st = (u8)self->status;
        if ((u8)(st - 2) <= 1) {
            return ADXB_GetDefPan(self->adxb, pan);
        }
    }
    return -128;
}

void* ADXSJD_GetSpsdInfo(ADXSJD* self) { return (void*)(&self->spsd[0]); }

void ADXSJD_TakeSnapshot(ADXSJD* self) {
    ADXB_TakeSnapshot(self->adxb);
}

void ADXSJD_RestoreSnapshot(ADXSJD* self) {
    ADXB_RestoreSnapshot(self->adxb);
}
