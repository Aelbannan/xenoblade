// ADX TSW Wii-side DVD filesystem setup (srcwii).
#include <types.h>

// Device-name table in rodata: { "MFS\0", "GCD\0" }.
extern u32 lbl_eu_80519748[];

// Retail hoists a dead load of this rodata pointer (to the module SDK version
// banner) right after the prologue. Declaring it volatile (the same lever as
// lbl_eu_8051914C in ax_rna.c / lbl_eu_80517598 in ahx_sjd.c) keeps the read
// alive at the top of the body.
extern volatile u32 lbl_eu_80519744;

typedef void (*CvFsErrFn)(void* arg, const char* msg, int level);

extern void cvFsEntryErrFunc(CvFsErrFn fn, void* arg);
extern s32 cvFsAddDev(void* name, void* iface, s32 a);
extern void cvFsSetDefDev(void* dev);
extern void gcCiSetRdMode(void* a, void* b, void* c, u32 val);
extern void mfCiGetInterface(void);
extern void gcCiGetInterface(void);
extern void ADXERR_CallErrFunc1_(const char* msg);

// Opaque user-argument record passed through by cvFs (unused here).
typedef struct CvFsErrArg CvFsErrArg;

// cvFs error handler: forward every message to the ADX error func.
void adxwii_err_dvd(CvFsErrArg* arg, const char* msg);

void ADXWII_SetupDvdFs(int media) {
    // Dead read kept for retail parity (see note above).
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

void adxwii_err_dvd(CvFsErrArg* arg, const char* msg) {
    (void)arg;
    ADXERR_CallErrFunc1_(msg);
}
