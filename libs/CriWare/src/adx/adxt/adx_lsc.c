#include <harness_catalog.h>

extern void ADXCRS_Enter(void);
extern void LSC_ExecServer(void);
extern void ADXCRS_Leave(void);

void (*adxt_lsc_svr_start_cb)(s32);
s32 adxt_lsc_svr_start_cb_obj;
void (*adxt_lsc_svr_end_cb)(s32);
s32 adxt_lsc_svr_end_cb_obj;

void ADXT_ExecLscSvr(void) {
    void (*cb1)(s32);
    s32 arg1;
    void (*cb2)(s32);
    s32 arg2;

    ADXCRS_Enter();

    cb1 = adxt_lsc_svr_start_cb;
    if (cb1 != NULL) {
        arg1 = adxt_lsc_svr_start_cb_obj;
        cb1(arg1);
    }

    LSC_ExecServer();

    cb2 = adxt_lsc_svr_end_cb;
    if (cb2 != NULL) {
        arg2 = adxt_lsc_svr_end_cb_obj;
        cb2(arg2);
    }

    ADXCRS_Leave();
}
