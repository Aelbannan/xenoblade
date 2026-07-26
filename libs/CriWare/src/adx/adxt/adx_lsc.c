// Auto-scaffolded catalog TU for CriWare/src/adx/adxt/adx_lsc
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

struct ADXT_LscGlobals {
    void (*pre_callback)(struct ADXT_LscCallbackParam*);
    struct ADXT_LscCallbackParam* pre_param;
    void (*post_callback)(struct ADXT_LscCallbackParam*);
    struct ADXT_LscCallbackParam* post_param;
};

#define ADXT_LSC_GLOBALS ((struct ADXT_LscGlobals*)0x805E3328)

struct ADXT_LscCallbackParam {
    int placeholder;
};

void ADXT_ExecLscSvr(void) {
    ADXCRS_Enter();
    
    if (ADXT_LSC_GLOBALS->pre_callback) {
        ADXT_LSC_GLOBALS->pre_callback(ADXT_LSC_GLOBALS->pre_param);
    }
    
    LSC_ExecServer();
    
    if (ADXT_LSC_GLOBALS->post_callback) {
        ADXT_LSC_GLOBALS->post_callback(ADXT_LSC_GLOBALS->post_param);
    }
    
    ADXCRS_Leave();
}
