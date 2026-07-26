// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/sfd/sfd_pts
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

// LLM-HARNESS-BEGIN: us-803ce16c
void SFPTS_InitPtsQue(void* self) {
    *(u32*)((u8*)self) = 0;
    *(u32*)((u8*)self + 4) = 0;
    *(u32*)((u8*)self + 8) = 0;
    *(u32*)((u8*)self + 0xc) = 0;
    *(u32*)((u8*)self + 0x10) = 0;
}
// LLM-HARNESS-END: us-803ce16c

// LLM-HARNESS-BEGIN: us-803ce188
void SFPTS_ResetPtsQue(void* self) {
    *(u32*)((u8*)self + 0x08) = 0;
    *(u32*)((u8*)self + 0x0c) = 0;
    *(u32*)((u8*)self + 0x10) = 0;
}
// LLM-HARNESS-END: us-803ce188

// LLM-HARNESS-BEGIN: us-803ce19c
void SFD_SetVideoPts() {}
// LLM-HARNESS-END: us-803ce19c

// LLM-HARNESS-BEGIN: us-803ce260
void SFPTS_WritePtsQue() {}
// LLM-HARNESS-END: us-803ce260

// LLM-HARNESS-BEGIN: us-803ce35c
void SFPTS_ReadPtsQue() {}
// LLM-HARNESS-END: us-803ce35c

// LLM-HARNESS-BEGIN: us-803ce4c0
void sfpts_SearchPtsQue() {}
// LLM-HARNESS-END: us-803ce4c0

// LLM-HARNESS-BEGIN: us-803ce568
void SFPTS_IsPtsQueFull() {}
// LLM-HARNESS-END: us-803ce568
