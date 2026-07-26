// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/sfd/sfd_mpvf
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void SFMPVF_InitPool() {}

void SFD_SetMpvParaTbl() {}

void sfmpvf_CheckMpvPara() {}

void SFMPVF_ReadGlobalMpvPara() {}

void SFMPVF_WriteGlobalMpvPara() {}

void SFD_CalcYccPlane() {}

void SFD_SetPicUsrBuf() {}

extern u32 lbl_eu_80619B20[];
void sfmpvf_SetPicUsrBuf();
void SFMPVF_ResetPicUsrBuf(void) {
    sfmpvf_SetPicUsrBuf(lbl_eu_80619B20[0x1c], lbl_eu_80619B20[0x1d], lbl_eu_80619B20[0x1e]);
}

void sfmpvf_SetPicUsrBuf() {}

void SFMPVF_InitPicUsr() {}

void SFMPVF_GetRead() {}

void SFMPVF_AddRead() {}

void SFMPVF_TermDec(void) {}

u32 SFMPVF_IsTermDec(void* self) { return *(u32*)((u8*)self + 0x27f0); }

void SFMPVF_SetGopStat() {}

void SFMPVF_GetNumFrm() {}

void SFMPVF_AllocFrm() {}

void SFMPVF_FreeFrm(void* self) {
    if (self != NULL) {
        *(u32*)((u8*)self + 0x00) = 0;
    }
}

void SFMPVF_StbyFrm(void* self) {
    if (self != NULL) {
        *(u32*)((u8*)self + 0x00) = 2;
    }
}

void SFMPVF_RefStbyFrm(void* self) {
    if (self != NULL) {
        *(u32*)((u8*)self + 0x00) = 4;
    }
}

void SFMPVF_EndRefFrm(void* self) {
    if (self == NULL) return;
    *(u32*)self = 0;
    memset((u8*)self + 4, 0, 0x24);
}

void SFMPVF_HoldFrm() {}

void SFD_IsNextFrmReady() {}

void sfmpvf_SearchStbyFrm() {}

void sfmpvf_IsChkFirst() {}

void SFMPVF_ChkImageSize() {}
