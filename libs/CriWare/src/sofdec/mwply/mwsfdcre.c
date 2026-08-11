// Auto-scaffolded catalog TU for CriWare/src/sofdec/mwply/mwsfdcre
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

/* ---- Shared types for the Sofdec creator (MWSFCRE) ---- */

typedef struct TraceCb TraceCb;
typedef struct TraceCbVtable {
    u8 pad_0x00[0x24];
    void (*trace)(TraceCb* self, void* rec); /* 0x24 */
} TraceCbVtable;
struct TraceCb {
    const TraceCbVtable* vtable;
};

/* Per-function trace record (lbl_eu_80566*); entry sub-record at +0x04,
 * exit sub-record at +0x6c. */
typedef struct TraceRec {
    const char* name;   /* 0x00 */
    u32 field_0x04;     /* 0x04 */
    u32 field_0x08;     /* 0x08 */
    u32 self;           /* 0x0c */
    u32 field_0x10;     /* 0x10 */
    u32 field_0x14;     /* 0x14 */
    u32 arg;            /* 0x18 */
    u8 pad_0x1c[0x50];  /* 0x1c..0x6b */
    u32 field_0x6c;     /* 0x6c */
    u8 pad_0x70[0x4];   /* 0x70..0x73 */
    u32 field_0x74;     /* 0x74 */
} TraceRec;

/* Handle objects with a vtable exposing destroy at +0x0c (SJRBF/SJUNI). */
typedef struct HnObjVtable {
    u8 pad_0x00[0x0C];
    void (*destroy)(void* self);
} HnObjVtable;
typedef struct HnObj {
    HnObjVtable* vtable;
} HnObj;

/* MWSFDPLY movie-player handle. Partial layout covering the fields touched by
 * the creator helpers in this TU. */
typedef struct MWSFDPLY {
    u32 enable;             /* 0x00 */
    u32 status;             /* 0x04 */
    s32 width;              /* 0x08 */
    s32 height;             /* 0x0c */
    u32 frameCount;         /* 0x10 */
    u32 field_0x14;         /* 0x14 */
    s32 field_0x18;         /* 0x18 */
    u32 field_0x1c;         /* 0x1c */
    u32 field_0x20;         /* 0x20 */
    u32 field_0x24;         /* 0x24 */
    s32 poolCount;          /* 0x28 */
    s32 poolSize;           /* 0x2c */
    void** pool;            /* 0x30 */
    u8 pad_0x34[0x24];      /* 0x34..0x57 */
    void* sfd;              /* 0x58 */
    void* stm;              /* 0x5c */
    u8 pad_0x60[0x4];       /* 0x60..0x63 */
    void* lsc;              /* 0x64 */
    u8 pad_0x68[0x60];      /* 0x68..0xc7 */
    void* sfx;              /* 0xc8 */
    void* compoWork;        /* 0xcc */
    u32 compoWorkSize;      /* 0xd0 */
    u8 pad_0xd4[0x3E8];     /* 0xd4..0x4bb */
    void* picUsrTgt;        /* 0x4bc */
    u8 pad_0x4c0[0x4];      /* 0x4c0..0x4c3 */
    void* field_0x4c4;      /* 0x4c4 */
    u32 field_0x4c8;        /* 0x4c8 */
    u8 pad_0x4cc[0x34];     /* 0x4cc..0x4ff */
    u32 sj;                 /* 0x500 */
    HnObj* rbf;             /* 0x504 */
    u32 field_0x508;        /* 0x508 */
    u32 field_0x50c;        /* 0x50c */
    u32 field_0x510;        /* 0x510 */
    u32 field_0x514;        /* 0x514 */
    u32 field_0x518;        /* 0x518 */
    u32 field_0x51c;        /* 0x51c */
    u32 field_0x520;        /* 0x520 */
    HnObj* uni;             /* 0x524 */
    u8 pad_0x528[0xB0];     /* 0x528..0x5d7 */
    u8 field_0x5d8[0x5C];   /* 0x5d8..0x633 (SstHn sub-objects) */
    u32* field_0x634;       /* 0x634 */
} MWSFDPLY;

