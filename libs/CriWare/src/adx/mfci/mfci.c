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

typedef void (*MfCiErrFn)(u32, const char*, int);
extern MfCiErrFn lbl_eu_805EC450;
extern u32 lbl_eu_805EC454;
extern char lbl_eu_80518844[];
void mfCiEntryErrFunc(u32 a, u32 b) {
    lbl_eu_805EC450 = (MfCiErrFn)a;
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

typedef struct MfCiSectInfo {
    u32 unk00;
    s32 sctsize;
    s32 len;
    s32 sctcnt;
    s32 sctcnt2;
    s32 unk14;
    s32 unk18;
} MfCiSectInfo;

void mfCiSetSctLen(MfCiSectInfo* sct, int sctsize)
{
    if (sct == NULL) {
        const char* msg = lbl_eu_80518844 + 0x170;
        if (lbl_eu_805EC450 != NULL)
            lbl_eu_805EC450(lbl_eu_805EC454, msg, 0);
        return;
    }
    {
        s32 old = sct->sctsize;
        sct->sctsize = sctsize;
        sct->sctcnt = (sct->len + sctsize - 1) / sctsize;
        sct->sctcnt2 = sct->sctcnt2 * old / sctsize;
        sct->unk14 = sct->unk18 * sctsize;
    }
}

void mfCiGetNumTr() {}

void mfCiOptFn1() {}
