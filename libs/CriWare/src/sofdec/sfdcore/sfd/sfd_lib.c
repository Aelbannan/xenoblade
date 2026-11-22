// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/sfd/sfd_lib
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include "libs/CriWare/src/sofdec/sfdcore/sfd/sfd_types.h"


/* Forward declarations for external functions */
extern void UTY_InitTmr(s32 enable);
extern void SJRBF_Init(void);
extern void SFPLY_Init(void);
extern void SFHDS_Init(void);
extern void criware_803BFD20(void);
extern void UTY_MemsetDword(void* dst, u32 val, u32 count);
extern void MEM_Copy(void* dst, const void* src, u32 size);
extern s32 SFTIM_Init(void* ctx, u32 param);
extern void SFBUF_Init(void* ctx);
extern s32 SFTRN_Init(void* ctx, u32 param);
extern const s32 SFPLY_cond_dfl[];

const char SFLIB_version_str[] = "\nCRI SFD/WII Ver.2.119 Build:Oct 28 2009 11:37:54\n\0Append: MW4302 WII30Jul2008Patch04\n";

/* Trace-record name strings (one object each, in first-use order). */
static const char sTrFunc_SFD_Init[] = "SFD_Init"; // name-guess
static const char sTrArg_vsync_mhz[] = "vsync_mhz"; // name-guess
static const char sTrArg_sfd_errcode[] = "sfd_errcode"; // name-guess
static const char sTrFunc_SFD_Create[] = "SFD_Create"; // name-guess
static const char sTrArg_cpara_members[] = "cpara_members"; // name-guess
static const char sTrArg_sfdhn[] = "sfdhn"; // name-guess
static const char sTrFunc_SFD_Destroy[] = "SFD_Destroy"; // name-guess
static const char sTrFunc_SFD_Start[] = "SFD_Start"; // name-guess
static const char sTrFunc_SFD_Stop[] = "SFD_Stop"; // name-guess
static const char sTrFunc_SFD_RequestStop[] = "SFD_RequestStop"; // name-guess
static const char sTrFunc_SFD_TermSupply[] = "SFD_TermSupply"; // name-guess
static const char sTrFunc_SFD_GetFrm[] = "SFD_GetFrm"; // name-guess
static const char sTrArg_frminf[] = "frminf"; // name-guess
static const char sTrArg_time[] = "time"; // name-guess
static const char sTrArg_ptype[] = "ptype"; // name-guess
static const char sTrFunc_SFD_RelFrm[] = "SFD_RelFrm"; // name-guess
static const char sTrFunc_SFD_ExecOne[] = "SFD_ExecOne"; // name-guess
static const char sTrFunc_SFLIB_SetErr[] = "SFLIB_SetErr"; // name-guess
static const char sTrFunc_SFMPS_ExecServer[] = "SFMPS_ExecServer"; // name-guess
static const char sTrArg_supplied[] = "supplied"; // name-guess
static const char sTrArg_consumed[] = "consumed"; // name-guess
static const char sTrArg_discarded[] = "discarded"; // name-guess
static const char sTrFunc_SFMPV_ExecServer[] = "SFMPV_ExecServer"; // name-guess
static const char sTrFunc_SFADXT_ExecServer[] = "SFADXT_ExecServer"; // name-guess
static const char sTrFunc_sfmpv_DecodePicAtr[] = "sfmpv_DecodePicAtr"; // name-guess
static const char sTrArg_mpvhn[] = "mpvhn"; // name-guess
static const char sTrArg_flowcnt[] = "flowcnt"; // name-guess
static const char sTrArg_rsiz[] = "rsiz"; // name-guess
static const char sTrArg_mpverr[] = "mpverr"; // name-guess
static const char sTrArg_sfderr[] = "sfderr"; // name-guess
static const char sTrFunc_sfmpv_DecodeFrm[] = "sfmpv_DecodeFrm"; // name-guess
static const char sTrArg_goporder[] = "goporder"; // name-guess
static const char sTrArg_disp_order_in_gop[] = "disp_order_in_gop"; // name-guess
static const char sTrArg_errcnt[] = "errcnt"; // name-guess
static const char sTrArg_rcvcnt[] = "rcvcnt"; // name-guess
static const char sTrArg_num_slices[] = "num_slices"; // name-guess
static const char sTrFunc_sfmpv_DecodePicAtr_ret[] = "sfmpv_DecodePicAtr_ret"; // name-guess
static const char sTrArg_dec_ttu_time[] = "dec_ttu.time"; // name-guess
static const char sTrFunc_sfply_AdjustPrepEnd[] = "sfply_AdjustPrepEnd"; // name-guess
static const char sTrArg_vplay[] = "vplay"; // name-guess
static const char sTrArg_aplay[] = "aplay"; // name-guess
static const char sTrArg_sync_mode[] = "sync_mode"; // name-guess
static const char sTrArg_etrg[] = "etrg"; // name-guess
static const char sTrFunc_sfd_isSkip[] = "sfd_isSkip"; // name-guess
static const char sTrArg_reason[] = "reason"; // name-guess
static const char sTrFunc_sfd_waitSupply[] = "sfd_waitSupply"; // name-guess
static const char sTrFunc_sfd_waitRelFrm[] = "sfd_waitRelFrm"; // name-guess
static const char sTrFunc_sfmpv_ConcatSub[] = "sfmpv_ConcatSub"; // name-guess
static const char sTrArg_concat_cnt[] = "concat_cnt"; // name-guess
static const char sTrArg_add_time[] = "add_time"; // name-guess
static const char sTrArg_concat_time[] = "concat_time"; // name-guess
static const char sTrArg_concat_framecount[] = "concat_framecount"; // name-guess
static const char sTrFunc_sftim_UpdateTime[] = "sftim_UpdateTime"; // name-guess
static const char sTrArg_vtime[] = "vtime"; // name-guess
static const char sTrArg_vtunit[] = "vtunit"; // name-guess

