// Auto-scaffolded catalog TU for CriWare/src/adx/mfci/mfci
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

extern u32 lbl_eu_80518840;
extern char lbl_eu_80565B98[];
void* mfCiGetInterface(void) {
    *(volatile u32*)&lbl_eu_80518840;
    return lbl_eu_80565B98;
}

void mfci_str_to_uint_ptr() {}

void mfCiExecServer() {}

extern u32 lbl_eu_805EC450;
extern u32 lbl_eu_805EC454;
void mfCiEntryErrFunc(u32 a, u32 b) {
    lbl_eu_805EC450 = a;
    lbl_eu_805EC454 = b;
}

void mfCiGetFileSize() {}

void mfCiOpen() {}

void mfCiClose() {}

void mfCiSeek() {}

void mfCiTell() {}

void mfCiReqRd() {}

void mfCiStopTr() {}

void mfCiGetStat() {}

void mfCiGetSctLen() {}

void mfCiSetSctLen() {}

void mfCiGetNumTr() {}

void mfCiOptFn1() {}
