// Auto-scaffolded catalog TU for CriWare/src/adx/ahx/ahx_link
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void ADXT_AttachAhx() {}

extern void (*lbl_eu_805E4F20)(void);
void ADXT_DetachAhx(void) {
    if (lbl_eu_805E4F20 != NULL)
        (*lbl_eu_805E4F20)();
}

extern char lbl_eu_805174A0[];
extern void ADXT_Stop(void* handle);
extern void ADXM_Lock(void);
extern void ADXM_Unlock(void);
extern void AHXSJD_Stop(void* sjd);
extern void AHXSJD_Destroy(void* sjd);
extern void AHXSJD_Finish(void);

void AHXLINK_DetachAhx(void* handle) {
    void* r3;
    void* r31;
    void* sjd;

    if (handle == NULL) {
        ADXERR_CallErrFunc1_(lbl_eu_805174A0 + 0x7E);
        return;
    }

    ADXT_Stop(handle);
    r3 = *(void**)((u8*)handle + 0x04);
    r31 = *(void**)((u8*)r3 + 0x04);
    sjd = *(void**)((u8*)r31 + 0xB8);

    if (sjd == NULL) return;

    ADXM_Lock();
    AHXSJD_Stop(sjd);
    AHXSJD_Destroy(sjd);
    *(void**)((u8*)r31 + 0xB8) = NULL;
    ADXM_Unlock();
    AHXSJD_Finish();
}

void AHXLINK_SetDecSmpl(u32 val) { AHXSJD_SetDecSmpl(val); }

void AHXLINK_TermSupply(void) { AHXSJD_TermSupply(); }

extern s32 AHXSJD_GetStat(void* sjd);
extern void AHXSJD_SetLnkSw(void* sjd, int val);
extern void AHXSJD_Start(void* sjd);
extern void AHXSJD_ExecHndl(void* sjd);
extern s32 AHXSJD_GetDecNumSmpl(void* sjd);
extern s32 AHXSJD_GetDecDtLen(void* sjd);

void AHXLINK_ExecOneAhx(void* handle) {
    int i;
    void* sjd;
    s32 stat;
    int delta;

    ADXM_Lock();
    sjd = *(void**)((u8*)handle + 0xB8);

    stat = AHXSJD_GetStat(sjd);
    if (stat == 0) {
        *(u32*)((u8*)handle + 0x90) = 0;
        *(u32*)((u8*)handle + 0x88) = 0;
        AHXSJD_Stop(sjd);
    }

    {
        s32 state = *(s32*)((u8*)handle + 0x04);
        if (state == 1) {
            if (*(u32*)((u8*)handle + 0xAC) == 0) {
                AHXSJD_SetLnkSw(sjd, 0);
            } else {
                AHXSJD_SetLnkSw(sjd, 1);
            }
            *(u32*)((u8*)handle + 0x88) = 0;
            AHXSJD_Start(sjd);
            *(u32*)((u8*)handle + 0x04) = 2;
        } else if (state == 2) {
            for (i = 0; i < (int)*(u32*)((u8*)handle + 0xC0); i++) {
                AHXSJD_ExecHndl(sjd);
            }
            delta = AHXSJD_GetDecNumSmpl(sjd) - *(int*)((u8*)handle + 0x88);
            *(u32*)((u8*)handle + 0x90) = delta;
            *(u32*)((u8*)handle + 0x88) = *(u32*)((u8*)handle + 0x88) + delta;
            *(u32*)((u8*)handle + 0x94) = AHXSJD_GetDecDtLen(sjd);
            stat = AHXSJD_GetStat(sjd);
            if (stat == 3) {
                AHXSJD_Stop(sjd);
                *(u32*)((u8*)handle + 0x04) = 0;
            }
        }
    }

    ADXM_Unlock();
}
