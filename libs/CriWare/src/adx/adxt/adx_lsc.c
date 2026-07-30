#include <harness_catalog.h>

extern void ADXCRS_Enter(void);
extern void LSC_ExecServer(void);
extern void ADXCRS_Leave(void);

extern void *lbl_eu_805E3328;

void ADXT_ExecLscSvr(void) {
    void *base = (void *)&lbl_eu_805E3328;
    void (*cb1)(s32);
    s32 arg1;
    void (*cb2)(s32);
    s32 arg2;

    ADXCRS_Enter();

    cb1 = *(void (**)(s32))base;
    if (cb1 != NULL) {
        arg1 = *(s32 *)((u8 *)base + 4);
        cb1(arg1);
    }

    LSC_ExecServer();

    cb2 = *(void (**)(s32))((u8 *)base + 8);
    if (cb2 != NULL) {
        arg2 = *(s32 *)((u8 *)base + 12);
        cb2(arg2);
    }

    ADXCRS_Leave();
}
