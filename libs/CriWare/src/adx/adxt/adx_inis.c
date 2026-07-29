// Decompiled ADXT initialization/finalization module.
// Matches retail ADXT_Init, ADXT_Finish, adxini_rnaerr_cbfn, adxini_lscerr_cbfn.

#include <harness_catalog.h>
#include <string.h>
#include <adx/adxt/adx_inis.hpp>

// Forward declarations for SVM functions
s32 SVM_SetCbSvrIdWithString(s32 id, s32 arg, int (*cb)(void), s32 arg2,
                             const char* str);
s32 SVM_SetCbSvrWithString(s32 id, int (*cb)(void), s32 arg, const char* str);
void SVM_DelCbSvr(s32 id, s32 arg);
void SVM_CallErr1(const char* str);
void SVM_Init(void);
void SVM_Finish(void);

// Forward declarations for ADX subsystem init/finish
void ADXCRS_Init(void);
void ADXCRS_Finish(void);
void ADXCRS_Lock(void);
void ADXCRS_Unlock(void);
void SJUNI_Init(void);
void SJUNI_Finish(void);
void SJRBF_Init(void);
void SJRBF_Finish(void);
void SJMEM_Init(void);
void SJMEM_Finish(void);
void ADXERR_Init(void);
void ADXERR_Finish(void);
void ADXERR_CallErrFunc1_(void* param);
void ADXSTM_Init(void);
void ADXSTM_Finish(void);
void ADXSJD_Init(void);
void ADXSJD_Finish(void);
void ADXF_Init(void);
void ADXF_Finish(void);
void ADXRNA_Init(void);
void ADXRNA_Finish(void);
void ADXRNA_EntryErrFunc(void (*cb)(void*, void*), void* ctx);
void LSC_Init(void);
void LSC_Finish(void);
void LSC_EntryErrFunc(void (*cb)(void*, void*), void* ctx);
void ADXT_DestroyAll(void);
void ADXT_Destroy(ADXT_Tsvr* hndl);
void ADXT_SetDefSvrFreq(s32 freq);
char* criCrw_GetVersion(void);
void ADXT_ExecServer(void);
void ADXT_ExecLscSvr(void);
void ADXT_ExecFsSvr(void);

// Callback for ADXRNA error handling.
// Forwards the error param to ADXERR_CallErrFunc1_; ctx is unused.
void adxini_rnaerr_cbfn(void* ctx, void* param) {
    ADXERR_CallErrFunc1_(param);
}

// Callback for LSC error handling.
// Forwards the error param to ADXERR_CallErrFunc1_; ctx is unused.
void adxini_lscerr_cbfn(void* ctx, void* param) {
    ADXERR_CallErrFunc1_(param);
}

int adxt_exec_main_thrd(void) {
    ADXT_ExecLscSvr();
    return 0;
}

int adxt_exec_tsvr(void) {
    ADXT_ExecServer();
    return 0;
}

int adxt_exec_fssvr(void) {
    ADXT_ExecFsSvr();
    return 0;
}

// One-time initialization of all ADX subsystems.
// Uses a refcount so repeated calls are no-ops until ADXT_Finish matches.
void ADXT_Init(void) {
    struct AdxInisContext* ctx = &lbl_eu_805E26C8;

    ctx->field_0x18 = (u8*)lbl_eu_80515FB8;
    criCrw_GetVersion();

    // Only perform full init on the first call (refcount == 0).
    if (ctx->refcount != 0) {
        ctx->refcount += 1;
        return;
    }

    ADXCRS_Init();
    ADXCRS_Lock();
    SJUNI_Init();
    SJRBF_Init();
    SJMEM_Init();
    ADXERR_Init();
    ADXSTM_Init();
    ADXSJD_Init();
    ADXF_Init();
    ADXRNA_Init();
    LSC_Init();
    SVM_Init();

    // Register error callbacks for RNA and LSC subsystems.
    ADXRNA_EntryErrFunc(adxini_rnaerr_cbfn, NULL);
    LSC_EntryErrFunc(adxini_lscerr_cbfn, NULL);

    // Clear the handle array.
    memset(ctx->handles, 0, sizeof(ctx->handles));

    // Register SVM server callbacks.
    SVM_SetCbSvrIdWithString(2, 1, adxt_exec_tsvr, 0, lbl_eu_80516010);
    ctx->handle_0x10 =
        SVM_SetCbSvrWithString(4, adxt_exec_fssvr, 0, lbl_eu_80516010 + 0x0f);
    ctx->handle_0x08 =
        SVM_SetCbSvrWithString(5, adxt_exec_main_thrd, 0,
                               lbl_eu_80516010 + 0x1f);

    ctx->field_0x14 = 0;
    ctx->field_0x0C = 0;
    ADXT_SetDefSvrFreq(0x3c);
    ADXCRS_Unlock();

    ctx->refcount += 1;
}

// Tears down all ADX subsystems when refcount reaches zero.
void ADXT_Finish(void) {
    struct AdxInisContext* ctx = &lbl_eu_805E26C8;
    const char* str;
    struct AdxInisHandle* hndl;
    s32 i;

    // If refcount is already zero, report error and return.
    if (ctx->refcount == 0) {
        SVM_CallErr1(lbl_eu_80516010 + 0x33);
        return;
    }

    // Decrement refcount; only perform full teardown when it reaches zero.
    ctx->refcount -= 1;
    if (ctx->refcount != 0) {
        return;
    }

    ADXT_DestroyAll();
    ADXRNA_Finish();
    ADXF_Finish();
    ADXSTM_Finish();
    LSC_Finish();

    ADXCRS_Lock();
    SVM_DelCbSvr(2, 1);
    SVM_DelCbSvr(4, ctx->handle_0x10);
    SVM_DelCbSvr(5, ctx->handle_0x08);
    SVM_Finish();
    ADXSJD_Finish();
    ADXERR_Finish();
    SJMEM_Finish();
    SJRBF_Finish();
    SJUNI_Finish();
    ADXCRS_Unlock();
    ADXCRS_Finish();

    // Destroy any active handles that remain in the array.
    str = lbl_eu_80516010;
    hndl = ctx->handles;
    for (i = 0; i < 16; i++) {
        if (hndl->flag != 0) {
            SVM_CallErr1(str + 0x7e);
            ADXT_Destroy((ADXT_Tsvr*)hndl);
        }
        hndl++;
    }
}
