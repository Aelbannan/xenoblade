// Auto-scaffolded catalog TU for CriWare/src/adx/gcci/gcci
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include <string.h>

extern u32 lbl_eu_805E6B70;
extern u8  lbl_eu_805E7B30[];
extern char lbl_eu_80565B30[];

void *gcCiGetInterface(void) {
    u32 *p = (u32 *)&lbl_eu_805E6B70;
    {
        volatile u32 t = *(volatile u32 *)p;
        *p = t;
    }
    memset(lbl_eu_805E7B30, 0, 0x100);
    p[1] = 0;
    p[2] = 0;
    memset((u8 *)p + 0xC, 0, 0xC);
    return (void *)lbl_eu_80565B30;
}

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

void gcCiStopTr(void*);
int DVDClose(void*);

void gcCiClose(void* self) {
    if (self == NULL) return;
    gcCiStopTr(self);
    DVDClose((u8*)self + 0x28);
    *(u8*)self = 0;
    memset(self, 0, 0x64);
}

void gcCiSeek() {}

void gcCiTell() {}

void gcCiReqRd() {}

void gcCiStopTr(void* self) { (void)self; }

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
