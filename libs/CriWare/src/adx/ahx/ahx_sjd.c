// AHX SJD (Streaming Jingle Decoder)
// libs/CriWare/src/adx/ahx/ahx_sjd.c

#include <harness_catalog.h>
#include <string.h>

typedef struct AHXSJD {
    /*0x00*/ void* dcd;
    /*0x04*/ void* bsr;
    /*0x08*/ u8 initialized;
    /*0x09*/ s8 status;
    /*0x0A*/ u8 numChannels;
    /*0x0B*/ u8 pad;
    /*0x0C*/ u32 termSupply;
    /*0x10*/ void* streamObj;
    /*0x14*/ u32 chanInfo[8];
    /*0x34*/ u32 decSmpl;
    /*0x38*/ u32 lnkSw;
    /*0x3C*/ u32 decSmplLim;
    /*0x40*/ u32 outSmplOfst;
    /*0x44*/ u32 outSmplTotal;
    /*0x48*/ u32 pad2;
    /*0x4C*/ u32 decCallback;
    /*0x50*/ u32 decCallbackPrm;
    /*0x54*/ void* fltFunc;
    /*0x58*/ void* fltCtx;
} AHXSJD;

extern volatile u32 lbl_eu_80517598;
// Kept non-volatile so MWCC allocates the address temporaries in the same
// order as retail (r3=counter base, r4=flag base) in AHXSJD_Init.
extern s32 lbl_eu_805E64C0;

extern void AHXTBL_GetAtblInfo(u32*, u32*);
extern void AHXTBL_GetMtblInfo(u32*, u32*);
extern void AHXTBL_GetFtblInfo(u32*, u32*);
extern void AHXTBL_GetWtblInfo(u32*, u32*);
extern void AHXSJD_SetupAtbl(u32, u32);
extern void AHXSJD_SetupMtbl(u32, u32);
extern void AHXSJD_SetupFtbl(u32, u32);
extern void AHXSJD_SetupWtbl(u32, u32);
extern void AHXDCD_Init(void);
extern void AHXDCD_Finish(void);
extern void* AHXBSR_Create(void*, void*, s32);
extern void AHXBSR_Destroy(void*);
extern void AHXBSR_Restruct(void*, void*);
extern void* AHXDCD_Create(void*, s32);
extern void AHXDCD_Destroy(void*);
extern void AHXDCD_Reset(void*);
extern void AHXDCD_SetBsr(void*, void*);
extern void AHXDCD_DecodeHeader(void*);
extern s32 AHXDCD_DecodeFrmHdr(void*);
extern s32 AHXDCD_IsEof(void*);
extern s32 AHXDCD_GetOutSmpl(void*);
extern s32 AHXDCD_GetOutBps(void*);
extern s32 AHXDCD_DecodeData(void*, void*, s32, s32);
extern s32 AHXDCD_GetTotalNumSmpl(void*);
extern void AHXDCD_SetExtPrm(void*);
extern s32 AHXBSR_Tell(void*);

void AHXSJD_Init(void) {
    u32 i0, i1;
    u32 f;

    // Two dead volatile reads of the table flag reproduce the retail load
    // schedule (the scheduler also emits one between compare and branch).
    f = lbl_eu_80517598;
    f = lbl_eu_80517598;

    if (lbl_eu_805E64C0 == 0) {
        AHXTBL_GetAtblInfo(&i0, &i1);
        AHXSJD_SetupAtbl(i0, i1);
        AHXTBL_GetMtblInfo(&i0, &i1);
        AHXSJD_SetupMtbl(i0, i1);
        AHXTBL_GetFtblInfo(&i0, &i1);
        AHXSJD_SetupFtbl(i0, i1);
        AHXTBL_GetWtblInfo(&i0, &i1);
        AHXSJD_SetupWtbl(i0, i1);
        AHXDCD_Init();
    }
    ++lbl_eu_805E64C0;
}

void AHXSJD_Finish(void) {
    if (lbl_eu_805E64C0 == 1) {
        AHXDCD_Finish();
    }
    --lbl_eu_805E64C0;
}

