// Auto-scaffolded catalog TU for CriWare/src/sofdec/mwply/mwsfdlib
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void* MWSFLIB_GetLibWorkPtr(void) { return 0; }

void mwsflib_LscErrFunc(void) {}

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

void mwPlySfdInit() {}

void MWSFLIB_SfdErrFunc() {}
