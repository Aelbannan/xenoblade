// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/sfd/sfd_aoap
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void SFD_SetOutVol() {}

void SFD_GetOutVol() {}

void SFAOAP_SetSpeed() {}

int SFAOAP_Init(void) { return 0x0; }

int SFAOAP_Finish(void) { return 0x0; }

void SFAOAP_ExecServer() {}

void SFAOAP_Create() {}

int SFAOAP_Destroy(void) { return 0x0; }

int SFAOAP_RequestStop(void* param_1) {
    SFSET_GetCond(param_1, 6);
    return 0;
}

void SFAOAP_Start() {}

void SFAOAP_Stop() {}

void SFAOAP_Pause() {}

s32 SFLIB_SetErr(void* h, u32 err_code);
s32 SFAOAP_GetWrite(void* h) {
    return SFLIB_SetErr(h, 0xff000a01);
}

s32 SFLIB_SetErr(void* h, u32 err_code);
s32 SFAOAP_AddWrite(void* h) {
    return SFLIB_SetErr(h, 0xff000a01);
}

s32 SFLIB_SetErr(void* h, u32 err_code);
s32 SFAOAP_GetRead(void* h) {
    return SFLIB_SetErr(h, 0xff000a01);
}

s32 SFLIB_SetErr(void* h, u32 err_code);
s32 SFAOAP_AddRead(void* h) {
    return SFLIB_SetErr(h, 0xff000a01);
}

int SFAOAP_Seek(void) { return 0x0; }
