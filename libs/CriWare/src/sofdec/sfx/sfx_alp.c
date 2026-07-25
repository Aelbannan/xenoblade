// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfx/sfx_alp
// Mangled extern stubs for llm-harness / coop selection.
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

// LLM-HARNESS-BEGIN: us-803d825c
void* memset(void* s, int c, size_t n);
extern u8 lbl_eu_8061A138[0x128];
void SFXA_Init(void) {
    memset(lbl_eu_8061A138, 0, 0x128);
    *(u32*)(lbl_eu_8061A138 + 4) = 8;
}
// LLM-HARNESS-END: us-803d825c

// LLM-HARNESS-BEGIN: us-803d82a0
void SFXA_Create() {}
// LLM-HARNESS-END: us-803d82a0

// LLM-HARNESS-BEGIN: us-803d832c
void SFXA_Destroy(void* self) {
    if (self == NULL) return;
    *(u32*)self = 0;
}
// LLM-HARNESS-END: us-803d832c

// LLM-HARNESS-BEGIN: us-803d8350
void SFXA_MakeAlpLumiTbl(void* self, u32 a, u32 b, u32 c) {
    void (*cb)(u32, u32, u32, u32) = *(void (**)(u32, u32, u32, u32))((u8*)self + 0x18);
    if (cb) cb(*(u32*)((u8*)self + 8), *(u32*)((u8*)self + 0xc), *(u32*)((u8*)self + 0x10), c);
    *(u32*)((u8*)self + 4) = 0;
}
// LLM-HARNESS-END: us-803d8350

// LLM-HARNESS-BEGIN: us-803d83a8
void SFXA_MakeAlp3110Tbl() {}
// LLM-HARNESS-END: us-803d83a8

// LLM-HARNESS-BEGIN: us-803d83d4
void SFXA_MakeAlp3211Tbl() {}
// LLM-HARNESS-END: us-803d83d4

// LLM-HARNESS-BEGIN: us-803d8400
u32 SFXA_IsNeedUpdateLumiTbl(void* self) { return *(u32*)((u8*)self + 0x4); }
// LLM-HARNESS-END: us-803d8400
