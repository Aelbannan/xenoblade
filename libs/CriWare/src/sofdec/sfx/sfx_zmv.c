// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfx/sfx_zmv
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void SFXZ_Init() {}

void SFXZ_Create() {}

void SFXZ_Destroy(void* self) {
    if (self == NULL) return;
    *(u32*)self = 0;
}

void sfxzmv_SetTagGrp();
void SFXZ_SetTagInf(void* self, u32 a, u32 b) {
    *(u32*)((u8*)self + 0x08) = 1;
    *(u32*)((u8*)self + 0x0c) = a;
    *(u32*)((u8*)self + 0x10) = b;
    sfxzmv_SetTagGrp();
}

void sfxzmv_SetTagGrp() {}

void SFXZ_GetZfrmRange() {}

void sfxzmv_MakeOrgZ32TblByDirect() {}

void sfxzmv_MakeOrgZ32TblByCCIR() {}

void SFXZ_MakeCnvZTbl() {}

void sfxzmv_MakeZ16TblFromOrgZ32() {}

void sfxzmv_MakeZ32TblFromOrgZ32() {}