SfdTrcRec sTraceSfdInit = {
    sTrFunc_SFD_Init,
    { 0, 1, { { NULL, 0x4, sTrArg_vsync_mhz } } },
    { 0, 1, { { NULL, 0x5, sTrArg_sfd_errcode } } },
};

SfdTrcRec sTraceSfdCreate = {
    sTrFunc_SFD_Create,
    { 0, 1, { { NULL, 0xA, sTrArg_cpara_members } } },
    { 0, 1, { { NULL, 0xB, sTrArg_sfdhn } } },
};

SfdTrcRec sTraceSfdDestroy = {
    sTrFunc_SFD_Destroy,
    { 0, 1, { { NULL, 0xB, sTrArg_sfdhn } } },
    { 0, 0 },
};

SfdTrcRec sTraceSfdStart = {
    sTrFunc_SFD_Start,
    { 0, 1, { { NULL, 0xB, sTrArg_sfdhn } } },
    { 0, 0 },
};

SfdTrcRec sTraceSfdStop = {
    sTrFunc_SFD_Stop,
    { 0, 1, { { NULL, 0xB, sTrArg_sfdhn } } },
    { 0, 0 },
};

SfdTrcRec sTraceSfdRequestStop = {
    sTrFunc_SFD_RequestStop,
    { 0, 1, { { NULL, 0xB, sTrArg_sfdhn } } },
    { 0, 0 },
};

SfdTrcRec sTraceSfdTermSupply = {
    sTrFunc_SFD_TermSupply,
    { 0, 1, { { NULL, 0xB, sTrArg_sfdhn } } },
    { 0, 0 },
};

SfdTrcRec sTraceSfdGetFrm = {
    sTrFunc_SFD_GetFrm,
    { 0, 1, { { NULL, 0xB, sTrArg_sfdhn } } },
    { 0, 3, { { NULL, 0xB, sTrArg_frminf }, { NULL, 0x4, sTrArg_time }, { NULL, 0x4, sTrArg_ptype } } },
};

