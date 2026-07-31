// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/sfd/sfd_lib
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

/* Forward declarations for external functions */
extern void UTY_InitTmr(void);
extern void SJRBF_Init(void);
extern void SFPLY_Init(void);
extern void SFHDS_Init(void);
extern void criware_803BFD20(void);
extern void UTY_MemsetDword(void* dst, u32 val, u32 count);
extern void MEM_Copy(void* dst, void* src, u32 size);
extern s32 SFTIM_Init(void* ctx, u32 param);
extern void SFBUF_Init(void* ctx);
extern s32 SFTRN_Init(void* ctx, u32 param);
extern u32 lbl_eu_8051C590[];
extern u32 lbl_eu_8051CA10[];
extern u32 lbl_eu_80567ED8[];
extern u32 lbl_eu_80606E20[];
extern u32 lbl_eu_80606E38[];

s32 SFLIB_SetErr(s32 val, u32 err_code);
s32 SFLIB_CheckHn(void* h);
s32 fn_803C3400(void* cfg);

u32 SFD_IsVersionCompatible(void* self, u32 version) {
    return (version == 0x39b0) ? 1 : 0;
}

s32 fn_803C3320(void* cfg) {
    s32 result;
    void (*fn)(void*, void*);
    u32* ctx = lbl_eu_80606E20;
    u32* evt = lbl_eu_80567ED8;

    evt[3] = (u32)cfg + 4;

    if (ctx[5] != 0) {
        fn = *(void (**)(void*, void*))((u8*)(u32)ctx[5] + 0x24);
        fn((void*)ctx[5], &evt[1]);
    }

    ctx[0xCD] = 0x39B0;
    ctx[0xCE] = (u32)lbl_eu_8051C590;

    UTY_InitTmr();
    SJRBF_Init();

    result = fn_803C3400(cfg);
    if (result == 0) {
        SFPLY_Init();
        SFHDS_Init();
        criware_803BFD20();
    }

    if (ctx[5] != 0) {
        evt[0x1D] = (u32)&result;
        fn = *(void (**)(void*, void*))((u8*)(u32)ctx[5] + 0x24);
        fn((void*)ctx[5], &evt[0x1B]);
    }

    return result;
}

s32 fn_803C3400(void* cfg) {
    s32 error = 0;
    u32* ctx = lbl_eu_80606E38;
    u32 tmp;

    UTY_MemsetDword(ctx, 0, 0xC7);
    MEM_Copy(ctx, lbl_eu_8051CA10, 0x190);
    tmp = *(u32*)cfg;
    ctx[0x64] = tmp;
    ctx[0x65] = *(u32*)((u8*)cfg + 4);
    ctx[0x66] = 0;
    ctx[0x7E] = 0x5A5A5A5A;

    SFTIM_Init((u8*)ctx + 0x19C, 1);
    SFBUF_Init((u8*)ctx + 0x1B0);

    ctx[0x7C] = 0;
    ctx[0x7D] = 0;
    ctx[0x7F] = 0;
    ctx[0x80] = 0;
    ctx[0x81] = 0;
    ctx[0x82] = 0;
    ctx[0x83] = 0;
    ctx[0x84] = 0;
    ctx[0x85] = 0;

    s32 trnResult = SFTRN_Init((u8*)ctx + 0x1B4, tmp);
    if (trnResult != 0) {
        error = trnResult;
    }
    return error;
}

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

s32 criware_803C0D94(void* handle, void (*errFn)(u32, u32), u32 errArg) {
    if (handle == NULL) {
        lbl_eu_80606E20[0] = (u32)errFn;
        lbl_eu_80606E20[1] = errArg;
        return 0;
    }
    if (*(s32*)((u8*)handle + 0x54) != 0) {
        *(void (**)(u32, u32))((u8*)handle + 0xA08) = errFn;
        *(u32*)((u8*)handle + 0xA0C) = errArg;
        return 0;
    }
    return SFLIB_SetErr(0, 0xFF000101);
}

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
