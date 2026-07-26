// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/sfd/sfd_uo
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

// LLM-HARNESS-BEGIN: us-803d1400
void SFD_SetUsrSj() {}
// LLM-HARNESS-END: us-803d1400

// LLM-HARNESS-BEGIN: us-803d14c0
int SFUO_Init(void) { return 0x0; }
// LLM-HARNESS-END: us-803d14c0

// LLM-HARNESS-BEGIN: us-803d14c8
int SFUO_Finish(void) { return 0x0; }
// LLM-HARNESS-END: us-803d14c8

// LLM-HARNESS-BEGIN: us-803d14d0
void SFUO_ExecServer() {}
// LLM-HARNESS-END: us-803d14d0

// LLM-HARNESS-BEGIN: us-803d1568
void SFUO_Create() {}
// LLM-HARNESS-END: us-803d1568

// LLM-HARNESS-BEGIN: us-803d15e8
int SFUO_Destroy(void) { return 0x0; }
// LLM-HARNESS-END: us-803d15e8

// LLM-HARNESS-BEGIN: us-803d15f0
int SFUO_RequestStop(void) { return 0x0; }
// LLM-HARNESS-END: us-803d15f0

// LLM-HARNESS-BEGIN: us-803d15f8
int SFUO_Start(void) { return 0x0; }
// LLM-HARNESS-END: us-803d15f8

// LLM-HARNESS-BEGIN: us-803d1600
int SFUO_Stop(void) { return 0x0; }
// LLM-HARNESS-END: us-803d1600

// LLM-HARNESS-BEGIN: us-803d1608
int SFUO_Pause(void) { return 0x0; }
// LLM-HARNESS-END: us-803d1608

// LLM-HARNESS-BEGIN: us-803d1610
s32 SFLIB_SetErr(void* h, u32 err_code);
s32 SFUO_GetWrite(void* h) {
    return SFLIB_SetErr(h, 0xff000601);
}
// LLM-HARNESS-END: us-803d1610

// LLM-HARNESS-BEGIN: us-803d161c
s32 SFLIB_SetErr(void* h, u32 err_code);
s32 SFUO_AddWrite(void* h) {
    return SFLIB_SetErr(h, 0xff000601);
}
// LLM-HARNESS-END: us-803d161c

// LLM-HARNESS-BEGIN: us-803d1628
s32 SFLIB_SetErr(void* h, u32 err_code);
s32 SFUO_GetRead(void* h) {
    return SFLIB_SetErr(h, 0xff000601);
}
// LLM-HARNESS-END: us-803d1628

// LLM-HARNESS-BEGIN: us-803d1634
s32 SFLIB_SetErr(void* h, u32 err_code);
s32 SFUO_AddRead(void* h) {
    return SFLIB_SetErr(h, 0xff000601);
}
// LLM-HARNESS-END: us-803d1634

// LLM-HARNESS-BEGIN: us-803d1640
int SFUO_Seek(void) { return 0x0; }
// LLM-HARNESS-END: us-803d1640
