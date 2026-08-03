// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/sfd/sfd_uo
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

extern void SFBUF_SetUoch(void* entry, void* a, void* b, void* c);
extern s32 SFLIB_CheckHn(void* h);



s32 SFD_SetUsrSj(void* self, u32 a, u32 b, u32 c) {
    if (SFLIB_CheckHn(self))
        return SFLIB_SetErr(NULL, 0xFF000191);
    {
        s32 v = *(s32*)((u8*)self + 0x2208);
        u8* base = *(u8**)((u8*)self + 0x2200);
        if (v == 8)
            return SFLIB_SetErr(self, 0xFF000602);
        {
            u8* e = base + (a << 4);
            *(u32*)((u8*)e + 4) = b;
            *(u32*)((u8*)e + 8) = c;
            *(u32*)((u8*)e + 12) = 0;
            *(u32*)((u8*)e + 16) = 0;
            SFBUF_SetUoch(self, *(void**)((u8*)self + 0x2208), (void*)a, (u8*)e + 4);
        }
    }
    return 0;
}

int SFUO_Init(void) { return 0x0; }

int SFUO_Finish(void) { return 0x0; }

void SFUO_ExecServer() {}


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

s32 SFUO_GetWrite(void* h) {
    return SFLIB_SetErr(h, 0xff000601);
}

s32 SFUO_AddWrite(void* h) {
    return SFLIB_SetErr(h, 0xff000601);
}

s32 SFUO_GetRead(void* h) {
    return SFLIB_SetErr(h, 0xff000601);
}

s32 SFUO_AddRead(void* h) {
    return SFLIB_SetErr(h, 0xff000601);
}

int SFUO_Seek(void) { return 0x0; }
