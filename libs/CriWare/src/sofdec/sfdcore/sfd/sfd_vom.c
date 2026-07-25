// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/sfd/sfd_vom
// Mangled extern stubs for llm-harness / coop selection.
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

// LLM-HARNESS-BEGIN: us-803d1648
int SFVOM_Init(void) { return 0x0; }
// LLM-HARNESS-END: us-803d1648

// LLM-HARNESS-BEGIN: us-803d1650
int SFVOM_Finish(void) { return 0x0; }
// LLM-HARNESS-END: us-803d1650

// LLM-HARNESS-BEGIN: us-803d1658
void SFVOM_ExecServer() {}
// LLM-HARNESS-END: us-803d1658

// LLM-HARNESS-BEGIN: us-803d1744
int SFVOM_Create(void) { return 0x0; }
// LLM-HARNESS-END: us-803d1744

// LLM-HARNESS-BEGIN: us-803d174c
int SFVOM_Destroy(void) { return 0x0; }
// LLM-HARNESS-END: us-803d174c

// LLM-HARNESS-BEGIN: us-803d1754
int SFVOM_RequestStop(void) { return 0x0; }
// LLM-HARNESS-END: us-803d1754

// LLM-HARNESS-BEGIN: us-803d175c
int SFVOM_Start(void) { return 0x0; }
// LLM-HARNESS-END: us-803d175c

// LLM-HARNESS-BEGIN: us-803d1764
int SFVOM_Stop(void) { return 0x0; }
// LLM-HARNESS-END: us-803d1764

// LLM-HARNESS-BEGIN: us-803d176c
int SFVOM_Pause(void) { return 0x0; }
// LLM-HARNESS-END: us-803d176c

// LLM-HARNESS-BEGIN: us-803d1774
void SFLIB_SetErr(u32 err_code);
void SFVOM_GetWrite(void) {
    SFLIB_SetErr(0xff000701);
}
// LLM-HARNESS-END: us-803d1774

// LLM-HARNESS-BEGIN: us-803d1780
void SFLIB_SetErr(u32 err_code);
void SFVOM_AddWrite(void) {
    SFLIB_SetErr(0xff000701);
}
// LLM-HARNESS-END: us-803d1780

// LLM-HARNESS-BEGIN: us-803d178c
void SFVOM_GetRead() {}
// LLM-HARNESS-END: us-803d178c

// LLM-HARNESS-BEGIN: us-803d17f0
void SFBUF_VfrmAddRead(void* self, u32 a, u32 b, u32 c);
void SFVOM_AddRead(void* self, u32 a, u32 b) {
    SFBUF_VfrmAddRead(self, *(u32*)((u8*)self + 0x2180), a, b);
}
// LLM-HARNESS-END: us-803d17f0

// LLM-HARNESS-BEGIN: us-803d1804
int SFVOM_Seek(void) { return 0x0; }
// LLM-HARNESS-END: us-803d1804
