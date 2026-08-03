// Auto-scaffolded catalog TU for CriWare/src/adx/adxt/srcwii/adx_suwii
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void adxwii_err_dvd(void* a, void* b) { ADXERR_CallErrFunc1_((const char*)b); }

extern void cvFsEntryErrFunc(void* fn, ...);
extern s32 cvFsAddDev(void* name, void* iface, s32 a);
extern void cvFsSetDefDev(void* dev);
extern void gcCiSetRdMode(u32 a, u32 b, u32 c, u32 d);
extern void mfCiGetInterface(void);
extern void gcCiGetInterface(void);
extern u32 lbl_eu_80519744;
extern u32 lbl_eu_80519748[];

void ADXWII_SetupDvdFs(s32 arg1) {
    if (lbl_eu_80519744 == 0) {
        cvFsEntryErrFunc(adxwii_err_dvd);
        cvFsAddDev(&lbl_eu_80519748[0], mfCiGetInterface, 0);
        cvFsEntryErrFunc(adxwii_err_dvd, 0);
        cvFsAddDev(&lbl_eu_80519748[1], gcCiGetInterface, 0);
        cvFsSetDefDev(&lbl_eu_80519748[1]);
        if (arg1 == 0) {
            gcCiSetRdMode(0, 0, 0, 0);
        } else {
            gcCiSetRdMode(*(u32*)((u8*)arg1 + 0), 0, 0, 0);
        }
    }
}
