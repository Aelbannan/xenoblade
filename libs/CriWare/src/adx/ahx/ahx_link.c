// AHX link: ADXT AHX streaming attach/detach and per-frame execution.
// libs/CriWare/src/adx/ahx/ahx_link.c

#include <harness_catalog.h>
#include "libs/CriWare/src/adx/adxt/adx_sjd.h"

// CRI streaming I/O object backing the AHX work buffer. Opaque here: it is
// only passed through to the AHXSJD layer; the object itself exposes the
// AHXBSR_IoVtbl vtable (see ahx_bsr.c).
typedef struct AHXIO_ AHXIO;

// AHX Streaming Jingle Decoder (defined in ahx_sjd.c; opaque here).
typedef struct AHXSJD AHXSJD;

// AHX streaming sub-objects owned by an ADXT handle.
typedef struct ADXT_ {
    u32 field_0x00;
    ADXSJD* info;  /* 0x04 */
    u32 field_0x08;
    u32 field_0x0C;
    AHXIO* allocator;    /* 0x10 */
    u32 field_0x14;
    u32 chanInfo[8];     /* 0x18 */
} ADXT_;

// AHX link callbacks registered by ADXT_AttachAhx.
extern void (*lbl_eu_805E4F20)(ADXT_*);          /* detach */
extern void (*adxb_ahx_exec_one)(ADXB*);           /* exec one */
extern void (*adxb_ahx_term_supply)(AHXSJD*);         /* term supply */
extern void (*adxb_ahx_dec_smpl)(AHXSJD*, s32);    /* set dec sample */
extern void (*adxb_ahx_in_sj)(AHXSJD*, AHXIO*); /* set input sj */
extern void (*lbl_eu_805E535C)(AHXSJD*);         /* set ext prm */

extern void ADXERR_CallErrFunc1_(const char* msg);
extern void ADXT_Stop(ADXT_* handle);
extern void ADXM_Lock(void);
extern void ADXM_Unlock(void);
extern void AHXSJD_Init(void);
extern AHXSJD* AHXSJD_Create(AHXIO* allocator, s32 numChannels, u32* chanInfo,
                             s32 bufSize, s32 extraSize);
extern void AHXSJD_EntryFltFunc(AHXSJD* self, void* func, void* ctx);
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
void AHXLINK_ExecOneAhx(ADXB* handle);
void AHXLINK_TermSupply(AHXSJD* sjd);
void AHXLINK_SetDecSmpl(AHXSJD* sjd, s32 val);

void ADXT_AttachAhx(ADXT_* adxt, s32 bufSize, s32 extraSize) {
    ADXSJD* info;
    ADXB* ahx;
    AHXSJD* sjd;

    if (adxt == NULL || bufSize == 0) {
        ADXERR_CallErrFunc1_("E2005092203 : NULL pointer is passed.");
        return;
    }
    if (extraSize < 0x2000) {
        ADXERR_CallErrFunc1_("E2005092204 : Work area size is too short.");
        return;
    }

    ADXT_Stop(adxt);
    if (lbl_eu_805E4F20 != NULL) {
        lbl_eu_805E4F20(adxt);
    }

    AHXSJD_Init();
    info = adxt->info;
    ahx = info->adxb;
    sjd = AHXSJD_Create(adxt->allocator, 1, adxt->chanInfo, bufSize, extraSize);
    if (sjd == NULL) {
        ADXERR_CallErrFunc1_("E2005092202 : AHXSJD_Create function failed.");
        return;
    }

    ADXM_Lock();
    ahx->ainf = sjd;
    lbl_eu_805E4F20 = AHXLINK_DetachAhx;
    adxb_ahx_exec_one = AHXLINK_ExecOneAhx;
    adxb_ahx_term_supply = AHXLINK_TermSupply;
    adxb_ahx_dec_smpl = AHXLINK_SetDecSmpl;
    adxb_ahx_in_sj = AHXSJD_SetInSj;
    lbl_eu_805E535C = AHXSJD_SetExtPrm;
    AHXSJD_EntryFltFunc(sjd, info->dataCb, info->dataCbCtx);
    ADXM_Unlock();
}

// Forward the detach to the currently-registered AHX link detach callback.
void ADXT_DetachAhx(ADXT_* adxt) {
    if (lbl_eu_805E4F20 != NULL) {
        lbl_eu_805E4F20(adxt);
    }
}

void AHXLINK_DetachAhx(ADXT_* handle) {
    ADXSJD* info;
    ADXB* ahx;
    AHXSJD* sjd;

    if (handle == NULL) {
        ADXERR_CallErrFunc1_("E2005092205 : NULL pointer is passed.");
        return;
    }

    ADXT_Stop(handle);
    info = handle->info;
    ahx = info->adxb;
    sjd = ahx->ainf;

    if (sjd == NULL) return;

    ADXM_Lock();
    AHXSJD_Stop(sjd);
    AHXSJD_Destroy(sjd);
    ahx->ainf = NULL;
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

void AHXLINK_ExecOneAhx(ADXB* handle) {
    s32 i;
    AHXSJD* sjd;
    s32 stat;
    s32 delta;

    ADXM_Lock();
    sjd = handle->ainf;

    stat = AHXSJD_GetStat(sjd);
    if (stat == 0) {
        handle->decNumSmpl = 0;
        handle->readSmpl = 0;
        AHXSJD_Stop(sjd);
    }

    if ((s32)handle->stat == 1) {
        if (handle->lnkSw == 0) {
            AHXSJD_SetLnkSw(sjd, 0);
        } else {
            AHXSJD_SetLnkSw(sjd, 1);
        }
        handle->readSmpl = 0;
        AHXSJD_Start(sjd);
        handle->stat = 2;
    } else if ((s32)handle->stat == 2) {
        for (i = 0; i < (s32)handle->field_0xc0; i++) {
            AHXSJD_ExecHndl(sjd);
        }
        handle->decNumSmpl = AHXSJD_GetDecNumSmpl(sjd) - handle->readSmpl;
        delta = handle->decNumSmpl;
        handle->readSmpl = handle->readSmpl + delta;
        handle->decDtLen = AHXSJD_GetDecDtLen(sjd);
        stat = AHXSJD_GetStat(sjd);
        if (stat == 3) {
            AHXSJD_Stop(sjd);
            handle->stat = 0;
        }
    }

    ADXM_Unlock();
}


