// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/mpv/mpv_lib
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void MPV_Init() {}

extern s32 MPVVLC_IsVlcSizErr(void);
extern s32 criware_803A59B0(void);
extern u32 lbl_eu_8051C29C[];

extern s32 MPVERR_SetCode(s32 val, u32 err_code);
extern s32 MPVDEC_CheckVersion(void* a, s32 b, s32 c);

void mpvlib_ChkFatal(void) {
    if (MPVVLC_IsVlcSizErr()) {
        MPVERR_SetCode(0, 0xFF04FF03);
        return;
    }
    if (MPVDEC_CheckVersion(lbl_eu_8051C29C, 0xdac, 0x80) == 0) {
        MPVERR_SetCode(0, 0xFF04FF07);
        return;
    }
    if (criware_803A59B0() == 0) {
        MPVERR_SetCode(0, 0xFF04FF09);
        return;
    }
    {
        u32 x = 0x01020304;
        for (;;) {
            if ((u8)x == 1)
                break;
            ((void (*)(void))-1)();
        }
    }
}

extern u8 lbl_eu_80602BE8[];
extern u32 lbl_eu_80602FE8;

void mpvlib_InitClip0255(void) {
    u8* q = lbl_eu_80602BE8;
    int i;
    for (i = 0; i < 0x180; i++) {
        *q++ = 0;
    }
    for (i = 0; i < 0x100; i++) {
        *q++ = (u8)i;
    }
    for (i = 0; i < 0x180; i++) {
        *q++ = 0xFF;
    }
    lbl_eu_80602FE8 = (u32)(lbl_eu_80602BE8 + 0x180);
}

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

extern void MPVM2V_Destroy(void* self);
extern void MPVSL_Destroy(void* self);
extern u32 lbl_eu_80602FEC;
extern u32 lbl_eu_80602B88[];

s32 MPV_Destroy(void* self) {
    if (self == NULL)
        return -1;
    if (*(s32*)((u8*)self + 0xb08) == 2)
        return -1;
    lbl_eu_80602FEC = 0;
    MPVM2V_Destroy(self);
    MPVSL_Destroy(self);
    if (lbl_eu_80602B88[0x48/4] & 0x10000000) {
        s32 i;
        for (i = 0; i < 0x6e; i++) {
            __dcbi((void*)((u8*)self + i * 0x20));
        }
    }
    *(s32*)((u8*)self + 0xb08) = 1;
    return 0;
}

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
