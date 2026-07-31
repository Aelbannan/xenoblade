// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/mpv/mpv_lib
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void MPV_Init() {}

void mpvlib_ChkFatal() {}

extern u8 lbl_eu_80602BE8[];
extern u32 lbl_eu_80602FE8;

void mpvlib_InitClip0255() {}

void mpvlib_InitPicAtr() {}

void MPVUMC_Finish(void);
void MPVSL_Finish(void);
void MPVM2V_Finish(void);

extern u32 lbl_eu_80602B88[];

void MPV_Finish(void) {
    MPVUMC_Finish();
    MPVSL_Finish();
    MPVM2V_Finish();
    if (lbl_eu_80602B88[0x48/4] & 0x10000000) {
        u32 base = lbl_eu_80602B88[0x50/4];
        u32 i;
        for (i = 0; i < 0xDF; i++) {
            __dcbi((void*)(base + i * 0x20));
        }
    }
}

void MPV_Create() {}

void mpvlib_InitHn() {}

void MPV_GetDctCnt(void* self, u32* out1, u32* out2) {
    *out1 = *(u32*)((u8*)self + 0xa10);
    *out2 = *(u32*)((u8*)self + 0xa14);
}

void MPV_Destroy() {}

extern u32 lbl_eu_80602FEC;
void MPVM2V_SetCond(void* mpv);
s32 MPVERR_SetCode(s32 val, u32 err_code);

s32 MPV_SetCond(void* mpv, s32 cond, s32 val) {
    u32* tbl;
    if (mpv == NULL) {
        u32* base = *(u32**)((u8*)lbl_eu_80602B88 + 0x58);
        s32 n = *(s32*)((u8*)lbl_eu_80602B88 + 0x54);
        tbl = base + cond;
        while (n-- > 0) {
            if (*(s32*)((u8*)base + 0xb08) == 2) {
                *(u32*)((u8*)tbl + 0xb10) = val;
            }
            tbl = (u32*)((u8*)tbl + 0xdc0);
            base = (u32*)((u8*)base + 0xdc0);
        }
        tbl = (u32*)lbl_eu_80602B88;
    } else {
        s32 err;
        if (mpv == NULL) {
            err = -1;
        } else if (*(s32*)((u8*)mpv + 0xb08) != 2) {
            err = -1;
        } else {
            lbl_eu_80602FEC = (u32)mpv;
            err = 0;
        }
        if (err != 0) {
            return MPVERR_SetCode(0, 0xff030202);
        }
        tbl = (u32*)((u8*)mpv + 0xb10);
    }
    tbl[cond] = val;
    MPVM2V_SetCond(mpv);
    return 0;
}

s32 MPV_GetCond(void* mpv, s32 cond, s32* out) {
    u32* tbl;
    if (mpv == NULL) {
        tbl = (u32*)lbl_eu_80602B88;
    } else {
        s32 err;
        if (mpv == NULL) {
            err = -1;
        } else if (*(s32*)((u8*)mpv + 0xb08) != 2) {
            err = -1;
        } else {
            lbl_eu_80602FEC = (u32)mpv;
            err = 0;
        }
        if (err != 0) {
            return MPVERR_SetCode(0, 0xff030210);
        }
        tbl = (u32*)((u8*)mpv + 0xb10);
    }
    *out = tbl[cond];
    return 0;
}

void MPVM2V_SetMbCb(void* self, void* a, void* b, void* c);
void MPV_SetMbCb(void* self, void* a, void* b, void* c) {
    *(void**)((u8*)self + 0xb50) = a;
    *(void**)((u8*)self + 0xb58) = c;
    *(void**)((u8*)self + 0xb54) = b;
    MPVM2V_SetMbCb(self, a, b, c);
}

extern u32 lbl_eu_80602FEC;

int MPVLIB_CheckHn(void* handle) {
    if (handle == NULL) return -1;
    if (*(int*)((u8*)handle + 0xb08) != 2) return -1;
    lbl_eu_80602FEC = (u32)handle;
    return 0;
}
