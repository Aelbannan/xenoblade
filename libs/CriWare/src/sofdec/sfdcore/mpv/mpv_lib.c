// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/mpv/mpv_lib
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void MPV_Init() {}

extern s32 MPVVLC_IsVlcSizErr(void);
extern s32 criware_803A59B0(void);
extern u32 lbl_eu_8051C29C[];

extern s32 MPVERR_SetCode(s32 val, u32 err_code);
extern s32 MPVDEC_CheckVersion(void* a, s32 b, s32 c);

s32 mpvlib_ChkFatal(void) {
    s32 endian;
    if (MPVVLC_IsVlcSizErr()) {
        return MPVERR_SetCode(0, 0xff03ff03);
    }
    if (MPVDEC_CheckVersion(lbl_eu_8051C29C, 0xdac, 0x80)) {
        return MPVERR_SetCode(0, 0xff03ff07);
    }
    if (criware_803A59B0()) {
        return MPVERR_SetCode(0, 0xff03ff09);
    }
    /* endianness self-test: on big-endian (target) the first stored byte is 1 */
    endian = 0x01020304;
    if (((u8*)&endian)[0] != 1) {
        /* illegal endianness — spin forever in an unusable ctr loop */
        for (;;) {
            ((void (*)(void))-1)();
        }
    }
    return 0;
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

void mpvlib_InitPicAtr(void* self) {
    memset(self, 0, 4);
    *(u32*)((u8*)self + 0x00) = 0;
    *(u32*)((u8*)self + 0x04) = 0;
    *(u32*)((u8*)self + 0x08) = 0;
    *(u32*)((u8*)self + 0x0c) = 0;
    *(u32*)((u8*)self + 0x10) = 0;
    *(u32*)((u8*)self + 0x14) = 0;
    *(u32*)((u8*)self + 0x18) = 0;
    *(u32*)((u8*)self + 0x1c) = 0;
    *(u32*)((u8*)self + 0x20) = 0;
    *(u32*)((u8*)self + 0x24) = 0;
    *(u32*)((u8*)self + 0x28) = 0;
    *(u32*)((u8*)self + 0x2c) = 0;
    *(u32*)((u8*)self + 0x30) = 0;
    *(u32*)((u8*)self + 0x34) = 0;
    *(u32*)((u8*)self + 0x38) = 3;
    *(u32*)((u8*)self + 0x3c) = 1;
    *(u32*)((u8*)self + 0x40) = 1;
    *(u32*)((u8*)self + 0x44) = 1;
    *(u32*)((u8*)self + 0x48) = 0;
    *(u32*)((u8*)self + 0x4c) = 0;
    *(s16*)((u8*)self + 0x50) = -1;
    *(s16*)((u8*)self + 0x52) = -1;
    *(u8*)((u8*)self + 0x54) = 0;
    *(s8*)((u8*)self + 0x55) = -1;
    *(s8*)((u8*)self + 0x56) = -1;
    *(s8*)((u8*)self + 0x57) = -1;
    *(u8*)((u8*)self + 0x58) = 0;
    *(u8*)((u8*)self + 0x59) = 1;
    *(u8*)((u8*)self + 0x5a) = 0;
    *(u8*)((u8*)self + 0x5b) = 0;
    *(u8*)((u8*)self + 0x5c) = 0;
    *(u8*)((u8*)self + 0x5d) = 0xFF;
    *(s8*)((u8*)self + 0x5e) = -1;
    *(s8*)((u8*)self + 0x5f) = -1;
    *(s8*)((u8*)self + 0x60) = -1;
    *(u8*)((u8*)self + 0x61) = 0;
    *(u8*)((u8*)self + 0x62) = 0xFF;
    *(u8*)((u8*)self + 0x63) = 0xFF;
    *(u8*)((u8*)self + 0x64) = 0xFF;
    *(u32*)((u8*)self + 0x68) = 0;
}

void MPVUMC_Finish(void);
void MPVSL_Finish(void);
void MPVM2V_Finish(void);

extern u32 lbl_eu_80602B88[];

void MPV_Finish(void) {
    MPVUMC_Finish();
    MPVSL_Finish();
    MPVM2V_Finish();
    // Retail hoists both loads (flag + base) before the branch and runs a
    // 223-iteration dcbi cache-flush countdown (mtlr/bdnz). __dcbi is not an
    // MWCC intrinsic in any version (KB: dcbi intrinsic gap) so the
    // instruction itself cannot be reproduced in high-level C; the call
    // shape below keeps everything else (prologue, loads, countdown)
    // aligned with retail.
    if (lbl_eu_80602B88[0x48 / 4] & 0x10000000) {
        u8* base = (u8*)lbl_eu_80602B88[0x50 / 4];
        for (u32 off = 0; off < 0x1BE0; off += 0x20) {
            __dcbi(base, off);
        }
    }
}

void* mpvlib_InitHn(void* self);

void* MPV_Create(void* pool) {
    u8* base = (u8*)lbl_eu_80602B88[0x58 / 4];
    s32 n = lbl_eu_80602B88[0x54 / 4];
    void* h = 0;
    s32 i;
    for (i = 0; i < n; i++) {
        if (*(s32*)(base + 0xb08) == 1) {
            h = base;
            break;
        }
        base += 0xdc0;
    }
    if (h != 0) {
        // Retail flushes the found block with an inline dcbi + ps_sel
        // countdown loop (mtctr 110; dcbi r3,r4; ps_sel f0,f3,f31,f4;
        // addi r4,32; bdnz) before the tail-call.  Inline dcbi is not an
        // MWCC intrinsic (KB ref 04331c483d; MPV_Finish same gap) and the
        // spec-encoded ps_sel needs a dc.l word inside an asm-void kernel,
        // so the high-level loop below keeps the semantics via per-line
        // __dcbi calls (documented ceiling; see MWCC_REFERENCE §8507).
        if (lbl_eu_80602B88[0x48 / 4] & 0x10000000) {
            u8* b = (u8*)h;
            u32 off;
            for (off = 0; off < 0x6e * 0x20; off += 0x20) {
                __dcbi(b, off);
            }
        }
        return mpvlib_InitHn(h);
    }
    return h;
}

extern u32 lbl_eu_8060464C;
extern u32 lbl_eu_80604650;
extern u32 lbl_eu_80604654;
extern u32 lbl_eu_80604658;
extern u32 lbl_eu_8060465C;
extern u32 lbl_eu_80604660;
extern u32 lbl_eu_80604664;

void MEM_Copy(void* d, const void* s, u32 n);
void MPVERR_InitErrInf(void* self);
void MPVCMC_InitObj(void* self);
void MPVSL_Create(void* self);
int MPVM2V_Create(void* self);
void MPV_SetUsrSj(void* self, u32 idx, u32 a, u32 b, u32 c);
void MPV_SetPicUsrBuf(void* self, void* a, void* b);

void* mpvlib_InitHn(void* self) {
    u8* b = (u8*)self;
    u32 base = lbl_eu_80602B88[0x50 / 4];
    s32 i;

    *(u32*)(b + 0x990) = lbl_eu_80604664;
    *(u32*)(b + 0x994) = lbl_eu_80604660 - 16;
    *(u32*)(b + 0x998) = lbl_eu_8060465C - 32;
    *(u32*)(b + 0x99C) = lbl_eu_80604658 - 32;
    *(u32*)(b + 0x9A0) = lbl_eu_80604654;
    *(u32*)(b + 0x9A4) = lbl_eu_80604650;
    *(u32*)(b + 0x9A8) = lbl_eu_8060464C;
    *(u32*)(b + 0x9B8) = base + 0x11E0;
    *(u32*)(b + 0x9BC) = base + 0x1200;
    *(u32*)(b + 0x9B0) = base + 0x1100;
    {
        u32 clip = lbl_eu_80602FE8;
        *(u32*)(b + 0x9C0) = clip;
        *(u32*)(b + 0xA90) = clip;
    }
    *(u32*)(b + 0xA94) = (u32)self;
    *(u32*)(b + 0xA98) = (u32)self + 0x380;
    *(u32*)(b + 0xA9C) = (u32)self + 0x500;
    *(u32*)(b + 0xD0C) = (u32)self + 0x100;
    *(u32*)(b + 0xD10) = (u32)self + 0x180;
    *(u32*)(b + 0xD14) = (u32)self + 0x200;
    *(u32*)(b + 0xD18) = (u32)self + 0x280;
    *(u32*)(b + 0xD1C) = (u32)self;
    *(u32*)(b + 0xD20) = (u32)self + 0x80;
    *(u32*)(b + 0xB0C) = 0;
    MEM_Copy(b + 0xB10, (u8*)lbl_eu_80602B88, 64);
    *(u32*)(b + 0xB54) = 0;
    *(u32*)(b + 0xB58) = 0;
    *(u32*)(b + 0xB50) = 0x7FFFFFFF;
    MPVERR_InitErrInf(b + 0xBDC);
    MPVCMC_InitObj(self);
    mpvlib_InitPicAtr(b + 0xB5C);
    *(u32*)(b + 0xD24) = 0;
    *(u32*)(b + 0xD28) = 0;
    *(u32*)(b + 0xCFC) = 0;
    *(u32*)(b + 0xD00) = 0;
    *(u32*)(b + 0xD04) = 0;
    *(u32*)(b + 0xD08) = 0;
    *(u32*)(b + 0xD38) = 0;
    for (i = 0; i < 4; i++) {
        MPV_SetUsrSj(self, (u32)i, 0, 0, 0);
    }
    MPV_SetPicUsrBuf(self, 0, 0);
    *(u32*)(b + 0xDA8) = 0;
    MPVSL_Create(self);
    *(u32*)(b + 0xD54) = (u32)MPVM2V_Create(self);
    *(u32*)(b + 0xB08) = 2;
    return self;
}

void MPV_GetDctCnt(void* self, u32* out1, u32* out2) {
    *out1 = *(u32*)((u8*)self + 0xa10);
    *out2 = *(u32*)((u8*)self + 0xa14);
}

extern void MPVM2V_Destroy(void* self);
extern void MPVSL_Destroy(void* self);
extern u32 lbl_eu_80602FEC;
extern u32 lbl_eu_80602B88[];

s32 MPV_Destroy(void* self) {
    s32 err;
    /* validate handle: NULL or state != 2(active) is an error; on success
       (state == 2) publish the handle to the globals before tearing down. */
    if (self == NULL) {
        err = -1;
    } else if (*(s32*)((u8*)self + 0xb08) != 2) {
        err = -1;
    } else {
        lbl_eu_80602FEC = (u32)self;
        err = 0;
    }
    if (err != 0) {
        return MPVERR_SetCode(0, 0xff030201);
    }
    MPVM2V_Destroy(self);
    MPVSL_Destroy(self);
    if (lbl_eu_80602B88[0x48/4] & 0x10000000) {
        s32 i;
        for (i = 0; i < 0x6e; i++) {
            __dcbi(self, i * 0x20);
        }
    }
    *(s32*)((u8*)self + 0xb08) = 1;
    return 0;
}

extern u32 lbl_eu_80602FEC;
void MPVM2V_SetCond(void* mpv);
s32 MPVERR_SetCode(s32 val, u32 err_code);

s32 MPV_SetCond(void* mpv, s32 cond, s32 val) {
    u8* tbl;
    if (mpv == NULL) {
        u32 c4 = cond * 4;
        u8* base = (u8*)*(u32*)((u8*)lbl_eu_80602B88 + 0x58);
        s32 n = *(s32*)((u8*)lbl_eu_80602B88 + 0x54);
        u8* p;
        tbl = base + c4;
        p = base;
        while (n-- > 0) {
            if (*(s32*)(p + 0xb08) == 2) {
                *(u32*)(tbl + 0xb10) = val;
            }
            p += 0xdc0;
            tbl += 0xdc0;
        }
        tbl = (u8*)lbl_eu_80602B88;
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
        tbl = (u8*)mpv + 0xb10;
    }
    *(u32*)(tbl + 4 * cond) = val;
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
