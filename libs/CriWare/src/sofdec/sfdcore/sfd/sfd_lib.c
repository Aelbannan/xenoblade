// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/sfd/sfd_lib
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

u32 SFD_IsVersionCompatible(void* self, u32 version) {
    return (version == 0x39b0) ? 1 : 0;
}

void fn_803C3320() {}

void fn_803C3400() {}

typedef struct SfdErrorState {
    u32 field00;
    u32 field04;
    u32 field08;
    u32 field0C;
    u32 field10;
} SfdErrorState;

void fn_803C34DC(void* self) {
    SfdErrorState* state = (SfdErrorState*)self;
    state->field00 = 0;
    state->field04 = 0;
    state->field08 = 0;
    state->field0C = 0;
    state->field10 = 0;
}

s32 SFLIB_SetErr(s32 val, u32 err_code);
s32 fn_803C34F8(s32 val, u32 err_code) {
    return SFLIB_SetErr(val, err_code);
}

extern void* lbl_eu_80606E34;
extern u32 lbl_eu_80568720[];
extern u32 lbl_eu_80606E20[];
s32 SFLIB_SetErr(s32 val, u32 err_code) {
    void (*fn)(void*, void*);

    if (err_code == 0) {
        return 0;
    }

    if (lbl_eu_80606E34 != NULL) {
        lbl_eu_80568720[3] = val;
        lbl_eu_80568720[6] = (u32)&err_code;
        fn = *(void (**)(void*, void*))((u8*)*(u32*)lbl_eu_80606E34 + 0x24);
        fn(lbl_eu_80606E34, &lbl_eu_80568720[1]);
    }

    if (val == 0) {
        u32 err = err_code;
        if (lbl_eu_80606E20[2] == 0) {
            lbl_eu_80606E20[2] = err;
        }
        if (err != 0) {
            void (*cb)(u32, u32) = (void (*)(u32, u32))lbl_eu_80606E20[0];
            if (cb != NULL) {
                cb(lbl_eu_80606E20[1], err);
            }
        }
    } else {
        typedef void (*SfdErrorCallback)(u32, u32);
        typedef struct SfdHandle {
            u8 _00[0x54];
            s32 errorState;
            u8 _58[0x9B0];
            SfdErrorCallback errorCallback;
            u32 errorCallbackArg;
            u32 lastError;
        } SfdHandle;
        SfdHandle* handle = (SfdHandle*)val;
        u32 last = handle->lastError;
        u32 err = err_code;
        if (last == 0) {
            handle->lastError = err;
        }
        if (err != 0 && handle->errorCallback != NULL) {
            handle->errorCallback(handle->errorCallbackArg, err);
        }
        if (handle->errorState > 0) {
            handle->errorState = -handle->errorState;
        }
    }

    if (lbl_eu_80606E34 != NULL) {
        fn = *(void (**)(void*, void*))((u8*)*(u32*)lbl_eu_80606E34 + 0x24);
        fn(lbl_eu_80606E34, &lbl_eu_80568720[27]);
    }

    return err_code;
}

void criware_803C0D94() {}

extern void* lbl_eu_8060715C;
s32 SFLIB_CheckHn(void* h) {
    typedef struct SfdHandleHeader {
        u8 _00[0x54];
        s32 errorState;
    } SfdHandleHeader;
    if (h == NULL) return -1;
    if (((SfdHandleHeader*)h)->errorState == 0) return -1;
    lbl_eu_8060715C = h;
    return 0;
}

void SVM_Lock(void);
void SFLIB_LockCs(void* cs) {
    SVM_Lock();
}

void SVM_Unlock(void);
void SFLIB_UnlockCs(void* cs) {
    SVM_Unlock();
}
