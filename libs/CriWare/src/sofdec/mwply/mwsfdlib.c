// Auto-scaffolded catalog TU for CriWare/src/sofdec/mwply/mwsfdlib
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

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

extern MWSFDLibWork lbl_eu_805FF3A8;

void* MWSFLIB_GetLibWorkPtr(void) { return &lbl_eu_805FF3A8; }

extern void MWSFSVM_Error(const char*, ...);
void mwsflib_LscErrFunc(void* context, int code) { MWSFSVM_Error((const char*)code); }

void mwPlyInitSfdFx() {}

typedef struct {
    float fps;
    u32 a;
    u32 b;
    u32 c;
} MWSFDINITPRM;

extern void MWSFSVR_SetMwsfdSvrFlg(int);
extern const float lbl_eu_8051ADCC;

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

extern char lbl_eu_8051ADD0[];
extern u32 lbl_eu_8051AB38;

extern s32 SFD_IsVersionCompatible(const void* hdr, s32 size);
extern s32 fn_803C3320(s32 a, void* fn, s32 c);
extern s32 criware_803C0D94(void* v);
extern void MWSFLIB_SfdErrFunc(void);
extern void MWSFSVM_Error(const char* fmt, ...);

s32 mwPlySfdInit(void* self) {
    if (SFD_IsVersionCompatible(lbl_eu_8051ADD0 + 0xA8, 0x39B0) != 1) {
        MWSFSVM_Error(lbl_eu_8051ADD0 + 0xAE);
        return -1;
    }
    if (fn_803C3320(0, MWSFLIB_SfdErrFunc, 0) != 0) {
        *(s32*)((u8*)&lbl_eu_805FF3A8 + 0x68) = -303;
        return -303;
    }
    {
        s32 v = (s32)lbl_eu_8051AB38;
        if (criware_803C0D94(&v) != 0) {
            *(s32*)((u8*)&lbl_eu_805FF3A8 + 0x68) = -301;
            return -301;
        }
    }
    return 0;
}

void MWSFLIB_SfdErrFunc() {}
