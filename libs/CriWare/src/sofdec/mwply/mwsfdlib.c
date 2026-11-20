// Auto-scaffolded catalog TU for CriWare/src/sofdec/mwply/mwsfdlib
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include "libs/CriWare/src/sofdec/sfdcore/sfd/sfd_types.h"

/* Real CriWare rodata objects (JP map names, global linkage). */
const struct { char build[0x30]; char append[0x24]; } mwsfd_ver_str = {{"\nMWSFD/WII Ver.4.95 Build:Oct 28 2009 21:13:31\n"}, {"Append: MW4302 WII30Jul2008Patch04\n"}};
const u32 mwsfd_trentry[0xF] = {0};
__attribute__((aligned(8))) const u32 mwsfd_initsfdpara[2] = {0, 0xEA24};
const u32 mwsfd_siz_mwplyhn = 0x690;
/* Real compiler string-table objects (JP map, local linkage). */
__attribute__((aligned(4))) static const struct { char s0[0x10]; char s1[0x14]; } sMwPlyInitSfdFx_09C = {{"mwPlyInitSfdFx"},{"mwPlyFinishSfdFx"}};
__attribute__((aligned(8))) static const struct { char s0[0x14]; char s1[0x10]; } sMwPlyCreateSofdec_0C0 = {{"mwPlyCreateSofdec"},{"cpara_members"}};
__attribute__((aligned(4))) static const struct { char s0[0x8]; char s1[0x14]; } sMwply_0E4 = {{"mwply"},{"mwPlyDestroy"}};
__attribute__((aligned(8))) static const struct { char s[0x10]; } sMwPlyStartFname_100 = {{"mwPlyStartFname"}};
__attribute__((aligned(8))) static const struct { char s[0x8]; } sFname_110 = {{"fname"}};
__attribute__((aligned(8))) static const struct { char s[0x18]; } sMwPlyStartFnameRange_118 = {{"mwPlyStartFnameRange"}};
__attribute__((aligned(8))) static const struct { char s0[0xC]; char s1[0xC]; } sOffset_sct_130 = {{"offset_sct"},{"range_sct"}};
__attribute__((aligned(8))) static const struct { char s[0x10]; } sMwPlyStartMem_148 = {{"mwPlyStartMem"}};
__attribute__((aligned(8))) static const struct { char s[0x8]; } sAddr_158 = {{"addr"}};
__attribute__((aligned(8))) static const struct { char s0[0x4]; char s1[0x10]; char s2[0x4]; } sLen_160 = {{"len"},{"mwPlyStartSj"},{"sji"}};
__attribute__((aligned(8))) static const struct { char s[0x10]; } sMwPlyStartAfs_178 = {{"mwPlyStartAfs"}};
__attribute__((aligned(8))) static const struct { char s[0x8]; } sPatid_188 = {{"patid"}};
__attribute__((aligned(8))) static const struct { char s0[0x4]; char s1[0x14]; } sFid_190 = {{"fid"},{"mwPlyStartFnameLp"}};
__attribute__((aligned(8))) static const struct { char s[0x10]; } sMwPlyReleaseLp_1A8 = {{"mwPlyReleaseLp"}};
__attribute__((aligned(8))) static const struct { char s[0x10]; } sMwPlyStartAfsLp_1B8 = {{"mwPlyStartAfsLp"}};
__attribute__((aligned(8))) static const struct { char s[0x18]; } sMwPlyStartFnameRangeLp_1C8 = {{"mwPlyStartFnameRangeLp"}};
__attribute__((aligned(8))) static const struct { char s[0x10]; } sMwPlyEntryFname_1E0 = {{"mwPlyEntryFname"}};
__attribute__((aligned(8))) static const struct { char s0[0x14]; char s1[0x18]; } sMwPlyStartSeamless_1F0 = {{"mwPlyStartSeamless"},{"mwPlyReleaseSeamless"}};
__attribute__((aligned(4))) static const struct { char s0[0x14]; char s1[0x8]; } sMwPlySetSeamlessLp_21C = {{"mwPlySetSeamlessLp"},{"flag"}};
__attribute__((aligned(8))) static const struct { char s0[0xC]; char s1[0x14]; } sMwPlyStop_238 = {{"mwPlyStop"},{"mwPlyRequestStop"}};
__attribute__((aligned(8))) static const struct { char s[0x10]; } sMWSFSVM_Error_258 = {{"MWSFSVM_Error"}};
__attribute__((aligned(8))) static const struct { char s[0x8]; } sErrstr_268 = {{"errstr"}};
__attribute__((aligned(8))) static const struct { char s[0x18]; } sMWSFSVM_GotoIdleBorder_270 = {{"MWSFSVM_GotoIdleBorder"}};
__attribute__((aligned(8))) static const struct { char s[0x18]; } sMWSFSVR_VsyncThrdProc_288 = {{"MWSFSVR_VsyncThrdProc"}};
__attribute__((aligned(8))) static const struct { char s0[0x14]; char s1[0x18]; } sRemain_work_flag_2A0 = {{"remain_work_flag"},{"MWSFSVR_MainThrdProc"}};
__attribute__((aligned(4))) static const struct { char s0[0x18]; char s1[0x18]; char s2[0x14]; } sMWSFSVR_IdleThrdProc_2CC = {{"MWSFSVR_IdleThrdProc"},{"MWSFSVR_DecodeServer"},{"mwsfd_ExecSvrHndl"}};
__attribute__((aligned(8))) static const struct { char name[0xC]; float unit; float scale; } sMwSfdVsyncPool = {{"mwSfdVsync"}, 0.5f, 1000.0f};
__attribute__((aligned(4))) static const struct { float fps; } sDefaultFps = {59.94f};


