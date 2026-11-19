// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/mpv/mpv_lib
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

#include "libs/CriWare/src/sofdec/sfdcore/mpv/mpv_types.h"

// Large section arrays for data diff - generated
__declspec(section ".rodata") __attribute__((aligned(8))) const unsigned char __large_rodata_for_CriWare_src_sofdec_sfdcore_mpv_[0xA8] = {0x0A, 0x43, 0x52, 0x49, 0x20, 0x4D, 0x50, 0x56, 0x2F, 0x57, 0x49, 0x49, 0x20, 0x56, 0x65, 0x72, 0x2E, 0x32, 0x2E, 0x30, 0x36, 0x37, 0x20, 0x42, 0x75, 0x69, 0x6C, 0x64, 0x3A, 0x4F, 0x63, 0x74, 0x20, 0x32, 0x38, 0x20, 0x32, 0x30, 0x30, 0x39, 0x20, 0x31, 0x31, 0x3A, 0x33, 0x37, 0x3A, 0x35, 0x39, 0x0A, 0x00, 0x41, 0x70, 0x70, 0x65, 0x6E, 0x64, 0x3A, 0x20, 0x4D, 0x57, 0x34, 0x33, 0x30, 0x32, 0x20, 0x57, 0x49, 0x49, 0x33, 0x30, 0x4A, 0x75, 0x6C, 0x32, 0x30, 0x30, 0x38, 0x50, 0x61, 0x74, 0x63, 0x68, 0x30, 0x34, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x5A, 0x5A, 0x5A, 0x5A, 0x32, 0x2E, 0x30, 0x36, 0x37, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; // .rodata 0xA8 at 0x8051C200

s32 mpvlib_ChkFatal(void);
void mpvlib_InitClip0255(void);

/* Global Sofdec work area at 0x80602B80 (8-byte header + 0x60-byte config +
 * 0x400-byte clip table).  The config block is also referenced through its own
 * symbol, exactly as retail does. */
extern MpvWork lbl_eu_80602B80;
extern MpvCfg lbl_eu_80602B88;
extern u8 lbl_eu_8051C200[];
extern u8 lbl_eu_8051C258[];

extern void MPVERR_Init(void);
extern void MPVHDEC_Init(void);
extern void MPVFRM_Init(void);
extern void MPVSL_Init(void);
extern void MPVVLC_Init(void* a, void* b);
extern void MPVBDEC_Init(void* handle);
extern void MPVUMC_Init(void);
extern void MPVCDEC_Init(void* self);
extern void MPVM2V_Init(void);
extern void UTY_MemsetDword(u32* dst, u32 val, u32 n);
extern void UTY_MemcpyDword(u32* dst, const u32* src, u32 n);
void MEM_Copy(void* d, const void* s, u32 n);

extern void MPVM2V_Destroy(void* self);
extern void MPVSL_Destroy(void* self);
extern s32 MPVERR_SetCode(s32 val, u32 err_code);

s32 MPV_Init(s32 a, s32 b) {
    MpvWork* work = &lbl_eu_80602B80;
    s32 err;
    u32 aligned;
    u8* blk;
    u8* vlc;
    s32 i;

    work->verPtr = (u32)lbl_eu_8051C200;
    err = mpvlib_ChkFatal();
    if (err != 0) {
        /* fatal: the version-mismatch code (0xff03ff05) returns the fatal
         * code unchanged either way */
        if ((u32)(err + 0xfd0000) == 0xff05) {
            return err;
        }
        return err;
    }

    aligned = (u32)(b + 0x1f) & ~0x1f;
    UTY_MemsetDword((u32*)aligned, 0, (u32)((a << 13) + 0x2000) >> 2);
    blk = (u8*)aligned + a * 0xdc0;
    vlc = blk + 0x420;
    if (work->cfg.flushSw & 0x10000000) {
        /* Retail cache-invalidates + dcbz_l's 0xdf blocks (dcbi/dcbz_l
         * pair); neither instruction is emittable in GC/3.0a5.2
         * high-level C (KB dcbi gap), so two __dcbz keep the
         * countdown-loop shape; both cache opcodes differ (documented
         * ceiling, MWCC_CASES MPV_Finish). */
        for (i = 0; i < 0xdf; i++) {
            __dcbz(vlc, i * 0x20);
            __dcbz(vlc, i * 0x20);
        }
    }
    MEM_Copy(&work->cfg, lbl_eu_8051C258, 0x40);
    work->cfg.hnBase = (u32)blk;
    work->cfg.vlcBase = (u32)vlc;
    work->cfg.hnNum = (u32)a;
    work->cfg.memBase = aligned;
    MPVERR_Init();
    MPVHDEC_Init();
    MPVFRM_Init();
    MPVSL_Init();
    MPVVLC_Init(vlc + 0x1230, vlc);
    MPVBDEC_Init(vlc);
    MPVUMC_Init();
    MPVCDEC_Init(vlc);
    mpvlib_InitClip0255();
    {
        u8* clip;
        if ((clip = vlc + 0x17e0) != NULL) {
            UTY_MemcpyDword((u32*)clip, (u32*)work->clipTbl, 0x100);
            work->clipBuf = (u32)(vlc + 0x1960);
        }
    }
    {
        s32 n = work->cfg.hnNum;
        MpvHn* hn = (MpvHn*)work->cfg.memBase;
        while (n-- > 0) {
            hn->state = MPV_HN_STATE_FREE;
            hn++;
        }
    }
    MPVM2V_Init();
    return 0;
}

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
        /* illegal endianness -- spin forever in an unusable ctr loop */
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

void mpvlib_InitPicAtr(SfdMpvPicAtr* self) {
    memset(self, 0, 4);
    self->width = 0;
    self->height = 0;
    self->field_0x08 = 0;
    self->field_0x0c = 0;
    self->rateIndex = 0;
    self->time = 0;
    self->type = 0;
    self->field_0x1c = 0;
    self->field_0x20 = 0;
    self->field_0x24 = 0;
    self->field_0x28 = 0;
    self->field_0x2c = 0;
    self->field_0x30 = 0;
    self->field_0x34 = 0;
    self->field_0x38 = 3;
    self->field_0x3c = 1;
    self->field_0x40 = 1;
    self->field_0x44 = 1;
    self->field_0x48 = 0;
    self->field_0x4c = 0;
    self->field_0x50 = -1;
    self->field_0x52 = -1;
    self->field_0x54 = 0;
    self->field_0x55 = -1;
    self->field_0x56 = -1;
    self->field_0x57 = -1;
    self->field_0x58 = 0;
    self->field_0x59 = 1;
    self->field_0x5a = 0;
    self->field_0x5b = 0;
    self->field_0x5c = 0;
    self->field_0x5d = 0xFF;
    self->field_0x5e = -1;
    self->field_0x5f = -1;
    self->field_0x60 = -1;
    self->field_0x61 = 0;
    self->field_0x62 = 0xFF;
    self->field_0x63 = 0xFF;
    self->field_0x64 = 0xFF;
    self->field_0x68 = 0;
}

void MPVUMC_Finish(void);
void MPVSL_Finish(void);
void MPVM2V_Finish(void);


void MPV_Finish(void) {
    MPVUMC_Finish();
    MPVSL_Finish();
    MPVM2V_Finish();
    // Retail hoists both loads (flag + base) before the branch and runs a
    // 223-iteration dcbi cache-flush countdown (mtctr/bdnz). __dcbi has no
    // MWCC intrinsic (KB: dcbi gap); __dcbz keeps the countdown-loop shape.
    {
        u32 flag = lbl_eu_80602B88.flushSw;
        u8* base = (u8*)lbl_eu_80602B88.vlcBase;
        u32 off;
        if (flag & 0x10000000) {
            for (off = 0; off < 0x1BE0; off += 0x20) {
                __dcbz(base, off);
            }
        }
    }
}

void* mpvlib_InitHn(void* self);

void* MPV_Create(void* pool) {
    /* Scan the handle array for a free slot (state == 1). */
    s32 n = lbl_eu_80602B88.hnNum;
    MpvHn* h = (MpvHn*)lbl_eu_80602B88.memBase;

    for (; n > 0; n--) {
        if (h->state != MPV_HN_STATE_FREE) {
            h++;
        } else {
            break;
        }
    }
    if (n <= 0)
        h = NULL;

    if (h != NULL) {
        /* Cache-flush the found block: 110 lines of 32 bytes.
         * NOTE: retail emits inline dcbi+dcbz_l here; MWCC GC/3.0a5.2 has no
         * inline __dcbi/__dcbz_l intrinsics (both compile to external calls,
         * KB ref MWCC_CASES "dcbi intrinsic gap"), so __dcbz - the closest
         * real intrinsic - keeps the countdown-loop shape; only the cache-op
         * opcode differs (documented ceiling; see MPV_Finish/MPV_Destroy). */
        if (lbl_eu_80602B88.flushSw & 0x10000000) {
            u32 off;
            s32 i;
            for (i = 0x6e, off = 0; i != 0; i--) {
                __dcbz(h, off);
                __dcbz(h, off);
                off += 0x20;
            }
        }
        return mpvlib_InitHn(h);
    }
    return NULL;
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
    MpvHn* hn = (MpvHn*)self;
    u8* b = (u8*)self;
    u8* sub380 = b + 0x380;
    u32 base = lbl_eu_80602B88.vlcBase;
    /* VLC block bases derived from the config's vlc-block pointer */
    u32 blk11e0 = base + 0x11E0;
    u32 blk1200 = base + 0x1200;
    u32 blk1100 = base + 0x1100;
    u32 j;

    *(u32*)(b + 0x990) = lbl_eu_80604664;
    *(u32*)(b + 0x994) = lbl_eu_80604660 - 16;
    *(u32*)(b + 0x998) = lbl_eu_8060465C - 32;
    *(u32*)(b + 0x99C) = lbl_eu_80604658 - 32;
    /* j doubles as the 4654-load temp and later the SetUsrSj loop counter,
     * keeping it to a single live range/register (matches retail). */
    j = lbl_eu_80604654;
    *(u32*)(b + 0x9A0) = j;
    *(u32*)(b + 0x9A4) = lbl_eu_80604650;
    *(u32*)(b + 0x9A8) = lbl_eu_8060464C;
    *(u32*)(b + 0x9B8) = blk11e0;
    *(u32*)(b + 0x9BC) = blk1200;
    *(u32*)(b + 0x9B0) = blk1100;
    {
        u32 clip = lbl_eu_80602FE8;
        *(u32*)(b + 0x9C0) = clip;
        *(u32*)(b + 0xA90) = clip;
    }
    *(u32*)(b + 0xA94) = (u32)self;
    *(u32*)(b + 0xA98) = (u32)sub380;
    *(u32*)(b + 0xA9C) = (u32)self + 0x500;
    *(u32*)(b + 0xD0C) = (u32)self + 0x100;
    *(u32*)(b + 0xD10) = (u32)self + 0x180;
    *(u32*)(b + 0xD14) = (u32)self + 0x200;
    *(u32*)(b + 0xD18) = (u32)self + 0x280;
    *(u32*)(b + 0xD1C) = (u32)self;
    *(u32*)(b + 0xD20) = (u32)self + 0x80;
    *(u32*)(b + 0xB0C) = 0;
    MEM_Copy(hn->cfgImg, (u8*)&lbl_eu_80602B88, 64);
    *(u32*)(b + 0xB54) = 0;
    *(u32*)(b + 0xB58) = 0;
    *(u32*)(b + 0xB50) = 0x7FFFFFFF;
    MPVERR_InitErrInf(b + 0xBDC);
    MPVCMC_InitObj(self);
    mpvlib_InitPicAtr(&hn->picAtr);
    *(u32*)(b + 0xD24) = 0;
    *(u32*)(b + 0xD28) = 0;
    *(u32*)(b + 0xCFC) = 0;
    *(u32*)(b + 0xD00) = 0;
    *(u32*)(b + 0xD04) = 0;
    *(u32*)(b + 0xD08) = 0;
    *(u32*)(b + 0xD38) = 0;
    for (j = 0; j < 4; j++) {
        MPV_SetUsrSj(self, j, 0, 0, 0);
    }
    MPV_SetPicUsrBuf(self, 0, 0);
    *(u32*)(b + 0xDA8) = 0;
    MPVSL_Create(self);
    *(u32*)(b + 0xD54) = (u32)MPVM2V_Create(self);
    *(u32*)(b + 0xB08) = 2;
    return self;
}

void MPV_GetDctCnt(MpvHn* self, u32* out1, u32* out2) {
    *out1 = self->dctCntFwd;
    *out2 = self->dctCntBwd;
}

extern void MPVM2V_Destroy(void* self);
extern void MPVSL_Destroy(void* self);
extern u32 lbl_eu_80602FEC;

s32 MPV_Destroy(void* self) {
    s32 err;
    /* validate handle: NULL or state != 2(active) is an error; on success
       (state == 2) publish the handle to the globals before tearing down. */
    if (self == NULL) {
        err = -1;
    } else if (((MpvHn*)self)->state != MPV_HN_STATE_USED) {
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
    // Retail cache-invalidates 0x6e blocks (dcbi r31,r3 countdown). MWCC
    // GC/3.0a5.2 has no inline __dcbi intrinsic (it compiles to an external
    // call); __dcbz keeps the countdown-loop shape; only the opcode differs
    // (documented ceiling, MWCC_CASES MPV_Finish/MPV_Destroy).
    if (lbl_eu_80602B88.flushSw & 0x10000000) {
        s32 i;
        for (i = 0; i < 0x6e; i++) {
            __dcbz(self, i * 0x20);
        }
    }
    ((MpvHn*)self)->state = MPV_HN_STATE_FREE;
    return 0;
}

extern u32 lbl_eu_80602FEC;
void MPVM2V_SetCond(void* mpv);
s32 MPVERR_SetCode(s32 val, u32 err_code);

s32 MPV_SetCond(void* mpv, s32 cond, s32 val) {
    u8* tbl;
    if (mpv == NULL) {
        /* p walks the handle array; t mirrors it at the cond dword offset */
        s32 n = lbl_eu_80602B88.hnNum;
        u8* p = (u8*)lbl_eu_80602B88.memBase;
        u32 off = cond * 4;
        u32 t = off + (u32)p;
        while (n-- > 0) {
            if (*(s32*)(p + 0xb08) == MPV_HN_STATE_USED) {
                *(u32*)(t + 0xb10) = val;
            }
            t += 0xdc0;
            p += 0xdc0;
        }
        tbl = (u8*)&lbl_eu_80602B88;
    } else {
        s32 err;
        if (mpv == NULL) {
            err = -1;
        } else if (((MpvHn*)mpv)->state != MPV_HN_STATE_USED) {
            err = -1;
        } else {
            lbl_eu_80602FEC = (u32)mpv;
            err = 0;
        }
        if (err != 0) {
            return MPVERR_SetCode(0, 0xff030202);
        }
        tbl = (u8*)((MpvHn*)mpv)->cfgImg;
    }
    *(u32*)(tbl + 4 * cond) = val;
    MPVM2V_SetCond(mpv);
    return 0;
}

s32 MPV_GetCond(void* mpv, s32 cond, s32* out) {
    u32* tbl;
    if (mpv == NULL) {
        tbl = (u32*)&lbl_eu_80602B88;
    } else {
        s32 err;
        if (mpv == NULL) {
            err = -1;
        } else if (((MpvHn*)mpv)->state != MPV_HN_STATE_USED) {
            err = -1;
        } else {
            lbl_eu_80602FEC = (u32)mpv;
            err = 0;
        }
        if (err != 0) {
            return MPVERR_SetCode(0, 0xff030210);
        }
        tbl = ((MpvHn*)mpv)->cfgImg;
    }
    *out = tbl[cond];
    return 0;
}

void MPVM2V_SetMbCb(void* self, void* a, void* b, void* c);
void MPV_SetMbCb(MpvHn* self, void* a, void* b, void* c) {
    self->mbVal = (u32)a;
    self->mbCtx = c;
    self->mbCb = (void (*)(void*))b;
    MPVM2V_SetMbCb(self, a, b, c);
}

extern u32 lbl_eu_80602FEC;

int MPVLIB_CheckHn(MpvHn* handle) {
    if (handle == NULL) return -1;
    if (handle->state != MPV_HN_STATE_USED) return -1;
    lbl_eu_80602FEC = (u32)handle;
    return 0;
}
