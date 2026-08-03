// Auto-scaffolded catalog TU for CriWare/src/sofdec/mwply/mwsfdlib
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

typedef struct MWSFDLibWork MWSFDLibWork;
extern MWSFDLibWork lbl_eu_805FF3A8;

void* MWSFLIB_GetLibWorkPtr(void) { return &lbl_eu_805FF3A8; }

extern void MWSFSVM_Error(const char*, ...);
void mwsflib_LscErrFunc(void* context, int code) { MWSFSVM_Error((const char*)code); }

void mwPlyInitSfdFx() {}

void mwsflib_InitLibWork() {}

typedef struct MWSFDLibWork {
    u8 _00[0x38];
    u32 usePicUsr;
    u32 pauseBdr;
    u8 _40[0x28];
    u32 errCode;
} MWSFDLibWork;

extern MWSFDLibWork lbl_eu_805FF3A8;
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
