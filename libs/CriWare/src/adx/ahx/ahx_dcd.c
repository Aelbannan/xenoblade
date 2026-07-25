// Auto-scaffolded catalog TU for CriWare/src/adx/ahx/ahx_dcd
// Mangled extern stubs for llm-harness / coop selection.
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

// LLM-HARNESS-BEGIN: us-80390c84
void SKG_GenerateKey() {}
// LLM-HARNESS-END: us-80390c84

// LLM-HARNESS-BEGIN: us-8039142c
void AHXSJD_SetupAtbl(void) {}
// LLM-HARNESS-END: us-8039142c

// LLM-HARNESS-BEGIN: us-80391430
void AHXSJD_SetupMtbl(u32 val) {}
// LLM-HARNESS-END: us-80391430

// LLM-HARNESS-BEGIN: us-8039143c
void AHXSJD_SetupFtbl(void) {}
// LLM-HARNESS-END: us-8039143c

// LLM-HARNESS-BEGIN: us-80391440
void AHXSJD_SetupWtbl(void) { AHXDCD_SetupWtbl(); }
// LLM-HARNESS-END: us-80391440

// LLM-HARNESS-BEGIN: us-80391444
void AHXDCD_Init() {}
// LLM-HARNESS-END: us-80391444

// LLM-HARNESS-BEGIN: us-80391484
extern s32 lbl_eu_805E64D8;
void AHXSBF_Finish(void);
void AHXDCD_Finish(void) {
    if (--lbl_eu_805E64D8 != 0) return;
    AHXSBF_Finish();
}
// LLM-HARNESS-END: us-80391484

// LLM-HARNESS-BEGIN: us-803914a0
void AHXDCD_Create() {}
// LLM-HARNESS-END: us-803914a0

// LLM-HARNESS-BEGIN: us-80391568
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
// LLM-HARNESS-END: us-80391568

// LLM-HARNESS-BEGIN: us-803915bc
void AHXDCD_Reset() {}
// LLM-HARNESS-END: us-803915bc

// LLM-HARNESS-BEGIN: us-80391690
void AHXDCD_SetBsr() {}
// LLM-HARNESS-END: us-80391690

// LLM-HARNESS-BEGIN: us-803916cc
void AHXDCD_DecodeHeader() {}
// LLM-HARNESS-END: us-803916cc

// LLM-HARNESS-BEGIN: us-8039188c
void AHXDCD_DecodeFrmHdr() {}
// LLM-HARNESS-END: us-8039188c

// LLM-HARNESS-BEGIN: us-80391970
u32 AHXDCD_IsEof(void* self) {
    return (*(u32*)((u8*)self + 0x34c) == 0xc) ? 1 : 0;
}
// LLM-HARNESS-END: us-80391970

// LLM-HARNESS-BEGIN: us-80391984
void AHXDCD_DecodeData() {}
// LLM-HARNESS-END: us-80391984

// LLM-HARNESS-BEGIN: us-80391a40
s16 AHXDCD_GetOutBps(void* self) { return (signed char)((u8*)self)[0x34b]; }
// LLM-HARNESS-END: us-80391a40

// LLM-HARNESS-BEGIN: us-80391a4c
int AHXDCD_GetOutSmpl(void) { return 0x60; }
// LLM-HARNESS-END: us-80391a4c

// LLM-HARNESS-BEGIN: us-80391a54
u32 AHXDCD_GetTotalNumSmpl(void* self) {
    if ((signed char)((u8*)self)[0x349] == 0) return 0;
    return *(u32*)((u8*)self + 0x394);
}
// LLM-HARNESS-END: us-80391a54

// LLM-HARNESS-BEGIN: us-80391a70
void* memcpy(void* dest, const void* src, size_t n);
void AHXDCD_SetExtPrm(void* self, void* prm) {
    memcpy((u8*)self + 0xbc4, prm, 8);
}
// LLM-HARNESS-END: us-80391a70

// LLM-HARNESS-BEGIN: us-80391a7c
void AHXDCD_DecodeBhdr() {}
// LLM-HARNESS-END: us-80391a7c

// LLM-HARNESS-BEGIN: us-80391b9c
void AHXDCD_BhdrToDinf() {}
// LLM-HARNESS-END: us-80391b9c

// LLM-HARNESS-BEGIN: us-80391c90
void AHXDCD_DecodeBitalloc2() {}
// LLM-HARNESS-END: us-80391c90

// LLM-HARNESS-BEGIN: us-80391e20
void AHXDCD_DecodeScale2() {}
// LLM-HARNESS-END: us-80391e20

// LLM-HARNESS-BEGIN: us-803921e8
void AHXDCD_GetSample_Dequantize_Denormalize() {}
// LLM-HARNESS-END: us-803921e8
