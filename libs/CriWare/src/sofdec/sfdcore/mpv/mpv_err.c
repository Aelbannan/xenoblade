// CriWare Sofdec MPEG video error-handler registration (mpv_err.c).
#include <harness_catalog.h>

extern u32 lbl_eu_80602A78[];

typedef void (*MPVErrCallback)(u32);

int MPVLIB_CheckHn(void* handle);

// Error-handler registration block embedded in the MPV decoder state
// (and mirrored globally at lbl_eu_80602A78 when no handle is given).
typedef struct MPVErrInf {
    MPVErrCallback func;  // 0xBDC: user error callback
    u32 user;             // 0xBE0: opaque argument passed to the callback
    u32 code;             // 0xBE4: last error code
    u32 field_0xC;
    u32 field_0x10;
} MPVErrInf;

typedef struct SFD_MPV {
    u8 field_0x0[0xbdc];
    MPVErrInf errInf;
} SFD_MPV;

// Reset module-global decoder state hook (no-op in retail).
void MPVERR_Init(void) {}

// Clear the whole error-registration area (callback, argument, code).
void MPVERR_InitErrInf(MPVErrInf* inf) {
    inf->func = NULL;
    inf->user = 0;
    inf->code = 0;
    inf->field_0xC = 0;
    inf->field_0x10 = 0;
}

// Record an error code and dispatch it through the registered callback.
// A NULL handle selects the global registration block instead.
s32 MPVERR_SetCode(s32 val, u32 err_code) {
    SFD_MPV* mpv = (SFD_MPV*)val;
    if (mpv == NULL) {
        lbl_eu_80602A78[2] = err_code;
        if (err_code != 0) {
            MPVErrCallback cb = (MPVErrCallback)lbl_eu_80602A78[0];
            if (cb != NULL) {
                cb(lbl_eu_80602A78[1]);
            }
        }
    } else {
        mpv->errInf.code = err_code;
        if (err_code != 0) {
            MPVErrCallback cb = mpv->errInf.func;
            if (cb != NULL) {
                cb(mpv->errInf.user);
            }
        }
    }
    return err_code;
}

// Register (or clear, with cb == NULL) the user error callback either on a
// decoder handle or, when the handle is NULL, in the global block.
// Inline twin of MPVERR_SetCode used by MPV_SetErrFunc; inlining it is what
// produces the retail code shape of the invalid-handle path.
static __inline__ s32 MPVERR_SetCode_i(s32 val, u32 err_code) {
    SFD_MPV* mpv = (SFD_MPV*)val;
    if (mpv == NULL) {
        /* callback loaded before the code store: fixes the temp order */
        MPVErrCallback gcb = (MPVErrCallback)lbl_eu_80602A78[0];
        lbl_eu_80602A78[2] = err_code;
        if (err_code != 0 && gcb != NULL) {
            gcb(lbl_eu_80602A78[1]);
        }
    } else {
        mpv->errInf.code = err_code;
        if (err_code != 0) {
            MPVErrCallback cb = mpv->errInf.func;
            if (cb != NULL) {
                cb(mpv->errInf.user);
            }
        }
    }
    return err_code;
}

// Register (or clear, with cb == NULL) the user error callback either on a
// decoder handle or, when the handle is NULL, in the global block.
// The callback/argument pair lands at errInf+0/+4; an invalid handle is
// rejected by MPVLIB_CheckHn and reported as fatal code 0xFF030203 through
// the inline twin below.
s32 MPV_SetErrFunc(SFD_MPV* mpv, MPVErrCallback cb, u8* arg) {
    u32* dst;
    if (mpv == NULL) {
        dst = lbl_eu_80602A78;
    } else if (MPVLIB_CheckHn(mpv)) {
        /* invalid handle: record the fatal code and dispatch it through the
         * previously registered global callback */
        return MPVERR_SetCode_i(0, 0xFF030203);
    } else {
        dst = (u32*)&mpv->errInf;
    }
    dst[0] = (u32)cb;
    dst[1] = (u32)arg;
    return 0;
}
