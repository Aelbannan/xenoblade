// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/sfd/sfd_see
// Mangled extern stubs for llm-harness / coop selection.
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

// LLM-HARNESS-BEGIN: us-803ce5a0
void SFSEE_InitHn(void* self) {
    *(u32*)((u8*)self + 0x00) = 0;
    *(u32*)((u8*)self + 0x04) = 0;
    *(u32*)((u8*)self + 0x08) = -3;
    *(u32*)((u8*)self + 0x0C) = 1;
}
// LLM-HARNESS-END: us-803ce5a0

// LLM-HARNESS-BEGIN: us-803ce5c0
void SFD_EntrySeek() {}
// LLM-HARNESS-END: us-803ce5c0

// LLM-HARNESS-BEGIN: us-803ce61c
void SFSEE_FixAvPlay(void *arg, int arg2, int arg3) {
    void *ptr = *(void**)((char*)arg + 0x2670);
    if (ptr == NULL) return;
    if (*(int*)((char*)ptr + 0xdb8) < 0) {
        *(int*)((char*)ptr + 0xdb8) = arg2;
    }
    if (*(int*)((char*)ptr + 0xdbc) < 0) {
        *(int*)((char*)ptr + 0xdbc) = arg3;
    }
}
// LLM-HARNESS-END: us-803ce61c

// LLM-HARNESS-BEGIN: us-803ce64c
void SFSEE_ExecServer() {}
// LLM-HARNESS-END: us-803ce64c

// LLM-HARNESS-BEGIN: us-803ce68c
void sfsee_ExecHeadAnaly() {}
// LLM-HARNESS-END: us-803ce68c

// LLM-HARNESS-BEGIN: us-803ce880
void SFD_SetFileSize() {}
// LLM-HARNESS-END: us-803ce880

// LLM-HARNESS-BEGIN: us-803ce8f8
void SFD_SetTotTime() {}
// LLM-HARNESS-END: us-803ce8f8

// LLM-HARNESS-BEGIN: us-803ce980
void SFD_SetByteRate() {}
// LLM-HARNESS-END: us-803ce980

// LLM-HARNESS-BEGIN: us-803ce9f8
void SFD_SetSeekPos() {}
// LLM-HARNESS-END: us-803ce9f8

// LLM-HARNESS-BEGIN: us-803cea68
void sfsee_ExecFinAnaly() {}
// LLM-HARNESS-END: us-803cea68

// LLM-HARNESS-BEGIN: us-803ceb60
void sfsee_UpdateEByteRate() {}
// LLM-HARNESS-END: us-803ceb60
