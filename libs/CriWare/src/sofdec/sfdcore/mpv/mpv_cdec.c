// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/mpv/mpv_cdec
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

extern void fn_803A7770(void);
extern void fn_803BDF3C(void* a, s32 b);
extern void DCT_IsrInit(void);
extern void DCT_IsrInitScaleTbl(void* tbl);
extern u32 lbl_eu_8051C080;
extern u32 lbl_eu_80602A68[];
extern u32 lbl_eu_80602A6C[];

void MPVCDEC_Init(void* self) {
    lbl_eu_80602A68[0] = (u32)self;
    fn_803BDF3C((void*)fn_803A7770, 0);
    DCT_IsrInit();
    DCT_IsrInitScaleTbl((u8*)self + 0x1160);
    lbl_eu_80602A6C[2] = (u32)&lbl_eu_8051C080;
}

void fn_803A7770(void) { MPVERR_SetCode(0); }

extern void DCT_IsrTrans(void* self, s32 val);

void MPVCDEC_StartFrame(void* self) {
    u8* base = (u8*)self;
    *(void**)(base + 0xc84) = DCT_IsrTrans;
    *(u32*)(base + 0x9b4) = lbl_eu_80602A68[0] + 0x1160;
    *(u32*)(base + 0xc68) = ((u32*)lbl_eu_80602A6C[2])[0];
    *(u32*)(base + 0xc6c) = ((u32*)lbl_eu_80602A6C[2])[1];
}

void mpvcdec_IntraBlocksInt1() {}

void mpvcdec_NintraBlocksInt1() {}
