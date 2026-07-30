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
    u32 val = *(u32*)((u8*)self + 0xb24);
    int flags = 4;
    u8* ptr;
    if (val == 0) {
        flags = -1;
    }
    ptr = (u8*)self + 0x380;
    *(int*)((u8*)self + 0xad4) = flags;
    *(u32*)((u8*)self + 0xad8) = (u32)ptr;
    *(u32*)((u8*)self + 0xae0) = (u32)ptr;
    *(u32*)((u8*)self + 0xae8) = (u32)ptr;
    *(u32*)((u8*)self + 0xaf0) = (u32)ptr;
    *(u32*)((u8*)self + 0xaf8) = (u32)ptr;
    *(u32*)((u8*)self + 0xb00) = (u32)ptr;
    *(int*)((u8*)self + 0xadc) = 8;
    *(int*)((u8*)self + 0xae4) = 8;
    *(int*)((u8*)self + 0xaec) = 8;
    *(int*)((u8*)self + 0xaf4) = 8;
    *(int*)((u8*)self + 0xafc) = 8;
    *(int*)((u8*)self + 0xb04) = 8;
}

void MPVCMC_InitMcOiRt(void* self) {
    u32 val = *(u32*)((u8*)self + 0xb24);
    s32* base = (s32*)((u8*)self + 0xaa4);
    s32 r4 = 4;
    if (val == 0) {
        r4 = -1;
    }
    s32 r0 = *(s16*)((u8*)self + 0xc0e);
    s32 r6 = *(s16*)((u8*)self + 0xc0c);
    base[-1] = r4;
    base[1] = r6;
    base[3] = r6;
    base[5] = r0;
    base[7] = r0;
    base[9] = r0;
    base[11] = r0;
}

void MPVCMC_SetCcnt(void* self) {
    u32 val = *(u32*)((u8*)self + 0xb24);
    int r4;
    if (val != 0) {
        r4 = 4;
    } else {
        r4 = -1;
    }
    *(u32*)((u8*)self + 0xad4) = r4;
    *(u32*)((u8*)self + 0xaa0) = r4;
}