__attribute__((aligned(8))) unsigned char mwsfdlib_bss[0x3720];


/* ------------------------------------------------------------------ */
/* Type definitions                                                    */
/* ------------------------------------------------------------------ */

/* SFD library work area (0x805FF3A8). The lib work block is 0x10 bytes
 * into the global control block at 0x805FF398; functions that already
 * hold the control-block base reach the fields as members of MWSFDGlobal,
 * while helpers that only touch the lib work reference the 0x805FF3A8
 * symbol directly (retail keeps both views). */
typedef struct MWSFDLibWork {
    u8 _00[0x04];
    float fps;       /* 0x04 */
    u32 _08;         /* 0x08 */
    u32 _0C;         /* 0x0C */
    u32 _10;         /* 0x10 */
    u8 _14[0x38 - 0x14];
    u32 usePicUsr;   /* 0x38 */
    u32 pauseBdr;    /* 0x3C */
    u8 _40[0x5C - 0x40];
    u32 _5C;         /* 0x5C */
    u8 _60[0x68 - 0x60];
    u32 errCode;     /* 0x68 */
    u8 _6C[0x34F0 - 0x6C];
    u32 _34F0;       /* 0x34F0 */
} MWSFDLibWork;

/* SVM trace-callback infrastructure: the object's vtable exposes a method
 * at +0x24; each instrumented function stores its args into a per-function
 * record (sTraceInitSfdFx) and fires the callback on entry (+0x04) and
 * exit (+0x6C). Same shape as the mwsfdply TU's SfdTraceObj/SfdTraceRec. */
typedef struct MWSFDTraceRec {
    u32 name;           /* 0x00 */
    u32 entry;          /* 0x04 */
    u8 pad_0x08[0x6C - 0x08];
    u32 exit;           /* 0x6C */
} MWSFDTraceRec;

/* Global Sofdec-movie control block at 0x805FF398. */
typedef struct MWSFDGlobal {
    u32 refCount;            /* 0x00 */
    u32 initFlag;            /* 0x04 */
    SfdTraceCtx* traceObj; /* 0x08 */
    u8 _0C[0x10 - 0x0C];     /* 0x0C */
    MWSFDLibWork libWork;    /* 0x10 .. 0x3503 (0x34F4 bytes) */
    u8 _3504[0x3508 - 0x3504];
    u32 sfxParams;           /* 0x3508 */
    void* hndl;              /* 0x350C */
    void* sfdHn;             /* 0x3510 */
    u32 errCount;            /* 0x3514 */
    u32 errStack[16];        /* 0x3518 .. 0x3557 */
    char errMsg[0x200];      /* 0x3558 */
} MWSFDGlobal;

