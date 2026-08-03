// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/sfd/sfd_uo
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void SFD_SetUsrSj() {}

int SFUO_Init(void) { return 0x0; }

int SFUO_Finish(void) { return 0x0; }

void SFUO_ExecServer() {}

extern void SFBUF_SetUoch(void* entry, void* a, void* b, void* c);

s32 SFUO_Create(void* self) {
    u8* end = (u8*)self + 0x2638;
    u8* e = end + 4;
    u32 i = 0;
    void* x = *(void**)((u8*)self + 0x2208);
    *(void**)((u8*)self + 0x2200) = end;
    *(u32*)((u8*)self + 0x2638) = 0;
    do {
        *(u32*)((u8*)e + 0) = 0;
        *(u32*)((u8*)e + 4) = 0;
        *(u32*)((u8*)e + 8) = 0;
        *(u32*)((u8*)e + 12) = 0;
        SFBUF_SetUoch(self, x, (void*)i, e);
        i++;
        e += 16;
    } while (i < 3);
    return 0;
}

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