SfdTrcRec sTraceSfdRelFrm = {
    sTrFunc_SFD_RelFrm,
    { 0, 2, { { NULL, 0xB, sTrArg_sfdhn }, { NULL, 0xB, sTrArg_frminf } } },
    { 0, 0 },
};

SfdTrcRec sTraceSfdExecOne = {
    sTrFunc_SFD_ExecOne,
    { 0, 1, { { NULL, 0xB, sTrArg_sfdhn } } },
    { 0, 0 },
};

SfdTrcRec sTraceSflibSetErr = {
    sTrFunc_SFLIB_SetErr,
    { 0, 2, { { NULL, 0xB, sTrArg_sfdhn }, { NULL, 0x5, sTrArg_sfd_errcode } } },
    { 0, 0 },
};

SfdTrcRec sTraceSfmpsExecServer = {
    sTrFunc_SFMPS_ExecServer,
    { 0, 1, { { NULL, 0xB, sTrArg_sfdhn } } },
    { 0, 3, { { NULL, 0x6, sTrArg_supplied }, { NULL, 0x6, sTrArg_consumed }, { NULL, 0x6, sTrArg_discarded } } },
};

SfdTrcRec sTraceSfmpvExecServer = {
    sTrFunc_SFMPV_ExecServer,
    { 0, 1, { { NULL, 0xB, sTrArg_sfdhn } } },
    { 0, 3, { { NULL, 0x6, sTrArg_supplied }, { NULL, 0x6, sTrArg_consumed }, { NULL, 0x6, sTrArg_discarded } } },
};

SfdTrcRec sTraceSfadxtExecServer = {
    sTrFunc_SFADXT_ExecServer,
    { 0, 1, { { NULL, 0xB, sTrArg_sfdhn } } },
    { 0, 3, { { NULL, 0x6, sTrArg_supplied }, { NULL, 0x6, sTrArg_consumed }, { NULL, 0x6, sTrArg_discarded } } },
};

SfdTrcRec sTraceDecodePicAtr = {
    sTrFunc_sfmpv_DecodePicAtr,
    { 0, 2, { { NULL, 0xB, sTrArg_mpvhn }, { NULL, 0x4, sTrArg_flowcnt } } },
    { 0, 3, { { NULL, 0x4, sTrArg_rsiz }, { NULL, 0x4, sTrArg_mpverr }, { NULL, 0x4, sTrArg_sfderr } } },
};

SfdTrcRec sTraceDecodeFrm = {
    sTrFunc_sfmpv_DecodeFrm,
    { 0, 4, { { NULL, 0xB, sTrArg_mpvhn }, { NULL, 0x4, sTrArg_ptype }, { NULL, 0x4, sTrArg_goporder }, { NULL, 0x4, sTrArg_disp_order_in_gop } } },
    { 0, 4, { { NULL, 0x4, sTrArg_rsiz }, { NULL, 0x4, sTrArg_errcnt }, { NULL, 0x4, sTrArg_rcvcnt }, { NULL, 0x4, sTrArg_num_slices } } },
};

SfdTrcRecIn sTraceDecodePicAtrRet = {
    sTrFunc_sfmpv_DecodePicAtr_ret,
    { 0, 4, { { NULL, 0x4, sTrArg_ptype }, { NULL, 0x4, sTrArg_goporder }, { NULL, 0x4, sTrArg_disp_order_in_gop }, { NULL, 0x4, sTrArg_dec_ttu_time } } },
};

SfdTrcRecIn sTraceAdjustPrepEnd = {
    sTrFunc_sfply_AdjustPrepEnd,
    { 0, 5, { { NULL, 0xB, sTrArg_sfdhn }, { NULL, 0x4, sTrArg_vplay }, { NULL, 0x4, sTrArg_aplay }, { NULL, 0x4, sTrArg_sync_mode }, { NULL, 0x4, sTrArg_etrg } } },
};