/* Movie-player handle (MWSFDPLY) fields read by the error formatter. */
typedef struct MWSFDHndl {
    u8 _00[0xE0];
    s32 field_0xE0;      /* 0xE0 */
    u8 _E4[0xE8 - 0xE4];
    s32 field_0xE8;      /* 0xE8 */
    s32 field_0xEC;      /* 0xEC */
} MWSFDHndl;

/* SFD init parameters; the caller (CLibCriMoviePlay) fills [0] with the
 * frame rate and [3] with a flag; the rest are SFD "init prm" fields. */
typedef struct {
    float fps;          /* 0x00 */
    u32 a;              /* 0x04 */
    u32 b;              /* 0x08 */
    u32 c;              /* 0x0C */
    u32 field_0x10;     /* 0x10 */
    u32 field_0x14;     /* 0x14 */
    u32 field_0x18;     /* 0x18 */
    u32 field_0x1C;     /* 0x1C */
} MWSFDINITPRM;

/* Constant table used to scale the frame rate (0x8051AAA8). */
typedef struct {
    u8 _00[0x31C];
    float f_31C;        /* 0x31C */
    float f_320;        /* 0x320 */
} MWSFDSfxPrmTbl;

/* ------------------------------------------------------------------ */
/* External symbols                                                    */
/* ------------------------------------------------------------------ */

extern MWSFDGlobal lbl_eu_805FF398;
extern MWSFDLibWork lbl_eu_805FF3A8;

extern u32 lbl_eu_806029F4;
extern MWSFDSfxPrmTbl lbl_eu_8051AAA8;
extern MWSFDTraceRec sTraceInitSfdFx;

extern void MWSFSVM_Error(const char*, ...);
extern void MWSFSVM_Init(void);
extern void MWSFSVM_EntryIdVfunc(void* fn, void* a, void* b, void* c);
extern void MWSFSVM_EntryMainFunc(int fn, int a, int b);
extern void MWSFSVM_EntryIdleFunc(void* fn, void* a, void* b);

extern s32 SFD_IsVersionCompatible(void* self, u32 version);
extern s32 fn_803C3320(void* cfg);
extern s32 criware_803C0D94(void* handle, void (*errFn)(u32, u32), u32 errArg);
extern void* fn_803C3300(void);
extern void MWSFD_SetCond(void* self, u32 sw, u32 val);
extern void* mwPlyGetSfdHn(void* self);

extern void ADXT_Init(void);
extern void SJRBF_Init(void);
extern void SJMEM_Init(void);
extern void SJUNI_Init(void);
extern int MWSTM_SetTrSct(void);
extern void LSC_Init(void);
extern void LSC_EntryErrFunc(void (*cb)(void*, int), void* ctx);
extern void MWSFSFX_Init(void);

extern void MWSFSVR_VsyncThrdProc(void);
extern void MWSFSVR_MainThrdProc(void);
extern void criware_803A2908(void* self);

extern void MWSFSVR_SetMwsfdSvrFlg(int);

extern int sprintf(char* str, const char* fmt, ...);
extern void* memset(void* dst, int val, size_t n);

/* Forward declarations for intra-TU calls made before the definition. */
void mwsflib_InitLibWork(MWSFDINITPRM* prm);
s32 mwPlySfdInit(void* self);
void MWSFLIB_SfdErrFunc(u32 hndl, u32 err);

/* ------------------------------------------------------------------ */
/* Functions (in retail address order)                                 */
/* ------------------------------------------------------------------ */

void* MWSFLIB_GetLibWorkPtr(void) { return &lbl_eu_805FF3A8; }

void mwsflib_LscErrFunc(void* context, int code) { MWSFSVM_Error((const char*)code); }

/* Initialize the Sofdec movie playback system. Called once (refcounted via
 * the control block); the prm supplies the frame rate and SFD init fields. */
