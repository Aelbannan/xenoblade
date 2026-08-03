// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/mpv/mpv_dec
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

extern char lbl_eu_8051C088[];
extern int strcmp(const char* a, const char* b);

s32 MPVDEC_CheckVersion(const char* version, unsigned int size, s32 v) {
    if (strcmp(lbl_eu_8051C088, version) != 0)
        return -1;
    switch (size) {
    default:
        return -1;
    case 0xDAC:
        return ((v - 128) | (128 - v)) >> 31;
    }
}

void MPVDEC_DecIpicMb() {}

void MPVDEC_DecPpicMb() {}

void MPVDEC_ResetMv(void* self) {
    *(u32*)((u8*)self + 0x10) = 0;
    *(u32*)((u8*)self + 0x14) = 0;
    *(u32*)((u8*)self + 0x18) = 0;
    *(u32*)((u8*)self + 0x1c) = 0;
}

void MPVDEC_ResetDc(void* self) {
    *(u32*)((u8*)self + 0xcf4) = 0x400;
    *(u32*)((u8*)self + 0xcf8) = 0x400;
    *(u32*)((u8*)self + 0xcf0) = 0x400;
}

void mpvdec_MotionSub() {}

void MPVDEC_DecBpicMb() {}
