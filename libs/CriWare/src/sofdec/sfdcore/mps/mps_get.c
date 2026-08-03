// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/mps/mps_get
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

extern s32 MPSLIB_CheckHn(void *handle);
extern s32 MPSLIB_SetErr(void *handle, u32 err_code);

void MPSGET_Init(void) {}

void MPSGET_Finish(void) {}

s32 MPS_GetPackHd(void *handle, void *out) {
    if (MPSLIB_CheckHn(handle)) {
        return MPSLIB_SetErr(NULL, 0xFF020201);
    }
    {
        u32 hi0 = *(u32 *)((u8 *)handle + 0x18);
        u32 lo0 = *(u32 *)((u8 *)handle + 0x1C);
        *(u32 *)((u8 *)out + 0x04) = lo0;
        *(u32 *)((u8 *)out + 0x00) = hi0;
        u32 hi1 = *(u32 *)((u8 *)handle + 0x20);
        u32 lo1 = *(u32 *)((u8 *)handle + 0x24);
        *(u32 *)((u8 *)out + 0x0C) = lo1;
        *(u32 *)((u8 *)out + 0x08) = hi1;
    }
    return 0;
}

s32 MPS_GetSysHd(void *handle, void *out, u32 index) {
    u8 *entry;
    if (MPSLIB_CheckHn(handle)) {
        return MPSLIB_SetErr(NULL, 0xFF020202);
    }
    entry = (u8 *)handle + (index << 5);
    {
        u32 hi0 = *(u32 *)(entry + 0x48);
        u32 lo0 = *(u32 *)(entry + 0x4C);
        *(u32 *)((u8 *)out + 0x04) = lo0;
        *(u32 *)((u8 *)out + 0x00) = hi0;
        u32 hi1 = *(u32 *)(entry + 0x50);
        u32 lo1 = *(u32 *)(entry + 0x54);
        *(u32 *)((u8 *)out + 0x0C) = lo1;
        *(u32 *)((u8 *)out + 0x08) = hi1;
        u32 hi2 = *(u32 *)(entry + 0x58);
        u32 lo2 = *(u32 *)(entry + 0x5C);
        *(u32 *)((u8 *)out + 0x14) = lo2;
        *(u32 *)((u8 *)out + 0x10) = hi2;
        u32 hi3 = *(u32 *)(entry + 0x60);
        u32 lo3 = *(u32 *)(entry + 0x64);
        *(u32 *)((u8 *)out + 0x1C) = lo3;
        *(u32 *)((u8 *)out + 0x18) = hi3;
    }
    return 0;
}

s32 MPS_GetLastSysHd(void *handle, void *out) {
    if (MPSLIB_CheckHn(handle)) {
        return MPSLIB_SetErr(NULL, 0xFF020202);
    }
    {
        u32 hi0 = *(u32 *)((u8 *)handle + 0x28);
        u32 lo0 = *(u32 *)((u8 *)handle + 0x2C);
        *(u32 *)((u8 *)out + 0x04) = lo0;
        *(u32 *)((u8 *)out + 0x00) = hi0;
        u32 hi1 = *(u32 *)((u8 *)handle + 0x30);
        u32 lo1 = *(u32 *)((u8 *)handle + 0x34);
        *(u32 *)((u8 *)out + 0x0C) = lo1;
        *(u32 *)((u8 *)out + 0x08) = hi1;
        u32 hi2 = *(u32 *)((u8 *)handle + 0x38);
        u32 lo2 = *(u32 *)((u8 *)handle + 0x3C);
        *(u32 *)((u8 *)out + 0x14) = lo2;
        *(u32 *)((u8 *)out + 0x10) = hi2;
        u32 hi3 = *(u32 *)((u8 *)handle + 0x40);
        u32 lo3 = *(u32 *)((u8 *)handle + 0x44);
        *(u32 *)((u8 *)out + 0x1C) = lo3;
        *(u32 *)((u8 *)out + 0x18) = hi3;
    }
    return 0;
}

s32 MPS_GetPketHd(void *handle, void *out) {
    if (MPSLIB_CheckHn(handle)) {
        return MPSLIB_SetErr(NULL, 0xFF020203);
    }
    {
        u32 hi0 = *(u32 *)((u8 *)handle + 0xA8);
        u32 lo0 = *(u32 *)((u8 *)handle + 0xAC);
        *(u32 *)((u8 *)out + 0x04) = lo0;
        *(u32 *)((u8 *)out + 0x00) = hi0;
        u32 hi1 = *(u32 *)((u8 *)handle + 0xB0);
        u32 lo1 = *(u32 *)((u8 *)handle + 0xB4);
        *(u32 *)((u8 *)out + 0x0C) = lo1;
        *(u32 *)((u8 *)out + 0x08) = hi1;
        u32 hi2 = *(u32 *)((u8 *)handle + 0xB8);
        u32 lo2 = *(u32 *)((u8 *)handle + 0xBC);
        *(u32 *)((u8 *)out + 0x14) = lo2;
        *(u32 *)((u8 *)out + 0x10) = hi2;
        u32 hi3 = *(u32 *)((u8 *)handle + 0xC0);
        u32 lo3 = *(u32 *)((u8 *)handle + 0xC4);
        *(u32 *)((u8 *)out + 0x1C) = lo3;
        *(u32 *)((u8 *)out + 0x18) = hi3;
        u32 hi4 = *(u32 *)((u8 *)handle + 0xC8);
        u32 lo4 = *(u32 *)((u8 *)handle + 0xCC);
        *(u32 *)((u8 *)out + 0x24) = lo4;
        *(u32 *)((u8 *)out + 0x20) = hi4;
    }
    return 0;
}