/* SFD creation parameter block shared by the frame-buffer allocators. */
typedef struct MWSFCRE_Para {
    u8 pad_0x00[0x08];
    s32 width;              /* 0x08 */
    s32 height;             /* 0x0c */
    s32 frameCount;         /* 0x10 */
    u8 pad_0x14[0x10];      /* 0x14..0x23 */
    s32 mode;               /* 0x24 */
    s32 outer;              /* 0x28 (outer frame pool mode) */
    s32 poolSize;           /* 0x2c */
    void* pool;             /* 0x30 */
} MWSFCRE_Para;

/* SFD_SetMpvParaTbl argument block sub-table: 10 default condition/value
 * pairs copied from the mode-specific template in .data. */
typedef struct SfdCondTbl {
    u32 field_0x00;  /* 0x00 */
    u32 field_0x04;  /* 0x04 */
    u32 field_0x08;  /* 0x08 */
    u32 field_0x0c;  /* 0x0c */
    u32 field_0x10;  /* 0x10 */
    u32 field_0x14;  /* 0x14 */
    u32 field_0x18;  /* 0x18 */
    u32 field_0x1c;  /* 0x1c */
    u32 field_0x20;  /* 0x20 */
    u32 field_0x24;  /* 0x24 */
    u32 field_0x28;  /* 0x28 */
    u32 field_0x2c;  /* 0x2c */
    u32 field_0x30;  /* 0x30 */
    u32 field_0x34;  /* 0x34 */
    u32 field_0x38;  /* 0x38 */
    u32 field_0x3c;  /* 0x3c */
    u32 field_0x40;  /* 0x40 */
    u32 field_0x44;  /* 0x44 */
    u32 field_0x48;  /* 0x48 */
    u32 field_0x4c;  /* 0x4c */
} SfdCondTbl;

/* SFD_SetSupplySj argument block. */
typedef struct MWSFCRE_SjArg {
    u32 a;                  /* 0x00 */
    u32 b;                  /* 0x04 */
    u32 c;                  /* 0x08 */
    u32 d;                  /* 0x0c */
    u32 e;                  /* 0x10 */
    u32 f;                  /* 0x14 */
} MWSFCRE_SjArg;

/* ---- External symbols / helpers ---- */

extern TraceCb* lbl_eu_805FF3A0;
extern TraceRec lbl_eu_805661AC;
extern TraceRec lbl_eu_80566280;
extern const char lbl_eu_8051A3CC[];

extern u32 lbl_eu_805FF2E4;
extern u32 lbl_eu_805FF2E8;
extern u32 lbl_eu_805660D0;
extern const SfdCondTbl lbl_eu_80567168;
extern const SfdCondTbl lbl_eu_805671B8;
extern const SfdCondTbl lbl_eu_80567208;

extern void MWSFSVM_Error(const char* message, ...);
extern s32 MWSFLIB_SetErrCode(s32 code);
extern void* MWSFLIB_GetLibWorkPtr(void);
extern s32 MWSFD_GetUsePicUsr(void);
extern void MWSFD_SetProhibitServer(s32 val);
extern void MWSFLIB_SfdErrFunc(u32 a, u32 b);

extern s32 SFD_Stop(void* sfd);
extern s32 SFD_Destroy(void* sfd);
extern s32 SFD_SetSupplySj(void* sfd, MWSFCRE_SjArg* arg);
extern void SFD_SetPicUsrBuf(void* sfd, void* buf, s32 height, s32 count);
extern void SFD_SetCond(void* sfd, u32 cond, u32 val);

extern s32 criware_803C0D94(void* handle, void (*errFn)(u32, u32), u32 errArg);
extern void criware_803A3A48(void* pool);
extern void* criware_803A3AE4(void* pool, u32 size);
extern s32 fn_803A537C(void* self);
extern s32 fn_803A7320(void* self);
extern s32 mwSfdStopDec(void* self);

extern s32 MWSFSFX_CalcHnWorkSiz(u32 a, u32 b);
extern void MWSFSFX_Destroy(void* sfx);
extern s32 MWSFTAG_IsUseAinfSj(void* self);
extern void MWSFTAG_DestroyAinfSj(void* self);
extern void LSC_Destroy(void* lsc);
extern void MWSTM_Destroy(void* stm);
extern void MWSST_Destroy(void* sst);

