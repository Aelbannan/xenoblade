// AHX link: ADXT AHX streaming attach/detach and per-frame execution.
// libs/CriWare/src/adx/ahx/ahx_link.c

#include <harness_catalog.h>

// CRI streaming I/O object backing the AHX work buffer. Opaque here: it is
// only passed through to the AHXSJD layer; the object itself exposes the
// AHXBSR_IoVtbl vtable (see ahx_bsr.c).
typedef struct AHXIO_ AHXIO;

// AHX Streaming Jingle Decoder (defined in ahx_sjd.c; opaque here).
typedef struct AHXSJD AHXSJD;

// Decoded-audio filter registered through ADXT_AttachAhx: invoked once per
// frame with the filter context and the output buffer address.
typedef struct AHXSJD_FltCtx_ AHXSJD_FltCtx;
typedef void (*AHXSJD_FltFunc)(AHXSJD_FltCtx* ctx, s32 mode, u32 outBuf);

// AHX streaming sub-objects owned by an ADXT handle.
// Layout recovered from retail usage across ahx_link.c / ahx_sjd.c / adx_bahx.c.
typedef struct AHX_ AHX_;
typedef struct ADXT_AHXInfo_ ADXT_AHXInfo;

typedef struct ADXT_ {
    u32 field_0x00;
    ADXT_AHXInfo* info;  /* 0x04 */
    u32 field_0x08;
    u32 field_0x0C;
    AHXIO* allocator;    /* 0x10 */
    u32 field_0x14;
    u32 chanInfo[8];     /* 0x18 */
} ADXT_;

struct ADXT_AHXInfo_ {
    u32 field_0x00;
    AHX_* ahx;               /* 0x04 */
    u8 _pad0[0x58 - 0x08];
    AHXSJD_FltFunc fltFunc;  /* 0x58 */
    AHXSJD_FltCtx* fltCtx;   /* 0x5C */
};

struct AHX_ {
    u32 field_0x00;
    s32 state;           /* 0x04: 0 idle, 1 start, 2 exec */
    u8 _pad0[0x88 - 0x08];
    s32 decSmpl;         /* 0x88 */
    s32 field_0x8C;
    s32 field_0x90;      /* samples decoded this frame */
    s32 field_0x94;      /* decode data length */
    u8 _pad1[0xAC - 0x98];
    s32 lnkSw;           /* 0xAC */
    u8 _pad2[0xB8 - 0xB0];
    AHXSJD* sjd;         /* 0xB8 */
    s32 field_0xBC;
    s32 field_0xC0;      /* handler exec count */
};

// AHX link callbacks registered by ADXT_AttachAhx.
extern void (*lbl_eu_805E4F20)(ADXT_*);          /* detach */
extern void (*lbl_eu_805E5354)(AHX_*);           /* exec one */
extern void (*lbl_eu_805E5350)(AHXSJD*);         /* term supply */
extern void (*lbl_eu_805E534C)(AHXSJD*, s32);    /* set dec sample */
extern void (*lbl_eu_805E5348)(AHXSJD*, AHXIO*); /* set input sj */
extern void (*lbl_eu_805E535C)(AHXSJD*);         /* set ext prm */

extern char lbl_eu_805174A0[];
extern void ADXERR_CallErrFunc1_(const char* msg);
extern void ADXT_Stop(ADXT_* handle);
extern void ADXM_Lock(void);
extern void ADXM_Unlock(void);
extern void AHXSJD_Init(void);
extern AHXSJD* AHXSJD_Create(AHXIO* allocator, s32 numChannels, u32* chanInfo,
                             s32 bufSize, s32 extraSize);
extern void AHXSJD_EntryFltFunc(AHXSJD* self, AHXSJD_FltFunc func,
                                AHXSJD_FltCtx* ctx);
extern void AHXSJD_SetInSj(AHXSJD* self, AHXIO* val);
extern void AHXSJD_SetExtPrm(AHXSJD* self);
extern void AHXSJD_Stop(AHXSJD* sjd);
extern void AHXSJD_Destroy(AHXSJD* sjd);
extern void AHXSJD_Finish(void);
extern void AHXSJD_Start(AHXSJD* sjd);
extern void AHXSJD_ExecHndl(AHXSJD* sjd);
extern s32 AHXSJD_GetStat(AHXSJD* sjd);
extern s32 AHXSJD_GetDecNumSmpl(AHXSJD* sjd);
extern s32 AHXSJD_GetDecDtLen(AHXSJD* sjd);
extern void AHXSJD_SetLnkSw(AHXSJD* sjd, s32 val);
extern void AHXSJD_SetDecSmpl(AHXSJD* sjd, s32 val);
extern void AHXSJD_TermSupply(AHXSJD* sjd);

