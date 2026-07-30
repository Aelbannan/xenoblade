// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/sfd/sfd_ply
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

extern u32 lbl_eu_80619BA0;
void SFPLY_Init(void) {
    lbl_eu_80619BA0 = 0;
}

extern void SFLIB_LockCs(void *);
extern void SFLIB_UnlockCs(void *);

void SFD_VbIn(void) {
    void* cs;
    SFLIB_LockCs(&cs);
    SFTIM_VbIn();
    SFLIB_UnlockCs(&cs);
}

int SFD_IsHnSvrWait(void* self) {
    int status = *(int*)((u8*)self + 0x54);
    if ((unsigned int)(status - 1) > 3) return 1;
    if (*(int*)((u8*)self + 0x64) == 1) return 1;
    return (*(int*)((u8*)self + 0x50) == 0) ? 1 : 0;
}

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

void SFPLY_AddDecPic(void* self, int delta, void* param) {
    void (*cb)(void*, void*, u32*) = *(void(**)(void*, void*, u32*))((u8*)self + 0xd6c);
    *(u32*)((u8*)self + 0x960) += delta;
    if (cb == NULL) return;
    cb(*(void**)((u8*)self + 0xd70), param, (u32*)&((u8*)self)[0x960]);
}

void SFPLY_AddSkipPic(void* self, int delta, void* param) {
    void (*cb)(void*, void*, u32*) = *(void(**)(void*, void*, u32*))((u8*)self + 0xd74);
    u32 count = *(u32*)((u8*)self + 0x964) + delta;
    *(u32*)((u8*)self + 0x964) = count;
    if (cb != NULL) {
        cb(*(void**)((u8*)self + 0xd78), param, (u32*)((u8*)self + 0x960));
    }
}

void SFD_Destroy() {}

void SFD_Start() {}

s32 SFTRN_CallTrtTrif(void* self, int a, int b, int c, int d);
s32 fn_803CD484(void* self) {
    s32 result = SFTRN_CallTrtTrif(self, 2, 6, 0, 0);
    if (result == 0) {
        return 0;
    } else {
        return result;
    }
}

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