extern int sprintf(char* buf, const char* fmt, ...);
extern void* memset(void* dst, int val, size_t n);

void* mwPlyCreateSofdec(MWSFDPLY* self);
void mwply_Destroy(MWSFDPLY* self);

/* ---- Target implementations ---- */

s32 mwsfcre_IsOuterFrmPoolUsed(MWSFDPLY* self) {
    s32 poolCount = self->poolCount;
    s32 poolSize = self->poolSize;
    void** pool = self->pool;
    s32 w = self->width;
    s32 h = self->height;
    s32 ok = 1;
    if (poolCount > 0 || poolSize > 0 || pool != NULL) {
        s32 w16 = (w + 15) / 16;
        s32 h16 = (h + 15) / 16;
        s32 W = w16 * 16;
        s32 H = h16 * 16;
        s32 luma = ((w16 >> 27) & 1) + W;
        s32 chroma = ((h16 >> 27) & 1) + H;
        s32 frameSize = ((chroma >> 1) * (((luma >> 1) + 0x1f) / 32 * 32)) * 2 +
                        H * (((W + 0x1f) / 32) * 32) + 0x20;
        if (poolSize < frameSize) {
            MWSFSVM_Error(lbl_eu_8051A3CC + 0xB9);
            ok = 0;
        }
        if (pool == NULL) {
            MWSFSVM_Error(lbl_eu_8051A3CC + 0xDB);
            ok = 0;
        }
        for (s32 i = 0; i < poolCount; i++) {
            if (pool[i] == NULL) {
                MWSFSVM_Error(lbl_eu_8051A3CC + 0xFC);
                ok = 0;
            }
        }
    } else {
        ok = 0;
    }
    return ok;
}

void MWSFCRE_SetSupplySj(MWSFDPLY* self) {
    u32 sj = self->sj;
    void* sfd = self->sfd;
    if (sj != 0) {
        MWSFCRE_SjArg arg;
        if (sj == (u32)self->uni) {
            arg.a = 2;
            arg.b = sj;
            arg.c = 0;
            arg.d = 0;
            arg.e = 0;
            arg.f = 0;
        } else if (sj == (u32)self->rbf) {
            arg.a = 0;
            arg.b = sj;
            arg.c = self->field_0x508;
            arg.d = self->field_0x50c;
            arg.e = self->field_0x510;
            arg.f = 0;
        } else {
            arg.a = self->field_0x514;
            arg.b = sj;
            arg.c = self->field_0x518;
            arg.d = self->field_0x51c;
            arg.e = self->field_0x520;
            arg.f = 0;
        }
        if (self->sj == (u32)self->uni) {
            SFD_SetCond(self->sfd, 0x55, 1);
            SFD_SetCond(self->sfd, 0x56, 0x800);
        } else {
            SFD_SetCond(self->sfd, 0x55, 0);
            SFD_SetCond(self->sfd, 0x56, 4);
        }
        if (SFD_SetSupplySj(sfd, &arg)) {
            MWSFLIB_SetErrCode(-0x138);
            MWSFSVM_Error(lbl_eu_8051A3CC + 0x11D);
        }
    }
}

s32 mwsfcre_MallocRfb(void* pool, MWSFCRE_Para* para, void** out) {
    s32 rc = 0;
    s32 w = para->width;
    s32 h = para->height;
    s32 outer = para->outer;
    s32 w16 = (w + 15) / 16;
    s32 h16 = (h + 15) / 16;
    s32 W = w16 * 16;
    s32 H = h16 * 16;
    s32 luma = ((w16 >> 27) & 1) + W;
    s32 chroma = ((h16 >> 27) & 1) + H;
    s32 frameSize = ((chroma >> 1) * (((luma >> 1) + 0x1f) / 32 * 32)) * 2 +
                    H * (((W + 0x1f) / 32) * 32) + 0x20;
    if (outer != 0) {
        if (outer < 2 || para->poolSize < frameSize) {
            out[0] = NULL;
            out[1] = NULL;
            rc = -1;
        } else {
            out[0] = (void*)((u32*)para->pool)[0];
            out[1] = (void*)((u32*)para->pool)[1];
        }
    } else {
        out[0] = criware_803A3AE4(pool, frameSize);
        out[1] = criware_803A3AE4(pool, frameSize);
    }
    if (out[0] == NULL || out[1] == NULL) {
        rc = -1;
    }
    return rc;
}