void mwPlyInitSfdFx(MWSFDINITPRM* prm) {
    MWSFDGlobal* g = &lbl_eu_805FF398;
    MWSFDSfxPrmTbl* tbl = &lbl_eu_8051AAA8;
    MWSFDINITPRM initPrm;
    s32 v;

    if (prm == NULL) {
        MWSFSVM_Error("E1122611 mwPlyInitSfdFx: iprm is NULL.");
        return;
    }
    if (g->traceObj != NULL) {
        g->traceObj->vtbl->trace(g->traceObj, &sTraceInitSfdFx.entry);
    }
    memset(&initPrm, 0, sizeof(initPrm));
    initPrm.fps = prm->fps;
    g->sfxParams = (u32)&lbl_eu_8051AAA8;
    MWSFDINITPRM* pInit = &initPrm;
    initPrm.a = 1;
    initPrm.b = 1;
    initPrm.c = prm->c;
    initPrm.field_0x10 = prm->field_0x10;
    initPrm.field_0x14 = prm->field_0x14;
    initPrm.field_0x18 = prm->field_0x18;
    initPrm.field_0x1C = prm->field_0x1C;
    MWSFSVM_Init();

    initPrm.b = initPrm.b - 2;
    if ((s32)initPrm.b < 0)
        initPrm.b = 0;

    if (g->refCount == 0) {
        ADXT_Init();
        SJRBF_Init();
        SJMEM_Init();
        SJUNI_Init();
        if (MWSTM_SetTrSct() != 0) {
            g->libWork.errCode = -101;
            MWSFSVM_Error("E2005 mwPlyInitSfdFx: can't init GSC");
        }
        mwsflib_InitLibWork(pInit);
        lbl_eu_806029F4 = 0;
        v = (s32)(tbl->f_320 * initPrm.fps + tbl->f_31C);
        if (mwPlySfdInit((void*)v) != 0) {
            MWSFSVM_Error("ERR20010421A mwPlyInitSfdFx");
        }
        g->initFlag = 1;
        MWSFD_SetCond(0, 0x1B, (s32)initPrm.fps);
        MWSFD_SetCond(0, 7, 1);
        LSC_Init();
        LSC_EntryErrFunc(mwsflib_LscErrFunc, NULL);
        MWSFSFX_Init();
        MWSFSVM_EntryIdVfunc((void*)2, (void*)MWSFSVR_VsyncThrdProc, (void*)0,
                            "MWSFSVR_VsyncThrdProc");
        MWSFSVM_EntryMainFunc((int)MWSFSVR_MainThrdProc, 0,
                              (int)("MWSFSVR_MainThrdProc"));
        MWSFSVM_EntryIdleFunc((void*)criware_803A2908, (void*)0,
                              "MWSFSVR_IdleThrdProc");
    }
    g->refCount++;
    if (g->traceObj != NULL) {
        g->traceObj->vtbl->trace(g->traceObj, &sTraceInitSfdFx.exit);
    }
}

void mwsflib_InitLibWork(MWSFDINITPRM *prm) {
    MWSFDLibWork *w = &lbl_eu_805FF3A8;
    memset(w, 0, 0x34F8);
    MWSFSVR_SetMwsfdSvrFlg(0);
    w->_5C = 0;
    if (prm != NULL) {
        w->fps = prm->fps;
        w->_08 = prm->a;
        w->_0C = prm->b;
        w->_10 = prm->c;
    } else {
        float f = sDefaultFps.fps;
        w->fps = f;
        w->_08 = 1;
        w->_0C = 1;
        w->_10 = 0;
    }
    w->usePicUsr = 1;
    w->pauseBdr = 0;
    w->_34F0 = 0;
}

u32 MWSFD_GetUsePicUsr(void) {
    return lbl_eu_805FF3A8.usePicUsr;
}

u32 MWSFD_GetPauseBdr(void) {
    return lbl_eu_805FF3A8.pauseBdr;
}

u32 MWSFLIB_SetErrCode(u32 code) {
    u32 lz = __cntlzw(code);
    u32 bit = (lz >> 5) & 1;
    lbl_eu_805FF3A8.errCode = code;
    return code & ~(0 - bit);
}

