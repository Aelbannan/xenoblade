// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/sfd/sfd_trn
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

// LLM-HARNESS-BEGIN: us-803d0d9c
void SFTRN_Init() {}
// LLM-HARNESS-END: us-803d0d9c

// LLM-HARNESS-BEGIN: us-803d0e94
void SFTRN_InitHn() {}
// LLM-HARNESS-END: us-803d0e94

// LLM-HARNESS-BEGIN: us-803d0f88
void sftrn_BuildAll() {}
// LLM-HARNESS-END: us-803d0f88

// LLM-HARNESS-BEGIN: us-803d1154
void sftrn_BuildSystem() {}
// LLM-HARNESS-END: us-803d1154

// LLM-HARNESS-BEGIN: us-803d12d8
void SFTRN_CallTrSetup() {}
// LLM-HARNESS-END: us-803d12d8

// LLM-HARNESS-BEGIN: us-803d1368
void SFTRN_CallTrtTrif() {}
// LLM-HARNESS-END: us-803d1368

// LLM-HARNESS-BEGIN: us-803d13a4
void SFTRN_SetPrepFlg(void* self, u32 idx, u32 val) {
    *(u32*)((u8*)self + 0x1fd8 + idx * 0x44) = val;
}
// LLM-HARNESS-END: us-803d13a4

// LLM-HARNESS-BEGIN: us-803d13b4
u32 SFTRN_GetPrepFlg(void* self, u32 idx) {
    return *(u32*)((u8*)self + 0x1fd8 + idx * 0x44);
}
// LLM-HARNESS-END: us-803d13b4

// LLM-HARNESS-BEGIN: us-803d13c4
void SFTRN_SetTermFlg(void* self, u32 idx, u32 val) {
    *(u32*)((u8*)self + 0x1fdc + idx * 0x44) = val;
}
// LLM-HARNESS-END: us-803d13c4

// LLM-HARNESS-BEGIN: us-803d13d4
u32 SFTRN_GetTermFlg(void* self, u32 idx) {
    return *(u32*)((u8*)self + 0x1fdc + idx * 0x44);
}
// LLM-HARNESS-END: us-803d13d4

// LLM-HARNESS-BEGIN: us-803d13e4
u32 SFTRN_IsSetup(void* self, u32 idx) {
    s32 val = *(s32*)((u8*)self + idx * 0x44 + 0x1fe4);
    return val ? 1 : 0;
}
// LLM-HARNESS-END: us-803d13e4
