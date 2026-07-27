// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/sfd/sfd_ply
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

extern u32 lbl_eu_80619BA0;
void SFPLY_Init(void) {
    lbl_eu_80619BA0 = 0;
}

void SFD_VbIn() {}

void SFD_IsHnSvrWait() {}

void SFD_ExecOne() {}

void sfply_ExecOne() {}

void sfply_StatPrep() {}

void fn_803CC170() {}

void fn_803CC238() {}

void sfply_IsBpaOn() {}

void sfply_IsBpaOff() {}

void sfply_IsEtrg() {}

void criware_803C9FC0() {}

void sfply_IsPlayTimeAutoStop() {}

void criware_803CA124() {}

void SFD_Create() {}

void sfply_InitHn() {}

void sfply_InitPlyInf() {}

void SFPLY_AddDecPic() {}

void SFPLY_AddSkipPic() {}

void SFD_Destroy() {}

void SFD_Start() {}

void fn_803CD484() {}

void SFD_Stop() {}

extern char lbl_eu_80606E38[];
u32 SFPLY_GetResetFlg(void) {
    u32* p = (u32*)(lbl_eu_80606E38 + 0x1f0);
    return *p;
}

void sfply_ResetHn() {}

void SFD_RequestStop() {}

void SFD_TermSupply() {}

void SFD_GetFrm() {}

void SFD_RelFrm() {}

void SFD_SetSupplySj() {}
