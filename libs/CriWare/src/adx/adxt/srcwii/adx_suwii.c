// ADX TSW Wii-side DVD filesystem setup (srcwii).
#include <types.h>

extern char lbl_eu_80519708[];
struct __attribute__((packed)) S_rodata_80519744 { const char* ptr; char names[8]; };
__declspec(section ".rodata") __attribute__((aligned(4))) const struct S_rodata_80519744 rodata_80519744 = {lbl_eu_80519708, {0x4D,0x46,0x53,0x00,0x47,0x43,0x44,0x00}};
#define lbl_eu_80519744 (rodata_80519744.ptr)
#define lbl_eu_80519748 (rodata_80519744.names)

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
    (void)*(volatile const char* const*)&lbl_eu_80519744;

    cvFsEntryErrFunc((CvFsErrFn)adxwii_err_dvd, 0);
    cvFsAddDev((void*)lbl_eu_80519748, mfCiGetInterface, 0);
    cvFsEntryErrFunc((CvFsErrFn)adxwii_err_dvd, 0);
    cvFsAddDev((void*)(lbl_eu_80519748+4), gcCiGetInterface, 0);
    cvFsSetDefDev((void*)(lbl_eu_80519748+4));

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