void AHXLINK_DetachAhx(ADXT_* handle);
void AHXLINK_ExecOneAhx(AHX_* handle);
void AHXLINK_TermSupply(AHXSJD* sjd);
void AHXLINK_SetDecSmpl(AHXSJD* sjd, s32 val);

void ADXT_AttachAhx(ADXT_* adxt, s32 bufSize, s32 extraSize) {
    ADXT_AHXInfo* info;
    AHX_* ahx;
    AHXSJD* sjd;

    if (adxt == NULL || bufSize == 0) {
        ADXERR_CallErrFunc1_(lbl_eu_805174A0);
        return;
    }
    if (extraSize < 0x2000) {
        ADXERR_CallErrFunc1_(lbl_eu_805174A0 + 0x26);
        return;
    }

    ADXT_Stop(adxt);
    if (lbl_eu_805E4F20 != NULL) {
        lbl_eu_805E4F20(adxt);
    }

    AHXSJD_Init();
    info = adxt->info;
    ahx = info->ahx;
    sjd = AHXSJD_Create(adxt->allocator, 1, adxt->chanInfo, bufSize, extraSize);
    if (sjd == NULL) {
        ADXERR_CallErrFunc1_(lbl_eu_805174A0 + 0x51);
        return;
    }

    ADXM_Lock();
    ahx->sjd = sjd;
    lbl_eu_805E4F20 = AHXLINK_DetachAhx;
    lbl_eu_805E5354 = AHXLINK_ExecOneAhx;
    lbl_eu_805E5350 = AHXLINK_TermSupply;
    lbl_eu_805E534C = AHXLINK_SetDecSmpl;
    lbl_eu_805E5348 = AHXSJD_SetInSj;
    lbl_eu_805E535C = AHXSJD_SetExtPrm;
    AHXSJD_EntryFltFunc(sjd, info->fltFunc, info->fltCtx);
    ADXM_Unlock();
}

// Forward the detach to the currently-registered AHX link detach callback.
void ADXT_DetachAhx(ADXT_* adxt) {
    if (lbl_eu_805E4F20 != NULL) {
        lbl_eu_805E4F20(adxt);
    }
}

void AHXLINK_DetachAhx(ADXT_* handle) {
    ADXT_AHXInfo* info;
    AHX_* ahx;
    AHXSJD* sjd;

    if (handle == NULL) {
        ADXERR_CallErrFunc1_(lbl_eu_805174A0 + 0x7E);
        return;
    }

    ADXT_Stop(handle);
    info = handle->info;
    ahx = info->ahx;
    sjd = ahx->sjd;

    if (sjd == NULL) return;

    ADXM_Lock();
    AHXSJD_Stop(sjd);
    AHXSJD_Destroy(sjd);
    ahx->sjd = NULL;
    ADXM_Unlock();
    AHXSJD_Finish();
}

// Both register arguments are forwarded untouched via a bare tail branch
// (retail AHXLINK_SetDecSmpl has no frame).
void AHXLINK_SetDecSmpl(AHXSJD* sjd, s32 val) {
    AHXSJD_SetDecSmpl(sjd, val);
}

void AHXLINK_TermSupply(AHXSJD* sjd) {
    AHXSJD_TermSupply(sjd);
}

void AHXLINK_ExecOneAhx(AHX_* handle) {
    s32 i;
    AHXSJD* sjd;
    s32 stat;
    s32 delta;

    ADXM_Lock();
    sjd = handle->sjd;

    stat = AHXSJD_GetStat(sjd);
    if (stat == 0) {
        handle->field_0x90 = 0;
        handle->decSmpl = 0;
        AHXSJD_Stop(sjd);
    }

    if (handle->state == 1) {
        if (handle->lnkSw == 0) {
            AHXSJD_SetLnkSw(sjd, 0);
        } else {
            AHXSJD_SetLnkSw(sjd, 1);
        }
        handle->decSmpl = 0;
        AHXSJD_Start(sjd);
        handle->state = 2;
    } else if (handle->state == 2) {
        for (i = 0; i < handle->field_0xC0; i++) {
            AHXSJD_ExecHndl(sjd);
        }
        handle->field_0x90 = AHXSJD_GetDecNumSmpl(sjd) - handle->decSmpl;
        delta = handle->field_0x90;
        handle->decSmpl = handle->decSmpl + delta;
        handle->field_0x94 = AHXSJD_GetDecDtLen(sjd);
        stat = AHXSJD_GetStat(sjd);
        if (stat == 3) {
            AHXSJD_Stop(sjd);
            handle->state = 0;
        }
    }

    ADXM_Unlock();
}
