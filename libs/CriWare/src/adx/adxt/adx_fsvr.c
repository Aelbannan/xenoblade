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

// The six independent globals of the ADX file server (JP symbol-map names).
s32 adxt_fssvr_enter_cnt = 0;
s32 adxt_fsst_enter_cnt = 0;
void (*adxt_fs_svr_start_cb)(struct ADXT_FsSvrCallbackParam*);
struct ADXT_FsSvrCallbackParam* adxt_fs_svr_start_cb_obj;
void (*adxt_fs_svr_end_cb)(struct ADXT_FsSvrCallbackParam*);
struct ADXT_FsSvrCallbackParam* adxt_fs_svr_end_cb_obj;

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
    ADXCRS_Lock();
    if (adxt_fssvr_enter_cnt != 0) {
        ADXCRS_Unlock();
        return;
    }
    adxt_fssvr_enter_cnt = 1;
    ADXCRS_Unlock();

    if (adxt_fs_svr_start_cb) {
        adxt_fs_svr_start_cb(adxt_fs_svr_start_cb_obj);
    }

    adxt_fssvr_enter_cnt = 3;
    ADXSTM_ExecFsSvr();
    adxt_fssvr_enter_cnt = 4;
    ADXSTM_ExecServer();
    adxt_fssvr_enter_cnt = 5;
    ADXF_ExecServer();
    adxt_fssvr_enter_cnt = 6;
    ADXSTM_ExecServer();
    adxt_fssvr_enter_cnt = 7;
    ADXSTM_ExecFsSvr();
    adxt_fssvr_enter_cnt = 9;
    ADXSTM_ExecFsIdle();

    adxt_fssvr_enter_cnt = 0;
    if (adxt_fs_svr_end_cb) {
        adxt_fs_svr_end_cb(adxt_fs_svr_end_cb_obj);
    }
}
