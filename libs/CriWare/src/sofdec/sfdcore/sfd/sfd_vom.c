// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/sfd/sfd_vom
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

int SFVOM_Init(void) { return 0x0; }

int SFVOM_Finish(void) { return 0x0; }

void SFVOM_ExecServer() {}

int SFVOM_Create(void) { return 0x0; }

int SFVOM_Destroy(void) { return 0x0; }

int SFVOM_RequestStop(void) { return 0x0; }

int SFVOM_Start(void) { return 0x0; }

int SFVOM_Stop(void) { return 0x0; }

int SFVOM_Pause(void) { return 0x0; }

s32 SFLIB_SetErr(void* h, u32 err_code);
s32 SFVOM_GetWrite(void* h) {
    return SFLIB_SetErr(h, 0xff000701);
}

s32 SFLIB_SetErr(void* h, u32 err_code);
s32 SFVOM_AddWrite(void* h) {
    return SFLIB_SetErr(h, 0xff000701);
}

extern s32 SFBUF_VfrmGetRead(void* self, void* buf, void* out, void* arg3);

s32 SFVOM_GetRead(void* self, void* out, void* arg3) {
    s32 type = *(s32*)((u8*)self + 0x54);
    s32 r;
    if (type == 3 || type == 4)
        goto call;
    {
        s32 z = 0;
        *(s32*)out = z;
        return z;
    }
call:
    r = SFBUF_VfrmGetRead(self, *(void**)((u8*)self + 0x2180), out, arg3);
    if (r != 0)
        return r;
    return 0;
}

void SFBUF_VfrmAddRead(void* self, u32 a, u32 b, u32 c);
void SFVOM_AddRead(void* self, u32 a, u32 b) {
    SFBUF_VfrmAddRead(self, *(u32*)((u8*)self + 0x2180), a, b);
}

int SFVOM_Seek(void) { return 0x0; }