SfdTrcRecIn sTraceSfdIsSkip = {
    sTrFunc_sfd_isSkip,
    { 0, 2, { { NULL, 0xB, sTrArg_sfdhn }, { NULL, 0xA, sTrArg_reason } } },
};

SfdTrcRecIn sTraceSfdWaitSupply = {
    sTrFunc_sfd_waitSupply,
    { 0, 1, { { NULL, 0xB, sTrArg_sfdhn } } },
};

SfdTrcRecIn sTraceSfdWaitRelFrm = {
    sTrFunc_sfd_waitRelFrm,
    { 0, 1, { { NULL, 0xB, sTrArg_sfdhn } } },
};

SfdTrcRecIn sTraceSfmpvConcatSub = {
    sTrFunc_sfmpv_ConcatSub,
    { 0, 5, { { NULL, 0xB, sTrArg_sfdhn }, { NULL, 0x4, sTrArg_concat_cnt }, { NULL, 0x4, sTrArg_add_time }, { NULL, 0x4, sTrArg_concat_time }, { NULL, 0x4, sTrArg_concat_framecount } } },
};

SfdTrcRecIn sSfdTimTraceData = {
    sTrFunc_sftim_UpdateTime,
    { 0, 3, { { NULL, 0xB, sTrArg_sfdhn }, { NULL, 0x4, sTrArg_vtime }, { NULL, 0x4, sTrArg_vtunit } } },
};

extern u32 lbl_eu_80606E20[];
extern SfdSys sSfdGlob;
extern void* lbl_eu_8060715C;

s32 SFLIB_SetErr(s32 val, u32 err_code);
s32 SFLIB_CheckHn(void* h);
s32 fn_803C3400(void* cfg);

u32 SFD_IsVersionCompatible(void* self, u32 version) {
    return (version == 0x39b0) ? 1 : 0;
}

s32 fn_803C3320(void* cfg) {
    u32* ctx = lbl_eu_80606E20;
    s32 result;

    /* Notify listener object (virtual call: vtable method at +0x24) with event block */
    if (ctx[5] != 0) {
        u32* obj = (u32*)ctx[5];
        sTraceSfdInit.entry.args[0].value = (u8*)cfg + 4;
        (*(void (**)(void*, void*))((u8*)*obj + 0x24))(obj, &sTraceSfdInit.entry);
    }

    ctx[0xCD] = 0x39B0;
    ctx[0xCE] = (u32)SFLIB_version_str;

    UTY_InitTmr(1);
    SJRBF_Init();

    result = fn_803C3400(cfg);
    if (result == 0) {
        SFPLY_Init();
        SFHDS_Init();
        criware_803BFD20();
    }

    /* Second notification carries the init result pointer */
    if (ctx[5] != 0) {
        u32* obj = (u32*)ctx[5];
        sTraceSfdInit.exit.args[0].value = &result;
        (*(void (**)(void*, void*))((u8*)*obj + 0x24))(obj, &sTraceSfdInit.exit);
    }

    return result;
}

s32 fn_803C3400(void* cfg) {
    s32 error;
    SfdSys* sys = &sSfdGlob;

    UTY_MemsetDword(sys, 0, 0xC7);
    MEM_Copy(sys, SFPLY_cond_dfl, 0x190);
    sys->field_0x190 = *(u32*)cfg;
    sys->field_0x194 = ((u32*)cfg)[1];
    sys->field_0x198 = 0;
    sys->field_0x1f8 = 0x5A5A5A5A;

    error = 0;
    SFTIM_Init(&sys->tim, ((u32*)cfg)[1]);
    SFBUF_Init(&sys->field_0x1b0);

    sys->field_0x1f0 = 0;
    sys->field_0x1f4 = 0;
    sys->field_0x1fc[0] = 0;
    sys->field_0x1fc[1] = 0;
    sys->field_0x1fc[2] = 0;
    sys->field_0x1fc[3] = 0;
    sys->field_0x1fc[4] = 0;
    sys->field_0x1fc[5] = 0;
    sys->field_0x1fc[6] = 0;
    sys->field_0x1fc[7] = 0;

    s32 trnResult = SFTRN_Init(sys->trn, *(u32*)cfg);
    if (trnResult != 0) {
        error = trnResult;
    }
    return error;
}

