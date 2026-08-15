// Auto-scaffolded catalog TU for CriWare/src/adx/adxt/srcwii/adx_suwii
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

typedef void (*CvFsErrFn)(void* arg, const char* msg, int level);

void adxwii_err_dvd(void* a, void* b) { ADXERR_CallErrFunc1_((const char*)b); }

extern void cvFsEntryErrFunc(CvFsErrFn fn, void* arg);
extern s32 cvFsAddDev(void* name, void* iface, s32 a);
extern void cvFsSetDefDev(void* dev);
extern void gcCiSetRdMode(void* a, void* b, void* c, u32 val);
extern void mfCiGetInterface(void);
extern void gcCiGetInterface(void);
extern volatile u32 lbl_eu_80519744;
extern u32 lbl_eu_80519748[];

void ADXWII_SetupDvdFs(int media) {
    /* Retail dead-reads rodata lbl_eu_80519744 (a pointer to the module SDK
       version banner at 0x80519708) into r0 before the first cvFsEntryErrFunc
       call; the value is unused. The volatile qualifier keeps the load alive
       (MWCC DCEs the plain read) - same lever as AXRNA_Init. */
    (void)lbl_eu_80519744;
    cvFsEntryErrFunc((CvFsErrFn)adxwii_err_dvd, 0);
    cvFsAddDev(&lbl_eu_80519748[0], mfCiGetInterface, 0);
    cvFsEntryErrFunc((CvFsErrFn)adxwii_err_dvd, 0);
    cvFsAddDev(&lbl_eu_80519748[1], gcCiGetInterface, 0);
    cvFsSetDefDev(&lbl_eu_80519748[1]);
    if (media != 0) {
        gcCiSetRdMode(0, 0, 0, *(u32*)media);
    } else {
        gcCiSetRdMode(0, 0, 0, 0);
    }
}