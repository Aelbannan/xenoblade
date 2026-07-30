// Auto-scaffolded catalog TU for CriWare/src/adx/gcci/gcci
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void gcCiGetInterface() {}

void gcCiExecHndl() {}

void gcCiExecServer() {}

extern u32 lbl_eu_805E6B74;
extern u32 lbl_eu_805E6B78;
void gcCiEntryErrFunc(u32 a, u32 b) {
    lbl_eu_805E6B74 = a;
    lbl_eu_805E6B78 = b;
}

void gcCiGetFileSize() {}

void gcCiOpen() {}

void gcCiClose() {}

void gcCiSeek() {}

void gcCiTell() {}

void gcCiReqRd() {}

void gcCiStopTr() {}

void gcCiGetStat() {}

void gcCiGetSctLen() {}

void gcCiSetSctLen() {}

void gcCiGetNumTr() {}

extern s32 DVDConvertPathToEntrynum(const char*);
s32 gcCiIsExistFile(const char* path) {
    s32 entry = DVDConvertPathToEntrynum(path);
    return (entry >= 0) ? 1 : 0;
}

void gcci_rd_cbfn() {}
