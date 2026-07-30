// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/sfd/sfd_pts
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void SFPTS_InitPtsQue(void* self) {
    *(u32*)((u8*)self) = 0;
    *(u32*)((u8*)self + 4) = 0;
    *(u32*)((u8*)self + 8) = 0;
    *(u32*)((u8*)self + 0xc) = 0;
    *(u32*)((u8*)self + 0x10) = 0;
}

void SFPTS_ResetPtsQue(void* self) {
    *(u32*)((u8*)self + 0x08) = 0;
    *(u32*)((u8*)self + 0x0c) = 0;
    *(u32*)((u8*)self + 0x10) = 0;
}

void SFD_SetVideoPts() {}

void SFPTS_WritePtsQue() {}

void SFPTS_ReadPtsQue() {}

void sfpts_SearchPtsQue() {}

int SFPTS_IsPtsQueFull(void* self, int idx) {
    u8* p = (u8*)self + idx * 0x74;
    if (*(u32*)(p + 0x13f0) == 0) return 0;
    s32 a = *(s32*)(p + 0x13f8);
    s32 b = *(s32*)(p + 0x13f4);
    return ((s64)a - (s64)b) >> 32;
}