typedef struct SfdErrorState {
    u32 field00;
    u32 field04;
    u32 field08;
    u32 field0C;
    u32 field10;
} SfdErrorState;

void fn_803C34DC(void* self) {
    SfdErrorState* state = (SfdErrorState*)self;
    state->field00 = 0;
    state->field04 = 0;
    state->field08 = 0;
    state->field0C = 0;
    state->field10 = 0;
}

s32 SFLIB_SetErr(s32 val, u32 err_code);
s32 fn_803C34F8(s32 val, u32 err_code) {
    return SFLIB_SetErr(val, err_code);
}

extern void* gpSfdCoreTraceCb;

s32 SFLIB_SetErr(s32 val, u32 err_code) {
    void (*fn)(void*, void*);

    if (err_code == 0) {
        return 0;
    }

    if (gpSfdCoreTraceCb != NULL) {
        sTraceSflibSetErr.entry.args[0].value = (void*)val;
        sTraceSflibSetErr.entry.args[1].value = &err_code;
        fn = *(void (**)(void*, void*))((u8*)*(u32*)gpSfdCoreTraceCb + 0x24);
        fn(gpSfdCoreTraceCb, &sTraceSflibSetErr.entry);
    }

    if (val == 0) {
        u32 err = err_code;
        if (lbl_eu_80606E20[2] == 0) {
            lbl_eu_80606E20[2] = err;
        }
        if (err != 0) {
            void (*cb)(u32, u32) = (void (*)(u32, u32))lbl_eu_80606E20[0];
            if (cb != NULL) {
                cb(lbl_eu_80606E20[1], err);
            }
        }
    } else {
        SfdHandle* handle = (SfdHandle*)val;
        u32 last = handle->errCode;
        u32 err = err_code;
        if (last == 0) {
            handle->errCode = err;
        }
        if (err != 0 && handle->errFn != NULL) {
            handle->errFn(handle->errFnArg, err);
        }
        if (handle->errorState > 0) {
            handle->errorState = -handle->errorState;
        }
    }

    if (gpSfdCoreTraceCb != NULL) {
        fn = *(void (**)(void*, void*))((u8*)*(u32*)gpSfdCoreTraceCb + 0x24);
        fn(gpSfdCoreTraceCb, &sTraceSflibSetErr.exit);
    }

    return err_code;
}

s32 SFLIB_CheckHn(void* h) {
    if (h == NULL) return -1;
    if (((SfdHandle*)h)->errorState == 0) return -1;
    lbl_eu_8060715C = h;
    return 0;
}

s32 criware_803C0D94(void* handle, void (*errFn)(u32, u32), u32 errArg) {
    SfdHandle* h = (SfdHandle*)handle;

    if (h == NULL) {
        lbl_eu_80606E20[0] = (u32)errFn;
        lbl_eu_80606E20[1] = errArg;
    } else {
        s32 ret = SFLIB_CheckHn(h);  // inlined by the compiler
        if (ret != 0)
            return SFLIB_SetErr(0, 0xFF000101);
        h->errFn = errFn;
        h->errFnArg = errArg;
    }
    return 0;
}

void SVM_Lock(void);
void SFLIB_LockCs(void* cs) {
    SVM_Lock();
}

void SVM_Unlock(void);
void SFLIB_UnlockCs(void* cs) {
    SVM_Unlock();
}
