// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/sfd/sfd_see
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void SFSEE_InitHn(void* self) {
    *(u32*)((u8*)self + 0x00) = 0;
    *(u32*)((u8*)self + 0x04) = 0;
    *(u32*)((u8*)self + 0x08) = -3;
    *(u32*)((u8*)self + 0x0C) = 1;
}

void SFD_EntrySeek() {}

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

void sfsee_ExecHeadAnaly();
void sfsee_ExecFinAnaly();

void SFSEE_ExecServer(void* self) {
    if (*(u32*)((u8*)self + 0x2670) != 0) {
        sfsee_ExecHeadAnaly(self);
        sfsee_ExecFinAnaly(self);
    }
}

void sfsee_ExecHeadAnaly() {}

void SFD_SetFileSize() {}

void SFD_SetTotTime() {}

void SFD_SetByteRate() {}

void SFD_SetSeekPos() {}

void sfsee_ExecFinAnaly() {}

void sfsee_UpdateEByteRate() {}
