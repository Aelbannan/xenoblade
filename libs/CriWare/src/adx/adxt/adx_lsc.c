// Auto-scaffolded catalog TU for CriWare/src/adx/adxt/adx_lsc
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

typedef struct ADXT_LscCallbackParam {
    // Will be populated when callback types are known
} ADXT_LscCallbackParam;

typedef struct {
    void (*pre_callback)(ADXT_LscCallbackParam*);
    ADXT_LscCallbackParam* pre_param;
    void (*post_callback)(ADXT_LscCallbackParam*);
    ADXT_LscCallbackParam* post_param;
} ADXT_LscCallbacks;

static ADXT_LscCallbacks s_lsc_callbacks = {0};

void ADXT_ExecLscSvr() {
    ADXCRS_Enter();
    
    if (s_lsc_callbacks.pre_callback) {
        s_lsc_callbacks.pre_callback(s_lsc_callbacks.pre_param);
    }
    
    LSC_ExecServer();
    
    if (s_lsc_callbacks.post_callback) {
        s_lsc_callbacks.post_callback(s_lsc_callbacks.post_param);
    }
    
    ADXCRS_Leave();
}
