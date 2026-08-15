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
extern volatile s32 lbl_eu_805E64C0;

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
    (void)lbl_eu_80517598;
    (void)lbl_eu_80517598;

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
    AHXSJD* sjd;
    void* bsr;
    void* dcd;
    s32 sjdSize, bsrEnd, dcdEnd;
    s32 i;

    sjdSize = ROUND_UP(bufSize, 8);
    sjd = (AHXSJD*)sjdSize;
    memset(sjd, 0, 0x5C);

    bsrEnd = ROUND_UP(sjdSize + 0x63, 8) + 0x34;
    if (bsrEnd > bufSize + extraSize) {
        return NULL;
    }

    bsr = AHXBSR_Create(allocator, (void*)sjdSize, 0x34);
    if (bsr == NULL) {
        return NULL;
    }
    sjd->bsr = bsr;

    dcdEnd = ROUND_UP(bsrEnd + 7, 8) + 0x1BF0;
    if (dcdEnd > bufSize + extraSize) {
        AHXBSR_Destroy(bsr);
        return NULL;
    }

    dcd = AHXDCD_Create((void*)ROUND_UP(bsrEnd + 7, 8), 0x1BF0);
    if (dcd == NULL) {
        AHXBSR_Destroy(bsr);
        return NULL;
    }
    sjd->dcd = dcd;

    sjd->numChannels = numChannels;
    sjd->streamObj = allocator;

    // Copy channel info
    if (numChannels > 0) {
        u32* src = (u32*)chanInfo;
        u32* dst = (u32*)&sjd->chanInfo[0];

        // Unrolled copy for 8+ channels
        if (numChannels > 8) {
            s32 count = numChannels - 8;
            s32 idx = 0;
            for (i = 0; i < count; i += 8) {
                dst[0] = src[0];
                dst[1] = src[1];
                dst[2] = src[2];
                dst[3] = src[3];
                dst[4] = src[4];
                dst[5] = src[5];
                dst[6] = src[6];
                dst[7] = src[7];
                src += 8;
                dst += 8;
                idx += 8;
            }
            // Copy remaining
            for (i = idx; i < numChannels; i++) {
                *dst++ = *src++;
            }
        } else {
            for (i = 0; i < numChannels; i++) {
                *dst++ = *src++;
            }
        }
    }

    sjd->status = 0;
    sjd->termSupply = 0;
    sjd->outSmplOfst = 0;
    sjd->outSmplTotal = 0;
    sjd->decSmpl = 0x7FFFFFFF;
    sjd->decSmplLim = -1;
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

void criware_8038CB9C(void* self) {
    AHXSJD* sjd = (AHXSJD*)self;
    void* dcd = sjd->dcd;
    s32 outSmpl = AHXDCD_GetOutSmpl(dcd);
    s32 outBps = AHXDCD_GetOutBps(dcd);
    s32 bytesPerSmpl = outBps >> 3;
    void* stream = sjd->streamObj;

    u32** vt = *(u32***)stream;
    s32 avail = ((s32 (*)(void*, s32))vt[9])(stream, 0);
    s32 needed = avail / bytesPerSmpl;

    if (needed < outSmpl) {
        return;
    }

    if (AHXDCD_IsEof(dcd)) {
        s32 result = AHXDCD_DecodeFrmHdr(dcd);
        if (result == 1) {
            if (sjd->lnkSw == 0) {
                sjd->status = 3;
                return;
            }
            sjd->outSmplTotal += sjd->outSmplOfst;
            sjd->outSmplOfst = 0;
            return;
        }
        if (result == -1) {
            return;
        }
    }

    u32 outBuf[4];
    vt = *(u32***)sjd->streamObj;
    ((void (*)(void*, s32, u32*, s32))vt[6])(sjd->streamObj, 0, outBuf, outSmpl * bytesPerSmpl);

    s32 actualSmpl = outBuf[1] / bytesPerSmpl;
    if (outSmpl != actualSmpl) {
        vt = *(u32***)sjd->streamObj;
        ((void (*)(void*, s32, u32*))vt[7])(sjd->streamObj, 0, outBuf);
        return;
    }

    s32 decoded = AHXDCD_DecodeData(dcd, (void*)outBuf[0], 0, outSmpl);

    s32 maxDec = 0x1E0;
    s32 decCount = (sjd->outSmplOfst >= maxDec) ? decoded : (decoded & ~(decoded >> 31));

    s32 totalSmpl = AHXDCD_GetTotalNumSmpl(dcd);
    if (sjd->outSmplOfst + decCount > totalSmpl) {
        decCount = totalSmpl - sjd->outSmplOfst;
    }

    s32 outBytes = decCount * bytesPerSmpl;
    if (outBuf[1] > outBytes) {
        outBuf[1] = outBytes;
    }

    u32 tmpBuf[2];
    tmpBuf[0] = outBuf[0];
    tmpBuf[1] = outBuf[1];

    if (sjd->fltFunc != NULL && decCount > 0) {
        ((void (*)(void*, s32, u32))sjd->fltFunc)(sjd->fltCtx, 0, tmpBuf[0]);
    }

    vt = *(u32***)sjd->streamObj;
    ((void (*)(void*, s32, u32*))vt[8])(sjd->streamObj, 1, outBuf);

    vt = *(u32***)sjd->streamObj;
    ((void (*)(void*, s32, u32*))vt[7])(sjd->streamObj, 0, tmpBuf);

    sjd->outSmplOfst += decoded;

    s32 bsrPos = AHXBSR_Tell(sjd->bsr);
    s32 bytes = (bsrPos + 7) >> 3;
    u32 decSmplLim = sjd->decSmplLim;

    sjd->decSmpl = bytes;
    sjd->outSmplOfst += decCount;
    sjd->outSmplTotal += decoded;
    sjd->decCallback += decoded;

    if (decSmplLim > 0 && sjd->decCallback >= decSmplLim) {
        if (sjd->decCallbackPrm != 0) {
            ((void (*)(u32))sjd->decCallbackPrm)(sjd->decCallback);
        }
    }

    if (sjd->lnkSw == 0 && sjd->outSmplOfst >= totalSmpl) {
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
