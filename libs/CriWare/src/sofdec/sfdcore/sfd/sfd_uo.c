// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/sfd/sfd_uo
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void SFD_SetUsrSj() {}

int SFUO_Init(void) { return 0x0; }

int SFUO_Finish(void) { return 0x0; }

void SFUO_ExecServer() {}

void SFUO_Create() {}

int SFUO_Destroy(void) { return 0x0; }

int SFUO_RequestStop(void) { return 0x0; }

int SFUO_Start(void) { return 0x0; }

int SFUO_Stop(void) { return 0x0; }

int SFUO_Pause(void) { return 0x0; }

s32 SFLIB_SetErr(void* h, u32 err_code);
s32 SFUO_GetWrite(void* h) {
    return SFLIB_SetErr(h, 0xff000601);
}

s32 SFLIB_SetErr(void* h, u32 err_code);
s32 SFUO_AddWrite(void* h) {
    return SFLIB_SetErr(h, 0xff000601);
}

s32 SFLIB_SetErr(void* h, u32 err_code);
s32 SFUO_GetRead(void* h) {
    return SFLIB_SetErr(h, 0xff000601);
}

s32 SFLIB_SetErr(void* h, u32 err_code);
s32 SFUO_AddRead(void* h) {
    return SFLIB_SetErr(h, 0xff000601);
}

int SFUO_Seek(void) { return 0x0; }