s32 mwsfcre_MallocTab(void* pool, MWSFCRE_Para* para, void** out) {
    s32 rc = 0;
    s32 mode = para->mode;
    s32 frames = para->frameCount;
    s32 w = para->width;
    s32 h = para->height;
    if ((u32)mode > 3) {
        MWSFSVM_Error(lbl_eu_8051A3CC);
    }
    s32 w16 = (w + 15) / 16;
    s32 h16 = (h + 15) / 16;
    s32 W = w16 * 16;
    s32 H = h16 * 16;
    s32 luma = ((w16 >> 27) & 1) + W;
    s32 chroma = ((h16 >> 27) & 1) + H;
    s32 frameSize = ((chroma >> 1) * (((luma >> 1) + 0x1f) / 32 * 32)) * 2 +
                    H * (((W + 0x1f) / 32) * 32) + 0x20;
    if (para->outer != 0) {
        if (para->outer < frames + 2 || para->poolSize < frameSize) {
            rc = -1;
        } else {
            for (s32 i = 0; i < frames; i++) {
                out[i] = (void*)((u32*)para->pool)[i + 2];
                if (out[i] == NULL) {
                    rc = -1;
                }
            }
        }
    } else {
        for (s32 i = 0; i < frames; i++) {
            out[i] = criware_803A3AE4(pool, frameSize);
            if (out[i] == NULL) {
                rc = -1;
            }
        }
    }
    return rc;
}

#pragma push
#pragma auto_inline off
void mwsfcre_AttachPicUsrBuf(MWSFDPLY* self) {
    void* tgt = self->picUsrTgt;
    if (tgt == NULL) {
        MWSFSVM_Error(lbl_eu_8051A3CC + 0x25F);
    } else {
        s32 h = self->field_0x18;
        s32 count = *(s32*)((u8*)tgt + 0x08);
        s32 size = *(s32*)((u8*)tgt + 0x04);
        void* buf = *(void**)((u8*)tgt + 0x00);
        if (size < (h + 3) * count) {
            MWSFSVM_Error(lbl_eu_8051A3CC + 0x294);
        } else if (MWSFD_GetUsePicUsr() == 1) {
            SFD_SetPicUsrBuf(self->sfd, buf, h + 3, count);
        }
    }
}
#pragma pop

s32 MWSFCRE_ResetSfdHn(MWSFDPLY* self) {
    void* sfd = self->sfd;
    self->sfd = NULL;
    if (SFD_Stop(sfd)) {
        MWSFSVM_Error(lbl_eu_8051A3CC + 0x31F);
        return -1;
    }
    self->sfd = sfd;
    if (criware_803C0D94(sfd, MWSFLIB_SfdErrFunc, (u32)self)) {
        MWSFLIB_SetErrCode(-0x12F);
        MWSFSVM_Error(lbl_eu_8051A3CC + 0x34F);
        return -1;
    }
    mwsfcre_AttachPicUsrBuf(self);
    return 0;
}

void* criware_8039FF34(MWSFDPLY* self) {
    if (lbl_eu_805FF3A0 != NULL) {
        char buf[0x200];
        sprintf(buf, lbl_eu_8051A3CC + 0x383, self->enable, self->status,
                self->width, self->height, self->frameCount, self->field_0x14,
                self->field_0x18, self->field_0x1c, self->field_0x20,
                self->field_0x24, self->poolCount, self->poolSize, self->pool);
        TraceCb* cb = lbl_eu_805FF3A0;
        if (cb != NULL) {
            lbl_eu_805661AC.self = (u32)buf;
            cb->vtable->trace(cb, &lbl_eu_805661AC.field_0x04);
        }
    }
    {
        void* hn = mwPlyCreateSofdec(self);
        TraceCb* cb = lbl_eu_805FF3A0;
        if (cb != NULL) {
            lbl_eu_805661AC.field_0x74 = (u32)hn;
            cb->vtable->trace(cb, &lbl_eu_805661AC.field_0x6c);
        }
        return hn;
    }
}