/* Config block passed to the SFD core init (fn_803C3320). */
typedef struct MWSFDCfg {
    u32 version;   /* 0x00 */
    u32 self;      /* 0x04 */
} MWSFDCfg;

/* One-time SFD library init. Initializes the config block (handle + library
 * version) up front, verifies the SFD core version, installs the shared error
 * callback, then hands the config block to fn_803C3320. Errors are recorded
 * in the lib work errCode slot and returned. */
s32 mwPlySfdInit(void* self) {
    MWSFDCfg cfg = {mwsfd_initsfdpara[0], (u32)self};

    if (SFD_IsVersionCompatible("2.119", 0x39B0) != 1) {
        MWSFSVM_Error("E011081 mwPlySfdInit: Not compatible SFD Version.");
        return -1;
    }
    if (criware_803C0D94(0, MWSFLIB_SfdErrFunc, 0) != 0) {
        lbl_eu_805FF3A8.errCode = -303;
        return -303;
    }
    if (fn_803C3320(&cfg) != 0) {
        lbl_eu_805FF3A8.errCode = -301;
        return -301;
    }
    return 0;
}

/* SFD error callback: records the error in the ring buffer and formats a
 * message (per error code) for the SVM error hook. */
void MWSFLIB_SfdErrFunc(u32 hndl, u32 err) {
    MWSFDGlobal* g = &lbl_eu_805FF398;
    MWSFDHndl* h = (MWSFDHndl*)hndl;
    u32 c;
    void* sfdhn;

    if (h != NULL) {
        sfdhn = mwPlyGetSfdHn(h);
        g->hndl = h;
        g->sfdHn = sfdhn;
    } else {
        h = NULL;
        g->hndl = h;
        g->sfdHn = h;
    }
    if (err != 0) {
        c = g->errCount;
        u32* errStack = g->errStack;
        errStack[c] = err;
        if ((int)c < 0xF)
            g->errCount = c + 1;
    }
    switch ((s32)err) {
    case -3:
    case -2:
        sprintf(g->errMsg, "DATA ERROR(%08X)", err);
        break;
    case 0xFF000431:
    case 0xFF000F5B:
        sprintf(g->errMsg, "DEC ERROR(%08X): %s", err, fn_803C3300());
        break;
    case 0xFF000F17:
    case 0xFF000F18: {
        s32 validSize = 0;
        s32 width = 0;
        s32 height = 0;
        if (h != NULL && h->field_0xE0 == 1) {
            width = h->field_0xE8;
            height = h->field_0xEC;
            if (width > 0 && height > 0)
                validSize = 1;
        }
        if (validSize == 0)
            sprintf(g->errMsg, "SFD ERROR(%08X): 'max_width, max_height' of creation parameter is short. Increase this value.", err, width, height);
        else
            sprintf(g->errMsg, "SFD ERROR(%08X): 'max_width, max_height' of creation parameter is short. This movie size is %d x %d.", err, width, height);
        break;
    }
    case 0xFF000408:
    case 0xFF00040C:
    case 0xFF000F1C:
        sprintf(g->errMsg, "SFD ERROR(%08X): Read buffer is small. Increase 'max_bps' of creation parameter.", err);
        break;
    case 0xFF000F15:
        sprintf(g->errMsg, "SFD ERROR(%08X): Frame pool size is incorrect. Set positive integer to 'nfrm_pool_wk' of creation parameter.", err);
        break;
    case 0xFF000C04:
        sprintf(g->errMsg, "SFD ERROR(%08X): Number of ADXT handles exceeds its maximum number. MWPLY handle uses one ADXT handle(stereo) for MWSFD_FTYPE_SFD.", err);
        break;
    case 0xFF000F1F:
        sprintf(g->errMsg, "SFD ERROR(%08X): mwPlyRelFrm() was called twice to the same frame ID.", err);
        break;
    case 0xFF000F04:
        sprintf(g->errMsg, "SFD ERROR(%08X): Decode picture error. For example, this library can't decode MPEG-2 video.", err);
        break;
    default:
        sprintf(g->errMsg, "SFD ERROR(%08X)", err);
        break;
    }
    MWSFSVM_Error(g->errMsg);
}
