// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/sfd/sfd_lib
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

u32 SFD_IsVersionCompatible(void* self, u32 version) {
    return (version == 0x39b0) ? 1 : 0;
}

void fn_803C3320() {}

void fn_803C3400() {}

void fn_803C34DC(void* self) {
    *(u32*)((u8*)self) = 0;
    *(u32*)((u8*)self + 4) = 0;
    *(u32*)((u8*)self + 8) = 0;
    *(u32*)((u8*)self + 0xc) = 0;
    *(u32*)((u8*)self + 0x10) = 0;
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
        u8* h = (u8*)val;
        u32 last = *(u32*)(h + 0xa10);
        u32 err = err_code;
        if (last == 0) {
            *(u32*)(h + 0xa10) = err;
        }
        if (err != 0) {
            void (*cb)(u32, u32) = *(void (**)(u32, u32))(h + 0xa08);
            if (cb != NULL) {
                cb(*(u32*)(h + 0xa0c), err);
            }
        }
        if (*(s32*)(h + 0x54) > 0) {
            *(s32*)(h + 0x54) = -*(s32*)(h + 0x54);
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
    if (h == NULL) return -1;
    if (*(s32*)((u8*)h + 0x54) == 0) return -1;
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
