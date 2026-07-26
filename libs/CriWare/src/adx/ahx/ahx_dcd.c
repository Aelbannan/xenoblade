// Auto-scaffolded catalog TU for CriWare/src/adx/ahx/ahx_dcd
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void SKG_GenerateKey() {}

void AHXSJD_SetupAtbl(void) {}

void AHXSJD_SetupMtbl(u32 val) {}

void AHXSJD_SetupFtbl(void) {}

void AHXSJD_SetupWtbl(void) { AHXDCD_SetupWtbl(); }

void AHXDCD_Init() {}

extern s32 lbl_eu_805E64D8;
void AHXSBF_Finish(void);
void AHXDCD_Finish(void) {
    if (--lbl_eu_805E64D8 != 0) return;
    AHXSBF_Finish();
}

void AHXDCD_Create() {}

void AHXDCD_Destroy(void* p)
{
    void** sbf = (void**)((char*)p + 0x354);
    if (*sbf) {
        void* tmp = *sbf;
        *sbf = 0;
        AHXSBF_Destroy(tmp);
    }
    memset(p, 0, 0xbcc);
}

void AHXDCD_Reset() {}

void AHXDCD_SetBsr() {}

void AHXDCD_DecodeHeader() {}

void AHXDCD_DecodeFrmHdr() {}

u32 AHXDCD_IsEof(void* self) {
    return (*(u32*)((u8*)self + 0x34c) == 0xc) ? 1 : 0;
}

void AHXDCD_DecodeData() {}

s16 AHXDCD_GetOutBps(void* self) { return (signed char)((u8*)self)[0x34b]; }

int AHXDCD_GetOutSmpl(void) { return 0x60; }

u32 AHXDCD_GetTotalNumSmpl(void* self) {
    if ((signed char)((u8*)self)[0x349] == 0) return 0;
    return *(u32*)((u8*)self + 0x394);
}

void* memcpy(void* dest, const void* src, size_t n);
void AHXDCD_SetExtPrm(void* self, void* prm) {
    memcpy((u8*)self + 0xbc4, prm, 8);
}

void AHXDCD_DecodeBhdr() {}

void AHXDCD_BhdrToDinf() {}

void AHXDCD_DecodeBitalloc2() {}

void AHXDCD_DecodeScale2() {}

void AHXDCD_GetSample_Dequantize_Denormalize() {}
