// Auto-scaffolded catalog TU for CriWare/src/sofdec/mwply/mwsfdlib
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

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
 * record (lbl_eu_805660D8) and fires the callback on entry (+0x04) and
 * exit (+0x6C). Same shape as the mwsfdply TU's SfdTraceObj/SfdTraceRec. */
typedef struct MWSFDTraceVtable {
    u8 pad_0x00[0x24];
    void (*trace)(void* self, void* rec);
} MWSFDTraceVtable;
typedef struct MWSFDTraceObj {
    const MWSFDTraceVtable* vtable;
} MWSFDTraceObj;
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
    MWSFDTraceObj* traceObj; /* 0x08 */
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
    u32 field_0xE0;      /* 0xE0 */
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

extern char lbl_eu_8051ADD0[];      /* error message string pool */
extern u32 lbl_eu_8051AB38;
extern u32 lbl_eu_806029F4;
extern MWSFDSfxPrmTbl lbl_eu_8051AAA8;
extern MWSFDTraceRec lbl_eu_805660D8;

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
extern const float lbl_eu_8051ADCC;

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
        MWSFSVM_Error(lbl_eu_8051ADD0);
        return;
    }
    if (g->traceObj != NULL) {
        g->traceObj->vtable->trace(g->traceObj, &lbl_eu_805660D8.entry);
    }
    memset(&initPrm, 0, sizeof(initPrm));
    initPrm.fps = prm->fps;
    MWSFDINITPRM* pInit = &initPrm;
    initPrm.a = 1;
    initPrm.b = 1;
    initPrm.c = prm->c;
    initPrm.field_0x10 = prm->field_0x10;
    initPrm.field_0x14 = prm->field_0x14;
    initPrm.field_0x18 = prm->field_0x18;
    initPrm.field_0x1C = prm->field_0x1C;
    g->sfxParams = (u32)&lbl_eu_8051AAA8;
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
            MWSFSVM_Error(lbl_eu_8051ADD0 + 0x27);
        }
        mwsflib_InitLibWork(pInit);
        lbl_eu_806029F4 = 0;
        v = (s32)(tbl->f_320 * initPrm.fps + tbl->f_31C);
        if (mwPlySfdInit((void*)v) != 0) {
            MWSFSVM_Error(lbl_eu_8051ADD0 + 0x4C);
        }
        g->initFlag = 1;
        MWSFD_SetCond(0, 0x1B, (s32)initPrm.fps);
        MWSFD_SetCond(0, 7, 1);
        LSC_Init();
        LSC_EntryErrFunc(mwsflib_LscErrFunc, NULL);
        MWSFSFX_Init();
        MWSFSVM_EntryIdVfunc((void*)2, (void*)MWSFSVR_VsyncThrdProc, (void*)0,
                            lbl_eu_8051ADD0 + 0x68);
        MWSFSVM_EntryMainFunc((int)MWSFSVR_MainThrdProc, 0,
                              (int)(lbl_eu_8051ADD0 + 0x7E));
        MWSFSVM_EntryIdleFunc((void*)criware_803A2908, (void*)0,
                              lbl_eu_8051ADD0 + 0x93);
    }
    g->refCount++;
    if (g->traceObj != NULL) {
        g->traceObj->vtable->trace(g->traceObj, &lbl_eu_805660D8.exit);
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
        float f = lbl_eu_8051ADCC;
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

/* One-time SFD library init. The version global and the handle are passed
 * on to fn_803C3320 as a two-word config block; errors are recorded in the
 * lib work errCode slot and returned. */
s32 mwPlySfdInit(void* self) {
    MWSFDCfg cfg;
    u32 version = lbl_eu_8051AB38;

    cfg.self = (u32)self;
    cfg.version = version;
    if (SFD_IsVersionCompatible(lbl_eu_8051ADD0 + 0xA8, 0x39B0) != 1) {
        MWSFSVM_Error(lbl_eu_8051ADD0 + 0xAE);
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

    if (h != NULL) {
        g->sfdHn = mwPlyGetSfdHn(h);
        g->hndl = h;
    } else {
        g->hndl = NULL;
        g->sfdHn = NULL;
    }
    if (err != 0) {
        c = g->errCount;
        g->errStack[c] = err;
        if ((s32)c < 0xF)
            g->errCount = c + 1;
    }
    switch (err) {
    case 0xFF000F15:
        sprintf(g->errMsg, lbl_eu_8051ADD0 + 0x219, err);
        break;
    case 0xFF000F1F:
        sprintf(g->errMsg, lbl_eu_8051ADD0 + 0x309, err);
        break;
    case 0xFF000F04:
        sprintf(g->errMsg, lbl_eu_8051ADD0 + 0x34F, err);
        break;
    case 0xFF000C04:
        sprintf(g->errMsg, lbl_eu_8051ADD0 + 0x286, err);
        break;
    case 0xFF000431:
    case 0xFF000F5B:
        sprintf(g->errMsg, lbl_eu_8051ADD0 + 0xF1, err,
                fn_803C3300());
        break;
    case 0xFF00040C:
    case 0xFF000408:
    case 0xFF000F1C:
        sprintf(g->errMsg, lbl_eu_8051ADD0 + 0x1C8, err);
        break;
    case 0xFF000F17:
        if (h != NULL && h->field_0xE0 == 1 && h->field_0xE8 > 0 &&
            h->field_0xEC > 0)
            sprintf(g->errMsg, lbl_eu_8051ADD0 + 0x163, err);
        else
            sprintf(g->errMsg, lbl_eu_8051ADD0 + 0x105, err);
        break;
    default:
        if ((s32)err >= -3) {
            if ((s32)err < -1)
                sprintf(g->errMsg, lbl_eu_8051ADD0 + 0xE0, err);
            else
                sprintf(g->errMsg, lbl_eu_8051ADD0 + 0x3AB, err);
        } else {
            sprintf(g->errMsg, lbl_eu_8051ADD0 + 0x3AB, err);
        }
        break;
    }
    MWSFSVM_Error(g->errMsg);
}
