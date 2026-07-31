// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/mpv/mpv_err
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

extern u32 lbl_eu_80602A78[];

void MPVERR_Init(void) {}

void MPVERR_InitErrInf(void* self) {
    *(u32*)((u8*)self) = 0;
    *(u32*)((u8*)self + 4) = 0;
    *(u32*)((u8*)self + 8) = 0;
    *(u32*)((u8*)self + 0xc) = 0;
    *(u32*)((u8*)self + 0x10) = 0;
}

int MPVLIB_CheckHn(void* handle);

s32 MPV_SetErrFunc(void* self, void* cb, void* arg) {
    u32* dst;
    if (self != 0) {
        if (MPVLIB_CheckHn(self) != 0) {
            u32 oldcb = lbl_eu_80602A78[0];
            lbl_eu_80602A78[2] = 0xFF030203;
            if (oldcb != 0) {
                ((void (*)(u32))oldcb)(lbl_eu_80602A78[1]);
            }
            return 0xFF030203;
        }
        dst = (u32*)((u8*)self + 0xbdc);
    } else {
        dst = lbl_eu_80602A78;
    }
    dst[0] = (u32)cb;
    dst[1] = (u32)arg;
    return 0;
}

s32 MPVERR_SetCode(s32 val, u32 err_code) {
    if (val == 0) {
        lbl_eu_80602A78[2] = err_code;
        if (err_code != 0) {
            void (*cb)(u32) = (void (*)(u32))lbl_eu_80602A78[0];
            if (cb != NULL) {
                cb(lbl_eu_80602A78[1]);
            }
        }
    } else {
        *(u32*)((u8*)val + 0xbe4) = err_code;
        if (err_code != 0) {
            void (*cb)(u32) = *(void (**)(u32))((u8*)val + 0xbdc);
            if (cb != NULL) {
                cb(*(u32*)((u8*)val + 0xbe0));
            }
        }
    }
    return err_code;
}
