// Auto-scaffolded catalog TU for CriWare/src/sofdec/mwply/mwsfdlib
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void* MWSFLIB_GetLibWorkPtr(void) { return 0; }

void mwsflib_LscErrFunc(void) {}

void mwPlyInitSfdFx() {}

void mwsflib_InitLibWork() {}

extern char lbl_eu_805FF3A8[];
u32 MWSFD_GetUsePicUsr(void) {
    return *(u32*)(lbl_eu_805FF3A8 + 0x38);
}

extern char lbl_eu_805FF3A8[];
u32 MWSFD_GetPauseBdr(void) {
    return *(u32*)(lbl_eu_805FF3A8 + 0x3c);
}

u32 MWSFLIB_SetErrCode(u32 code) {
    u32 lz = __cntlzw(code);
    u32 bit = (lz >> 5) & 1;
    *(u32*)(lbl_eu_805FF3A8 + 0x68) = code;
    return code & ~(0 - bit);
}

void mwPlySfdInit() {}

void MWSFLIB_SfdErrFunc() {}
