// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/mpv/mpv_cmc
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void MPVMC08_Init(u32* dst);
void MPVMC16_Init(void* this_ptr);
void mpvcmc_InitMcOiTa(void* self);

void MPVCMC_InitObj(void* self) {
    u8* base = (u8*)self;
    MPVMC08_Init((u32*)(base + 0xA4C));
    MPVMC16_Init(base + 0xA4C);
    mpvcmc_InitMcOiTa(self);
}

void mpvcmc_InitMcOiTa(void* self) {
    u8* base = (u8*)self;
    struct McOiTaPair { void* p; int v; }* e;
    int flags;
    int i;
    e = (struct McOiTaPair*)(base + 0xad8);
    flags = 4;
    if (*(u32*)(base + 0xb24) == 0) {
        flags = -1;
    }
    *(int*)(base + 0xad4) = flags;
    for (i = 0; i < 6; i++) {
        e[i].p = base + 0x380;
        e[i].v = 8;
    }
}

void MPVCMC_InitMcOiRt(void* self) {
    u8* base = (u8*)self;
    s32* e = (s32*)(base + 0xaa4);
    s32 i;
    *(s32*)(base + 0xaa0) = (*(u32*)(base + 0xb24) != 0) ? 4 : -1;
    for (i = 0; i < 2; i++) {
        e[i * 2 + 1] = *(s16*)(base + 0xc0c);
    }
    for (i = 2; i < 6; i++) {
        e[i * 2 + 1] = *(s16*)(base + 0xc0e);
    }
}

void MPVCMC_SetCcnt(void* self) {
    s32 r4 = 4;
    if (*(u32*)((u8*)self + 0xb24) == 0) {
        r4 = -1;
    }
    *(u32*)((u8*)self + 0xad4) = r4;
    *(u32*)((u8*)self + 0xaa0) = r4;
}