void* AHXSJD_Create(void* allocator, s32 numChannels, void* chanInfo, s32 bufSize, s32 extraSize) {
    // Offsets-as-pointers style: the struct "base" is the rounded work-area size.
    AHXSJD* sjd = (AHXSJD*)ROUND_UP(bufSize, 8);
    u32 bsrOfst, bsrEnd, dcdOfst;
    s32 total = bufSize + extraSize;
    s32 i;

    memset(sjd, 0, 0x5C);

    // Layout check: [sjd 0x5C][pad->8][bsr 0x34][pad->8][dcd 0x1BF0] must fit.
    bsrOfst = ROUND_UP((u32)sjd + 0x5C, 8);
    bsrEnd = bsrOfst + 0x34;
    if (bsrEnd > (u32)total) {
        return NULL;
    }

    sjd->bsr = AHXBSR_Create(allocator, (void*)bsrOfst, 0x34);
    if (sjd->bsr == NULL) {
        return NULL;
    }

    dcdOfst = ROUND_UP(bsrEnd, 8);
    if (dcdOfst + 0x1BF0 > (u32)total) {
        // Reload from the struct like retail (keeps bsr out of the regalloc pool)
        AHXBSR_Destroy(sjd->bsr);
        return NULL;
    }

    sjd->dcd = AHXDCD_Create((void*)dcdOfst, 0x1BF0);
    if (sjd->dcd == NULL) {
        // Reload from the struct like retail (keeps handles out of regalloc)
        AHXBSR_Destroy(sjd->bsr);
        return NULL;
    }

    sjd->numChannels = numChannels;
    sjd->streamObj = allocator;
    // Indexed access: MWCC's -O4 unroll emits the 8x mtctr block plus an
    // index-recomputed remainder loop.
    for (i = 0; i < numChannels; i++) {
        sjd->chanInfo[i] = ((u32*)chanInfo)[i];
    }

    sjd->status = 0;
    sjd->termSupply = 0;
    sjd->chanInfo[2] = 0;
    sjd->chanInfo[3] = 0;
    sjd->chanInfo[4] = 0;
    sjd->chanInfo[5] = 0;
    sjd->chanInfo[6] = 0;
    sjd->chanInfo[7] = 0;
    sjd->decSmpl = 0x7FFFFFFF;
    sjd->decSmplLim = -1;
    sjd->outSmplOfst = 0;
    sjd->outSmplTotal = 0;
    sjd->decCallback = 0;
    sjd->decCallbackPrm = 0;
    sjd->fltFunc = NULL;
    sjd->fltCtx = NULL;
    sjd->initialized = 1;

    return sjd;
}

void AHXSJD_Destroy(void* self) {
    AHXSJD* sjd = (AHXSJD*)self;
    void* bsr;
    void* dcd;

    if (sjd == NULL) return;

    bsr = sjd->bsr;
    if (bsr != NULL) {
        sjd->bsr = NULL;
        AHXBSR_Destroy(bsr);
    }

    dcd = sjd->dcd;
    if (dcd != NULL) {
        sjd->dcd = NULL;
        AHXDCD_Destroy(dcd);
    }

    memset(sjd, 0, 0x5C);
}

s32 AHXSJD_GetStat(void* self) {
    return ((AHXSJD*)self)->status;
}

void AHXSJD_SetInSj(void* self, void* val) {
    ((AHXSJD*)self)->streamObj = val;
}

void AHXSJD_Start(void* self) {
    AHXSJD* sjd = (AHXSJD*)self;
    void* stream;

    sjd->termSupply = 0;
    sjd->chanInfo[2] = 0;
    sjd->chanInfo[3] = 0;
    sjd->chanInfo[4] = 0;
    sjd->chanInfo[5] = 0;
    sjd->chanInfo[6] = 0;
    sjd->chanInfo[7] = 0;
    sjd->decSmpl = 0x7FFFFFFF;
    sjd->decSmplLim = -1;
    sjd->outSmplOfst = 0;
    sjd->outSmplTotal = 0;

    stream = sjd->streamObj;
    AHXBSR_Restruct(sjd->bsr, stream);
    AHXDCD_SetBsr(sjd->dcd, sjd->bsr);
    sjd->status = 1;
}

void AHXSJD_Stop(void* self) {
    AHXSJD* sjd = (AHXSJD*)self;
    AHXDCD_Reset(sjd->dcd);
    sjd->status = 0;
}

