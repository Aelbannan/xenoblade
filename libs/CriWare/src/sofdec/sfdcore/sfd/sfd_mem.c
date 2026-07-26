// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/sfd/sfd_mem
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

// LLM-HARNESS-BEGIN: us-803c36e8
int SFMEM_Init(void) { return 0x0; }
// LLM-HARNESS-END: us-803c36e8

// LLM-HARNESS-BEGIN: us-803c36f0
int SFMEM_Finish(void) { return 0x0; }
// LLM-HARNESS-END: us-803c36f0

// LLM-HARNESS-BEGIN: us-803c36f8
int SFMEM_ExecServer(void* p) {
    unsigned int val = *(unsigned int*)((char*)p + 0x1fec);
    SFBUF_SetPrepFlg(p, val, 1);
    return 0;
}
// LLM-HARNESS-END: us-803c36f8

// LLM-HARNESS-BEGIN: us-803c3724
int SFMEM_Create(void) { return 0x0; }
// LLM-HARNESS-END: us-803c3724

// LLM-HARNESS-BEGIN: us-803c372c
int SFMEM_Destroy(void) { return 0x0; }
// LLM-HARNESS-END: us-803c372c

// LLM-HARNESS-BEGIN: us-803c3734
int SFMEM_RequestStop(void) { return 0x0; }
// LLM-HARNESS-END: us-803c3734

// LLM-HARNESS-BEGIN: us-803c373c
int SFMEM_Start(void) { return 0x0; }
// LLM-HARNESS-END: us-803c373c

// LLM-HARNESS-BEGIN: us-803c3744
int SFMEM_Stop(void) { return 0x0; }
// LLM-HARNESS-END: us-803c3744

// LLM-HARNESS-BEGIN: us-803c374c
int SFMEM_Pause(void) { return 0x0; }
// LLM-HARNESS-END: us-803c374c

// LLM-HARNESS-BEGIN: us-803c3754
void* SFBUF_RingGetWrite(void* self, u32 field, u32 param);
void* SFMEM_GetWrite(void* self, u32 param) {
    return SFBUF_RingGetWrite(self, *(u32*)((u8*)self + 0x1fec), param);
}
// LLM-HARNESS-END: us-803c3754

// LLM-HARNESS-BEGIN: us-803c3760
void SFBUF_RingAddWrite(void* self, u32 a, u32 b, u32 c);
void SFMEM_AddWrite(void* self, u32* a, u32* b) {
    SFBUF_RingAddWrite(self, *(u32*)((u8*)self + 0x1fec), *a, *b);
}
// LLM-HARNESS-END: us-803c3760

// LLM-HARNESS-BEGIN: us-803c3778
s32 SFLIB_SetErr(void* h, u32 err_code);
s32 SFMEM_GetRead(void* h) {
    return SFLIB_SetErr(h, 0xff000501);
}
// LLM-HARNESS-END: us-803c3778

// LLM-HARNESS-BEGIN: us-803c3784
s32 SFLIB_SetErr(void* h, u32 err_code);
s32 SFMEM_AddRead(void* h) {
    return SFLIB_SetErr(h, 0xff000501);
}
// LLM-HARNESS-END: us-803c3784

// LLM-HARNESS-BEGIN: us-803c3790
int SFMEM_Seek(void) { return 0x0; }
// LLM-HARNESS-END: us-803c3790