s32 mwsfcre_MallocCompoWork(MWSFDPLY* self) {
    void* tag = (u8*)self + 8;
    s32 size = MWSFSFX_CalcHnWorkSiz(self->frameCount, *(u32*)((u8*)tag + 0x0C));
    void* work = criware_803A3AE4(self, size);
    if (work == NULL) {
        MWSFSVM_Error(lbl_eu_8051A3CC + 0x572);
        criware_803A3A48(self);
        return -1;
    }
    self->compoWork = work;
    self->compoWorkSize = size;
    if (MWSFTAG_IsUseAinfSj(tag) == 1) {
        void* ainf = criware_803A3AE4(self, 0x20000);
        if (ainf == NULL) {
            MWSFSVM_Error(lbl_eu_8051A3CC + 0x593);
            criware_803A3A48(self);
            return -1;
        }
        self->field_0x4c4 = ainf;
        self->field_0x4c8 = 0x20000;
    } else {
        self->field_0x4c4 = NULL;
        self->field_0x4c8 = 0;
    }
    return 0;
}

void criware_803A09B4(MWSFDPLY* self) {
    fn_803A537C(self);
    if (lbl_eu_805FF3A0 != NULL) {
        lbl_eu_80566280.self = (u32)self;
        lbl_eu_805FF3A0->vtable->trace(lbl_eu_805FF3A0, &lbl_eu_80566280.field_0x04);
    }
    mwply_Destroy(self);
    if (lbl_eu_805FF3A0 != NULL) {
        lbl_eu_805FF3A0->vtable->trace(lbl_eu_805FF3A0, &lbl_eu_80566280.field_0x6c);
    }
    *(u32*)((u8*)MWSFLIB_GetLibWorkPtr() + 0x34F4) -= 1;
}

void mwply_Destroy(MWSFDPLY* self) {
    if (self == NULL) {
        return;
    }
    MWSFD_SetProhibitServer(1);
    mwSfdStopDec(self);
    self->enable = 0;
    MWSFD_SetProhibitServer(0);
    MWSFTAG_DestroyAinfSj(self);
    if (self->sfx != NULL) {
        MWSFSFX_Destroy(self->sfx);
    }
    if (self->lsc != NULL) {
        LSC_Destroy(self->lsc);
    }
    if (self->stm != NULL) {
        MWSTM_Destroy(self->stm);
    }
    if (self->rbf != NULL) {
        self->rbf->vtable->destroy(self->rbf);
    }
    if (self->uni != NULL) {
        self->uni->vtable->destroy(self->uni);
    }
    if (self->field_0x634 != NULL) {
        *self->field_0x634 = 0;
    }
    if (self->sfd != NULL) {
        if (SFD_Destroy(self->sfd)) {
            MWSFLIB_SetErrCode(-0x132);
            MWSFSVM_Error(lbl_eu_8051A3CC + 0x301);
        }
    }
    MWSST_Destroy((void*)((u8*)self + 0x5D8));
    MWSST_Destroy((void*)((u8*)self + 0x600));
    criware_803A3A48(self);
    if (fn_803A7320(self)) {
        MWSFSVM_Error(lbl_eu_8051A3CC + 0x5B8);
    }
    memset(self, 0, 0x690);
    self->enable = 0;
}

/* ---- Non-target stubs preserved from the scaffold ---- */

extern const f64 lbl_eu_8051A3C0;  /* 0x4330000080000000 (int->float conversion bias) */

