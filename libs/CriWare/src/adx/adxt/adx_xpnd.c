// Auto-scaffolded catalog TU for CriWare/src/adx/adxt/adx_xpnd
// Mangled extern stubs for llm-harness / coop selection.
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

// LLM-HARNESS-BEGIN: us-803888a8
void* memset(void* s, int c, size_t n);
extern char lbl_eu_805E4F80[];
void ADXPD_Init(void) {
    memset(lbl_eu_805E4F80, 0, 0x3c0);
}
// LLM-HARNESS-END: us-803888a8

// LLM-HARNESS-BEGIN: us-803888bc
void ADXPD_Create() {}
// LLM-HARNESS-END: us-803888bc

// LLM-HARNESS-BEGIN: us-803889f8
void ADX_GetCoefficient(void* a, void* b, void* c, void* d);
void ADXPD_SetCoef(void* self, void* tbl, void* idx) {
    ADX_GetCoefficient(idx, tbl, (u8*)self + 0x30, (u8*)self + 0x32);
}
// LLM-HARNESS-END: us-803889f8

// LLM-HARNESS-BEGIN: us-80388a0c
void ADXPD_SetDly(void* self, s16* a, s16* b) {
    *(s16*)((u8*)self + 0x28) = a[0];
    *(s16*)((u8*)self + 0x2A) = b[0];
    *(s16*)((u8*)self + 0x2C) = a[1];
    *(s16*)((u8*)self + 0x2E) = b[1];
}
// LLM-HARNESS-END: us-80388a0c

// LLM-HARNESS-BEGIN: us-80388a30
void ADXPD_GetDly(void* self, s16* a, s16* b) {
    a[0] = *(s16*)((u8*)self + 0x28);
    b[0] = *(s16*)((u8*)self + 0x2A);
    a[1] = *(s16*)((u8*)self + 0x2C);
    b[1] = *(s16*)((u8*)self + 0x2E);
}
// LLM-HARNESS-END: us-80388a30

// LLM-HARNESS-BEGIN: us-80388a54
void ADXPD_SetExtPrm(void* self, s16 a, s16 b, s16 c) {
    *(s16*)((u8*)self + 0x34) = a;
    *(s16*)((u8*)self + 0x36) = b;
    *(s16*)((u8*)self + 0x38) = c;
}
// LLM-HARNESS-END: us-80388a54

// LLM-HARNESS-BEGIN: us-80388a64
void ADXPD_GetExtPrm(void* self, s16* out1, s16* out2, s16* out3) {
    *out1 = *(s16*)((u8*)self + 0x34);
    *out2 = *(s16*)((u8*)self + 0x36);
    *out3 = *(s16*)((u8*)self + 0x38);
}
// LLM-HARNESS-END: us-80388a64

// LLM-HARNESS-BEGIN: us-80388a80
void ADXPD_Destroy(void* self) {
    if (self == NULL) return;
    *(u32*)self = 0;
    {
        void* s = self;
        u32 z = (u32)(unsigned long)s & 0;
        memset(s, z, 0x3c);
    }
}
// LLM-HARNESS-END: us-80388a80

// LLM-HARNESS-BEGIN: us-80388aa0
u32 ADXPD_GetStat(void* self) { return *(u32*)((u8*)self + 0xc); }
// LLM-HARNESS-END: us-80388aa0

// LLM-HARNESS-BEGIN: us-80388aa8
void ADXPD_EntryMono() {}
// LLM-HARNESS-END: us-80388aa8

// LLM-HARNESS-BEGIN: us-80388adc
void ADXPD_EntrySte() {}
// LLM-HARNESS-END: us-80388adc

// LLM-HARNESS-BEGIN: us-80388b10
void ADXPD_EntryPl2() {}
// LLM-HARNESS-END: us-80388b10

// LLM-HARNESS-BEGIN: us-80388b44
void ADXPD_Start(void* self) {
    if (*(u32*)((u8*)self + 0xc) != 0) return;
    *(u32*)((u8*)self + 0x10) = 0;
    *(u32*)((u8*)self + 0xc) = 1;
}
// LLM-HARNESS-END: us-80388b44

// LLM-HARNESS-BEGIN: us-80388b64
void* memset(void* s, int c, size_t n);
void ADXPD_Stop(void* self) {
    *(u32*)((u8*)self + 0x0c) = 0;
    memset((u8*)self + 0x28, 0, 8);
}
// LLM-HARNESS-END: us-80388b64

// LLM-HARNESS-BEGIN: us-80388b7c
void ADXPD_Reset(void* self) {
    if (*(u32*)((u8*)self + 0x0c) == 3) {
        *(u32*)((u8*)self + 0x0c) = 0;
    }
}
// LLM-HARNESS-END: us-80388b7c

// LLM-HARNESS-BEGIN: us-80388b94
u32 ADXPD_GetNumBlk(void* self) { return *(u32*)((u8*)self + 0x10); }
// LLM-HARNESS-END: us-80388b94

// LLM-HARNESS-BEGIN: us-80388b9c
void ADXPD_ExecHndl() {}
// LLM-HARNESS-END: us-80388b9c
