// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/sfd/sfd_tst
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void SFTST_Create() {}

void SFTST_SetTstFlg(void* self, u32 val) { *(u32*)((u8*)self + 0x0) = val; }

void SFTST_SetTolerance(void* self, s32* param) {
    *(u32*)((u8*)self + 0x138) = param[0];
    *(u32*)((u8*)self + 0x13c) = param[1];
    *(u32*)((u8*)self + 0x140) = param[2];
    *(u32*)((u8*)self + 0x144) = param[3];
}

void SFTST_SetExcessErr(void* self, s32* param) {
    *(u32*)((u8*)self + 0x148) = param[0];
    *(u32*)((u8*)self + 0x14c) = param[1];
    *(u32*)((u8*)self + 0x150) = param[2];
    *(u32*)((u8*)self + 0x154) = param[3];
}

void SFTST_SetAdjStart(void* self, s32* param) {
    *(u32*)((u8*)self + 0x158) = param[0];
    *(u32*)((u8*)self + 0x15c) = param[1];
    *(u32*)((u8*)self + 0x160) = param[2];
    *(u32*)((u8*)self + 0x164) = param[3];
}

void SFTST_SetAdjPoff(void* self, s32* param) {
    *(u32*)((u8*)self + 0x168) = param[0];
    *(u32*)((u8*)self + 0x16c) = param[1];
    *(u32*)((u8*)self + 0x170) = param[2];
    *(u32*)((u8*)self + 0x174) = param[3];
}

void SFTST_SetMovaveRange(void* self, s32 val) {
    if (val > 0) {
        *(u32*)((u8*)self + 0x10) = val;
    }
}

void SFTST_Pause(void* self, u32 val) { *(u32*)((u8*)self + 0x4) = val; }

void SFTST_SetAdjFlg(void* self, u32 val) { *(u32*)((u8*)self + 0xc) = val; }

void SFTST_GoNextFrame() {}

void SFTST_SetSpeed(void* self, u32 a, u32 b) {
    *(u32*)((u8*)self + 0x198) = a;
    *(u32*)((u8*)self + 0x19c) = b;
}

void SFTST_Calc() {}

void sftst_CalcSub() {}
