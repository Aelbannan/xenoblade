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
    *(u64*)((u8*)out + 0x00) = *(u64*)((u8*)handle + 0x18);
    *(u64*)((u8*)out + 0x08) = *(u64*)((u8*)handle + 0x20);
    return 0;
}

s32 MPS_GetSysHd(void *handle, void *out, u32 index) {
    u8 *entry;
    if (MPSLIB_CheckHn(handle)) {
        return MPSLIB_SetErr(NULL, 0xFF020202);
    }
    entry = (u8*)handle + (index << 5);
    *(u64*)((u8*)out + 0x00) = *(u64*)(entry + 0x48);
    *(u64*)((u8*)out + 0x08) = *(u64*)(entry + 0x50);
    *(u64*)((u8*)out + 0x10) = *(u64*)(entry + 0x58);
    *(u64*)((u8*)out + 0x18) = *(u64*)(entry + 0x60);
    return 0;
}

s32 MPS_GetLastSysHd(void *handle, void *out) {
    if (MPSLIB_CheckHn(handle)) {
        return MPSLIB_SetErr(NULL, 0xFF020202);
    }
    *(u64*)((u8*)out + 0x00) = *(u64*)((u8*)handle + 0x28);
    *(u64*)((u8*)out + 0x08) = *(u64*)((u8*)handle + 0x30);
    *(u64*)((u8*)out + 0x10) = *(u64*)((u8*)handle + 0x38);
    *(u64*)((u8*)out + 0x18) = *(u64*)((u8*)handle + 0x40);
    return 0;
}

s32 MPS_GetPketHd(void *handle, void *out) {
    if (MPSLIB_CheckHn(handle)) {
        return MPSLIB_SetErr(NULL, 0xFF020203);
    }
    *(u64*)((u8*)out + 0x00) = *(u64*)((u8*)handle + 0xA8);
    *(u64*)((u8*)out + 0x08) = *(u64*)((u8*)handle + 0xB0);
    *(u64*)((u8*)out + 0x10) = *(u64*)((u8*)handle + 0xB8);
    *(u64*)((u8*)out + 0x18) = *(u64*)((u8*)handle + 0xC0);
    *(u64*)((u8*)out + 0x20) = *(u64*)((u8*)handle + 0xC8);
    return 0;
}