void mwsfcre_CalcWorkStmBuf(void* stm, u32* outA, u32* outB, u32* outC,
    u32* outD, u32* outE, u32* outF) {
    u8* s = (u8*)stm;
    s32 fmt = *(s32*)(s + 0x00);
    s32 size = *(s32*)(s + 0x04);
    s32 n = *(s32*)(s + 0x14);
    s32 size2 = size;

    if (n <= 0)
        n = 1;

    if (*(u32*)(s + 0x38) != 0) {
        if (size != 0) {
            s32 lim = (s32)(*(float*)(s + 0x3C) * (float)size);
            size2 = 0x8000;
            if (lim > 0x8000)
                size2 = lim;
        }
        n = 1;
    }

    if (fmt == 2 || fmt == 6 || fmt == 8 || fmt == 0xA) {
        s32 d = size2 / 8 / 2048;
        *outA = (u32)(n * (d << 11));
        *outB = 0;
        *outC = 0;
        *outD = 0;
        *outE = 0;
        *outF = 0;
    } else if (fmt == 3 || fmt == 7) {
        s32 d = size2 / 8 / 2048;
        s32 base = d << 11;
        *outA = (u32)(n * base);
        *outB = 0;
        *outC = (u32)(((base + ((base >> 31) & 1)) >> 1) + 0x800);
        *outD = 0;
        *outE = 0;
        *outF = 0;
    } else {
        s32 d = size2 / 8 / 2048;
        s32 base = d << 11;
        *outA = (u32)(n * base);
        *outB = 0;
        *outC = (u32)(((base + ((base >> 31) & 1)) >> 1) + 0x800);
        *outD = 0x5DCC;
        *outE = 0x5F0C;
        *outF = 0x10000 - 0x3E40;
    }
}

void mwPlyCalcWorkCprmSfd() {}

void criware_eu_803A29E0(s32 mode, u32 unused, SfdCondTbl* dest, MWSFDPLY* self) {
    /* Sparse mode dispatch: equality chain, case bodies appended after. */
    if (mode == 1) goto case1;
    if (mode == 2) goto case2;
    if (mode == 3) goto case3;
    if (mode == 8) goto case8;
    if (mode == 9) goto case9;
    if (mode == 0xb) goto case0xb;
    return;
case1:
    /* Copy the default SFD condition table and set the rbf supply params. */
    *dest = lbl_eu_80567168;
    self->field_0x508 = lbl_eu_805FF2E4;
    self->field_0x50c = lbl_eu_805FF2E8 - lbl_eu_805660D0;
    self->field_0x510 = lbl_eu_805660D0;
    return;
case2:
    *dest = lbl_eu_805671B8;
    self->field_0x508 = lbl_eu_805FF2E4;
    self->field_0x50c = lbl_eu_805FF2E8 - 0x800;
    self->field_0x510 = 0x800;
    return;
case3:
    *dest = lbl_eu_80567208;
    self->field_0x508 = lbl_eu_805FF2E4;
    self->field_0x50c = lbl_eu_805FF2E8 - lbl_eu_805660D0;
    self->field_0x510 = lbl_eu_805660D0;
    return;
case8:
    MWSFSVM_Error(lbl_eu_8051A3CC + 0x13c);
    return;
case9:
    MWSFSVM_Error(lbl_eu_8051A3CC + 0x164);
    return;
case0xb:
    MWSFSVM_Error(lbl_eu_8051A3CC + 0x18c);
    return;
}

void mwsfcre_CreateSfd() {}

void MWSFCRE_SetCondSfd() {}

#pragma push
#pragma auto_inline off
void* mwPlyCreateSofdec(MWSFDPLY* self) { return NULL; }
#pragma pop

uint32_t criware_803A0BA8(void* r3_this, uint32_t r4) {
    uint32_t a = *(uint32_t*)((char*)r3_this + 0x54c);
    uint32_t limit = *(uint32_t*)((char*)r3_this + 0x544);
    uint32_t sum = a + r4;
    if (sum > limit) {
        return 0;
    }
    uint32_t b = *(uint32_t*)((char*)r3_this + 0x548);
    *(uint32_t*)((char*)r3_this + 0x54c) = sum;
    *(uint32_t*)((char*)r3_this + 0x548) = b + r4;
    return b;
}

void criware_803A0BDC(void* self) {
    void* work = MWSFLIB_GetLibWorkPtr();
    void (*cb)(void*, void*) = (void(*)(void*, void*))*(void**)((u8*)work + 0x28);
    void* param = *(void**)((u8*)work + 0x30);
    cb(param, self);
}

void criware_803A0C1C() {}

void criware_803A0C20(void* self) {
    void* work = MWSFLIB_GetLibWorkPtr();
    void (*cb)(void*, void*) = (void(*)(void*, void*))*(void**)((u8*)work + 0x2C);
    void* param = *(void**)((u8*)work + 0x30);
    cb(param, self);
}
