// Auto-scaffolded catalog TU for CriWare/src/adx/adxt/adx_fsvr
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

// Forward declarations for external functions
void ADXCRS_Enter(void);
void ADXCRS_Leave(void);
void ADXCRS_Lock(void);
void ADXCRS_Unlock(void);
void ADXSTM_ExecFsSvr(void);
void ADXSTM_ExecServer(void);
void ADXSTM_ExecFsIdle(void);
void ADXF_ExecServer(void);
void ADXT_ExecFsSvr(void);
void adxt_ExecFsSvr(void);

// Opaque callback parameter type (layout unknown)
struct ADXT_FsSvrCallbackParam {
    int placeholder;
};

// Global state for the ADX file server, stored at lbl_eu_805E26B0.
// Layout recovered from adxt_ExecFsSvr: state machine at 0x0, pre/post
// callback function pointers and their arguments follow at 0x8-0x14.
struct ADXT_FsSvrGlobals {
    s32 state;                                            // 0x0
    s32 field_0x4;                                        // 0x4
    void (*pre_callback)(struct ADXT_FsSvrCallbackParam*); // 0x8
    struct ADXT_FsSvrCallbackParam* pre_arg;              // 0xC
    void (*post_callback)(struct ADXT_FsSvrCallbackParam*); // 0x10
    struct ADXT_FsSvrCallbackParam* post_arg;             // 0x14
};

// Declare the global struct at the retail address so the compiler emits
// @ha/@l relocations matching the retail lis/addi sequence.
extern struct ADXT_FsSvrGlobals lbl_eu_805E26B0;

// Wrapper that enters/leaves the ADX critical section around adxt_ExecFsSvr.
void ADXT_ExecFsSvr() {
    ADXCRS_Enter();
    adxt_ExecFsSvr();
    ADXCRS_Leave();
}

// Internal ADX file server state machine.
// Guards against re-entry via the state field, invokes an optional pre-callback,
// sequences through ADX subsystem servers (STM, F) with state updates, then
// invokes an optional post-callback and resets state to 0.
void adxt_ExecFsSvr() {
    struct ADXT_FsSvrGlobals* globals = &lbl_eu_805E26B0;

    ADXCRS_Lock();
    if (globals->state != 0) {
        ADXCRS_Unlock();
        return;
    }
    globals->state = 1;
    ADXCRS_Unlock();

    if (globals->pre_callback) {
        globals->pre_callback(globals->pre_arg);
    }

    globals->state = 3;
    ADXSTM_ExecFsSvr();
    globals->state = 4;
    ADXSTM_ExecServer();
    globals->state = 5;
    ADXF_ExecServer();
    globals->state = 6;
    ADXSTM_ExecServer();
    globals->state = 7;
    ADXSTM_ExecFsSvr();
    globals->state = 9;
    ADXSTM_ExecFsIdle();

    globals->state = 0;
    if (globals->post_callback) {
        globals->post_callback(globals->post_arg);
    }
}