// Streaming decode step: request one frame's worth of data from the stream,
// decode it, run the optional filter, then update all progress bookkeeping.
// chan_info[] doubles as decoder state:
//   [0]=stream obj  [3]=accumulated decoded  [4]=byte pos/8
//   [5]=outSmplOfst [6]=outSmplTotal         [7]=callback accumulator
void criware_8038CB9C(AHXSJD* sjd) {
    void* dcd = sjd->dcd;
    s32 outSmpl = AHXDCD_GetOutSmpl(dcd);
    s32 bytesPerSmpl = AHXDCD_GetOutBps(dcd) / 8;

    void* stream = (void*)sjd->chanInfo[0];
    s32 avail = ((s32 (*)(void*, s32))(*(u32***)stream)[9])(stream, 0);
    if (avail / bytesPerSmpl < outSmpl) {
        return;
    }

    if (AHXDCD_IsEof(dcd)) {
        s32 result = AHXDCD_DecodeFrmHdr(dcd);
        if (result == 1) {
            if (sjd->lnkSw == 0) {
                sjd->status = 3;
                return;
            }
            // Linked stream: roll the pending offset into the total and restart.
            sjd->chanInfo[3] = 0;
            sjd->chanInfo[6] += sjd->chanInfo[5];
            sjd->chanInfo[5] = 0;
            return;
        } else if (result == -1) {
            return;
        }
    }

    u32 outBuf[2];
    u32 tmpBuf[2];
    ((void (*)(void*, s32, u32*, u32))(*(u32***)sjd->chanInfo[0])[6])((void*)sjd->chanInfo[0], 0, outBuf, outSmpl * bytesPerSmpl);

    if (outSmpl != (s32)outBuf[1] / bytesPerSmpl) {
        ((void (*)(void*, s32, u32*))(*(u32***)sjd->chanInfo[0])[7])((void*)sjd->chanInfo[0], 0, outBuf);
        return;
    }

    s32 decoded = AHXDCD_DecodeData(dcd, (void*)outBuf[0], 0, outSmpl);

    // Warm-up gate: no counted output until 480 samples have accumulated.
    s32 decCount = (sjd->chanInfo[3] < 0x1E0) ? 0 : decoded;

    s32 totalSmpl = AHXDCD_GetTotalNumSmpl(sjd->dcd);
    if (sjd->chanInfo[5] + decCount > totalSmpl) {
        decCount = totalSmpl - sjd->chanInfo[5];
    }

    // Clamp the returned byte count to the (possibly trimmed) sample count;
    // tmpBuf describes the leftover tail of the buffer.
    u32 need = decCount * bytesPerSmpl;
    tmpBuf[1] = outBuf[1];
    if (outBuf[1] > need) {
        outBuf[1] = need;
    }
    if (tmpBuf[1] != outBuf[1]) {
        tmpBuf[0] = outBuf[0] + outBuf[1];
    } else {
        tmpBuf[0] = 0;
    }

    if (sjd->fltFunc != NULL && decCount > 0) {
        ((void (*)(void*, s32, u32))sjd->fltFunc)(sjd->fltCtx, 0, outBuf[0]);
    }

    ((void (*)(void*, s32, u32*))(*(u32***)sjd->chanInfo[0])[8])((void*)sjd->chanInfo[0], 1, outBuf);

    ((void (*)(void*, s32, u32*))(*(u32***)sjd->chanInfo[0])[7])((void*)sjd->chanInfo[0], 0, tmpBuf);

    sjd->chanInfo[3] += decoded;

    s32 bytes = (AHXBSR_Tell(sjd->bsr) + 7) / 8;
    u32 decSmplLim = sjd->decSmplLim;

    sjd->chanInfo[4] = bytes;
    sjd->chanInfo[5] += decCount;
    sjd->chanInfo[7] += decoded;
    sjd->outSmplOfst += decoded;

    if (decSmplLim >= 0 && sjd->outSmplOfst >= decSmplLim) {
        if (sjd->decCallback != 0) {
            ((void (*)(u32))sjd->decCallback)(sjd->decCallbackPrm);
        }
    }

    if (sjd->lnkSw == 0 && sjd->chanInfo[5] >= totalSmpl) {
        sjd->status = 3;
    }
}

void AHXSJD_ExecHndl(void* self) {
    AHXSJD* sjd = (AHXSJD*)self;
    s32 status = sjd->status;

    if (status == 2) {
        criware_8038CB9C(sjd);
    } else if (status == 1) {
        void* stream = sjd->streamObj;
        u32** vt = *(u32***)stream;
        s32 avail = ((s32 (*)(void*, s32))vt[9])(stream, 1);
        if (avail >= 0x24) {
            AHXDCD_DecodeHeader(sjd->dcd);
            AHXDCD_DecodeFrmHdr(sjd->dcd);
            sjd->status = 2;
        }
    }
}

u32 AHXSJD_GetDecDtLen(void* self) {
    // Retail reads offset 0x24, which is the decode-data-length slot
    // (chan_info[4]); dec_smpl@0x34 is set via AHXSJD_SetDecSmpl instead.
    return ((AHXSJD*)self)->chanInfo[4];
}

u32 AHXSJD_GetDecNumSmpl(void* self) {
    AHXSJD* sjd = (AHXSJD*)self;
    // Retail: lwz 0x2C; lwz 0x28; add — chan_info[6] + chan_info[5].
    return sjd->chanInfo[6] + sjd->chanInfo[5];
}

void AHXSJD_EntryFltFunc(void* self, void* func, void* ctx) {
    ((AHXSJD*)self)->fltFunc = func;
    ((AHXSJD*)self)->fltCtx = ctx;
}

void AHXSJD_SetExtPrm(void* self) {
    AHXDCD_SetExtPrm(((AHXSJD*)self)->dcd);
}

void AHXSJD_SetDecSmpl(void* self, u32 val) {
    ((AHXSJD*)self)->decSmpl = val;
}

void AHXSJD_TermSupply(void* self) {
    ((AHXSJD*)self)->termSupply = 1;
}

void AHXSJD_SetLnkSw(void* self, s32 val) {
    if (val == 0) {
        ((AHXSJD*)self)->lnkSw = 0;
    } else {
        ((AHXSJD*)self)->lnkSw = 1;
    }
}
