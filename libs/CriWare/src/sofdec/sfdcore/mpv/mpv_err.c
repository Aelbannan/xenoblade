// CriWare Sofdec MPEG video error-handler registration (mpv_err.c).
#include <harness_catalog.h>

typedef void (*MPVErrCallback)(u32);

int MPVLIB_CheckHn(void* handle);

// Error-handler registration block embedded in the MPV decoder state
// (and mirrored globally when no handle is given).
typedef struct MPVErrInf {
    MPVErrCallback func;  // 0xBDC: user error callback
    u32 user;             // 0xBE0: opaque argument passed to the callback
    u32 code;             // 0xBE4: last error code
    u32 field_0xC;
    u32 field_0x10;
} MPVErrInf;

// Global error-registration block used when no MPV handle is supplied
// (JP symbol mpverrinf, .bss size 0x14). Its retail .bss slot lies inside the
// mpv_vlc .bss range, so its defining TU is still unresolved; reference it only.
extern MPVErrInf mpverrinf;

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

// Register (or clear, with cb == NULL) the user error callback either on a
// decoder handle or, when the handle is NULL, in the global block.
// The callback/argument pair lands at errInf+0/+4; an invalid handle is
// rejected by MPVLIB_CheckHn and reported as fatal code 0xFF030203 through
// the inline global dispatch below.
s32 MPV_SetErrFunc(SFD_MPV* mpv, MPVErrCallback cb, u8* arg) {
    SFD_MPV* m = (SFD_MPV*)(void*)mpv;
    MPVErrInf* dst;
    if (m == NULL) {
        dst = &mpverrinf;
    } else if (MPVLIB_CheckHn(m)) {
        /* invalid handle: record the fatal code and dispatch it through the
         * previously registered global callback */
        mpverrinf.code = 0xFF030203;
        if (mpverrinf.func != NULL) {
            mpverrinf.func(mpverrinf.user);
        }
        return 0xFF030203;
    } else {
        dst = &m->errInf;
    }
    dst->func = cb;
    dst->user = (u32)arg;
    return 0;
}

// Record an error code and dispatch it through the registered callback.
// A NULL handle selects the global registration block instead.
s32 MPVERR_SetCode(s32 val, u32 err_code) {
    SFD_MPV* mpv = (SFD_MPV*)val;
    if (mpv == NULL) {
        mpverrinf.code = err_code;
        if (err_code != 0) {
            MPVErrCallback cb = mpverrinf.func;
            if (cb != NULL) {
                cb(mpverrinf.user);
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
