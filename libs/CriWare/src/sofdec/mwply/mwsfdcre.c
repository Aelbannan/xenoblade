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

/* User create-parameter block copied into the player at offset 4 (the copy is
 * what MWCC splits into 8-byte chunks). */
typedef struct PlyCfg {
    u32 data;
} PlyCfg;

/* MWSFDPLY movie-player handle (one slot per 0x690 bytes). Partial layout
 * covering the fields touched by the creator helpers in this TU. */
typedef struct MWSFDPLY {
    u32 enable;             /* 0x000 */
    PlyCfg cfg;             /* 0x004 */
    s32 width;              /* 0x008 */
    s32 height;             /* 0x00c */
    u32 frameCount;         /* 0x010 */
    u32 field_0x14;         /* 0x014 */
    s32 field_0x18;         /* 0x018 */
    u32 field_0x1c;         /* 0x01c */
    u32 field_0x20;         /* 0x020 */
    u32 field_0x24;         /* 0x024 */
    s32 poolCount;          /* 0x028 */
    s32 poolSize;           /* 0x02c */
    void** pool;            /* 0x030 */
    u8 pad_0x34[0x20];      /* 0x034..0x053 */
    u32 field_0x54;         /* 0x054 */
    void* sfd;              /* 0x058 */
    void* stm;              /* 0x05c */
    u32 field_0x60;         /* 0x060 */
    void* lsc;              /* 0x064 */
    u32 field_0x68;         /* 0x068 */
    u32 field_0x6c;         /* 0x06c */
    u32 field_0x70;         /* 0x070 */
    u32 field_0x74;         /* 0x074 */
    u32 field_0x78;         /* 0x078 */
    u8 pad_0x7c[0x08];      /* 0x07c..0x083 */
    u32 field_0x84;         /* 0x084 */
    u32 field_0x88;         /* 0x088 */
    u32 field_0x8c;         /* 0x08c */
    u8 field_0x90[0x04];    /* 0x090..0x093 */
    u32 field_0x94;         /* 0x094 */
    u8 pad_0x98[0x0c];      /* 0x098..0x0a3 */
    u32 field_0xa4;         /* 0x0a4 */
    u8 pad_0xa8[0x20];      /* 0x0a8..0x0c7 */
    void* sfx;              /* 0x0c8 */
    void* compoWork;        /* 0x0cc */
    u32 compoWorkSize;      /* 0x0d0 */
    u8 pad_0xd4[0x3d0];     /* 0x0d4..0x04a3 */
    u32 field_0x4a4;        /* 0x4a4 */
    u32 field_0x4a8;        /* 0x4a8 */
    u32 field_0x4ac;        /* 0x4ac */
    u8 pad_0x4b0[0x0c];     /* 0x4b0..0x4bb */
    void* picUsrTgt;        /* 0x4bc */
    void* field_0x4c0;      /* 0x4c0 */
    void* field_0x4c4;      /* 0x4c4 */
    u32 field_0x4c8;        /* 0x4c8 */
    u8 pad_0x4cc[0x1c];     /* 0x4cc..0x4e7 */
    void* field_0x4e8;      /* 0x4e8 */
    u32 field_0x4ec;        /* 0x4ec */
    u8 pad_0x4f0[0x10];     /* 0x4f0..0x4ff */
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
    u32 field_0x528;        /* 0x528 */
    u32 field_0x52c;        /* 0x52c */
    u8 pad_0x530[0x10];     /* 0x530..0x53f */
    u32 field_0x540;        /* 0x540 */
    u32 field_0x544;        /* 0x544 */
    u32 field_0x548;        /* 0x548 */
    u32 field_0x54c;        /* 0x54c */
    u8 pad_0x550[0x08];     /* 0x550..0x557 */
    u32 field_0x558;        /* 0x558 */
    u32 zeros[31];          /* 0x55c..0x5d4 */
    u8 field_0x5d8[0x5C];   /* 0x5d8..0x633 (SstHn sub-objects) */
    u32* field_0x634;       /* 0x634 */
    u32 field_0x638;        /* 0x638 */
    u32 field_0x63c;        /* 0x63c */
    u32 field_0x640;        /* 0x640 */
    u32 field_0x644;        /* 0x644 */
    u8 pad_0x648[0x04];     /* 0x648..0x64b */
    u32 field_0x64c;        /* 0x64c */
    u8 pad_0x650[0x10];     /* 0x650..0x65f */
    u32 field_0x660;        /* 0x660 */
    u32 field_0x664;        /* 0x664 */
    u32 field_0x668;        /* 0x668 */
    u32 field_0x66c;        /* 0x66c */
    u32 field_0x670;        /* 0x670 */
    u32 field_0x674;        /* 0x674 */
    u32 field_0x678;        /* 0x678 */
    u32 field_0x67c;        /* 0x67c */
    u32 field_0x680;        /* 0x680 */
    u8 pad_0x684[0x0c];     /* 0x684..0x68f */
} MWSFDPLY;

/* SFD creation parameter block shared by the frame-buffer allocators. */
typedef struct MWSFCRE_Para {
    s32 fmt;                /* 0x00 stream format code (2/3/6/7/8/0xa...) */
    u8 pad_0x04[0x04];
    s32 width;              /* 0x08 */
    s32 height;             /* 0x0c */
    s32 frameCount;         /* 0x10 */
    u8 pad_0x14[0x10];      /* 0x14..0x23 */
    s32 mode;               /* 0x24 */
    s32 outer;              /* 0x28 (outer frame pool mode) */
    s32 poolSize;           /* 0x2c */
    void* pool;             /* 0x30 */
    u32 field_0x34;         /* 0x34 */
    u8 pad_0x38[0x08];      /* 0x38..0x3f */
    void* field_0x40;       /* 0x40 */
} MWSFCRE_Para;

/* Per-slot scratch block living at lbl_eu_805FF2E0: stream-buffer sizes
 * computed by mwsfcre_CalcWorkStmBuf plus the frame-buffer sizes. */
typedef struct SfdWork {
    u32 field_0x00;         /* 0x00 */
    u32 field_0x04;         /* 0x04 */
    u32 outA;               /* 0x08 */
    u32 outB;               /* 0x0c */
    u32 outC;               /* 0x10 */
    u32 outD;               /* 0x14 */
    u32 outE;               /* 0x18 */
    u32 outF;               /* 0x1c */
    u32 size0;              /* 0x20 */
    u32 size1;              /* 0x24 */
} SfdWork;

/* Geometry info consumed by SFD_SetMpvParaTbl. */
typedef struct MpvInfo {
    u32 yStride;            /* 0x00 */
    u32 chromaHeight;       /* 0x04 */
    u32 width;              /* 0x08 */
    u32 height;             /* 0x0c */
    u32 field_0x10;         /* 0x10 */
    u32 width2;             /* 0x14 */
    u32 height2;            /* 0x18 */
    u32 frameCount;         /* 0x1c */
    u32 field_0x20;         /* 0x20 */
} MpvInfo;

/* ADX decoder configuration (two entries patched per stream). */
typedef struct AdxtCfg {
    u8 pad_0x00[0x08];      /* 0x00..0x07 */
    u32 field_0x08;         /* 0x08 */
    u8 pad_0x0c[0x0c];      /* 0x0c..0x17 */
    u32 field_0x18;         /* 0x18 */
} AdxtCfg;

/* SFD_Create parameter block (condition table + buffer descriptions). */
typedef struct SfdCreateCfg {
    u32 field_0x00;         /* 0x00 */
    void* stmBuf;           /* 0x04 */
    u32 field_0x08;         /* 0x08 */
    u32 field_0x0c;         /* 0x0c */
    u32 field_0x10;         /* 0x10 */
    u8 pad_0x14[0x14];      /* 0x14..0x27 */
    u32 field_0x28;         /* 0x28 */
    u32 field_0x2c;         /* 0x2c */
    u32 field_0x30;         /* 0x30 */
    u32 field_0x34;         /* 0x34 */
    u32 width;              /* 0x38 */
    u32 height;             /* 0x3c */
    u32 modeIdx;            /* 0x40 */
    u32 field_0x44;         /* 0x44 */
    u32 field_0x48;         /* 0x48 */
    u32 field_0x4c;         /* 0x4c */
} SfdCreateCfg;

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

extern SfdWork lbl_eu_805FF2E0;
extern u32 lbl_eu_805FF2E4;
extern u32 lbl_eu_805FF2E8;
extern u32 lbl_eu_805660D0;
extern MpvInfo lbl_eu_80567258;
extern AdxtCfg lbl_eu_8056727C;
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
extern void SFD_SetMpvCond(void* sfd, u32 cond, u32 val);

extern s32 criware_803C0D94(void* handle, void (*errFn)(u32, u32), u32 errArg);
extern void criware_803A3A48(void* pool);
extern void* criware_803A3AE4(void* pool, u32 size);
extern s32 fn_803A537C(void* self);
extern s32 fn_803A7320(void* self);
extern s32 fn_803A7328(void);
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

extern void SFD_SetAdxtPara(AdxtCfg* cfg);
extern void* SFD_Create(SfdCreateCfg* cfg, s32 arg);
extern s32 SFD_GetSofdecHeader(void* sfd, s32 arg, u32* out);
extern void* SJRBF_Create(u32 a, u32 b, u32 c);
extern void* SJUNI_Create(s32 a, u32 b, u32 c);
extern void* MWSTM_Create(HnObj* rbf);
extern void* LSC_Create(HnObj* rbf);
extern void LSC_SetStmHndl(void* lsc, void* stm);
extern void MWSFPLY_SetFlowLimit(MWSFDPLY* self);
extern void* MWSFSFX_Create(void* work, u32 size, s32 w, s32 h);
extern void MWSFSFX_SetCompoMode(MWSFDPLY* self, u32 mode);
extern void* MWSFTAG_CreateAinfSj(MWSFDPLY* self);
extern s32 MWSFTAG_SetAinfSj(MWSFDPLY* self);
extern void MWSFTAG_InitTagInf(MWSFDPLY* self);
extern void MWSFFRM_InitSfhInfTable(MWSFDPLY* self);
extern void MWSFFRM_SetShfCbFn(MWSFDPLY* self);
extern void MWSFSVR_SetHnMwplySvrFlg(MWSFDPLY* self, s32 flg);
extern void MWSFSVR_SetHnSfdSvrFlg(MWSFDPLY* self, s32 flg);
extern void criware_803A3B98(MWSFDPLY* self);

void* mwPlyCreateSofdec(MWSFDPLY* self);
void mwply_Destroy(MWSFDPLY* self);
void* mwsfcre_CreateSfd(MWSFDPLY* ply, MWSFCRE_Para* para);
void MWSFCRE_SetCondSfd(MWSFDPLY* ply, MWSFDPLY* user, u32 a, u32 b);
void mwsfcre_CalcWorkStmBuf(void* stm, u32* outA, u32* outB, u32* outC,
    u32* outD, u32* outE, u32* outF);

/* ---- Target implementations ---- */

s32 mwsfcre_IsOuterFrmPoolUsed(MWSFDPLY* self) {
    s32 ok = 1;
    void** pool;
    s32 poolCount = self->poolCount;
    s32 w;
    s32 poolSize = self->poolSize;
    s32 h;
    pool = self->pool;
    w = self->width;
    h = self->height;
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
    MWSFCRE_SjArg arg;
    if (sj != 0) {
        if (sj == (u32)self->uni) {
            arg.a = 2;
            arg.b = sj;
            arg.c = arg.d = arg.e = 0;
            arg.f = 0;
        } else {
            if (sj == (u32)self->rbf) {
                arg.a = 0;
                arg.b = sj;
                arg.c = self->field_0x508;
                arg.d = self->field_0x50c;
                arg.e = self->field_0x510;
                arg.f = 0;
            } else {
                arg.a = self->field_0x514, arg.b = sj;
                arg.c = self->field_0x518;
                arg.d = self->field_0x51c;
                arg.e = self->field_0x520;
                arg.f = 0;
            }
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
    s32 outer = para->outer;
    /* Round width/height up to macroblock multiples and size one RGB565 +
     * double-pitch-chroma frame (+0x20 header). */
    s32 w16 = (para->width + 15) / 16;
    s32 h16 = (para->height + 15) / 16;
    s32 W = w16 * 16;
    s32 H = h16 * 16;
    s32 luma = ((w16 >> 27) & 1) + W;
    s32 chroma = ((h16 >> 27) & 1) + H;
    s32 frameSize = ((chroma >> 1) * (((luma >> 1) + 0x1f) / 32 * 32)) * 2 +
                    H * (((W + 0x1f) / 32) * 32) + 0x20;
    s32 rc = 0;
    if (outer != 0) {
        if (outer < 2)
            goto fail;
        if (para->poolSize < frameSize)
            goto fail;
        /* Negated second test (not `else goto`) emits retail's blt/bge pair. */
        if (para->poolSize >= frameSize)
            goto good;

    fail:
        out[0] = NULL;
        rc = -1;
        out[1] = NULL;
        goto join;
    good:
        out[0] = ((void**)para->pool)[0];
        out[1] = ((void**)para->pool)[1];
        goto join;
    } else {
        out[0] = criware_803A3AE4(pool, frameSize);
        out[1] = criware_803A3AE4(pool, frameSize);
    }
join:
    if (out[0] == NULL || out[1] == NULL) {
        rc = -1;
    }
    return rc;
}

s32 mwsfcre_MallocTab(void* pool, MWSFCRE_Para* para, void** out) {
    s32 frames = para->frameCount;
    s32 rc = 0;
    s32 i;
    s32 frameSize;
    s32 w = para->width;
    s32 h = para->height;
    if ((u32)para->mode > 3) {
        MWSFSVM_Error(lbl_eu_8051A3CC);
    }
    s32 w16 = (w + 15) / 16;
    s32 h16 = (h + 15) / 16;
    s32 W = w16 * 16;
    s32 H = h16 * 16;
    s32 luma = ((w16 >> 27) & 1) + W;
    s32 chroma = ((h16 >> 27) & 1) + H;
    frameSize = H * (((W + 0x1f) / 32) * 32) +
                ((chroma >> 1) * (((luma >> 1) + 0x1f) / 32 * 32)) * 2 + 0x20;
    if (para->outer != 0) {
        if (para->outer < frames + 2 || para->poolSize < frameSize) {
            rc = -1;
        } else {
            for (i = 0; i < frames; i++) {
                if ((out[i] = ((void**)para->pool)[i + 2]) == NULL)
                    rc = -1;
            }
        }
    } else {
        for (i = 0; i < frames; i++) {
            if ((out[i] = criware_803A3AE4(pool, frameSize)) == NULL)
                rc = -1;
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
        void* buf;
        s32 count;
        s32 size;
        s32 h = self->field_0x18;
        count = *(s32*)((u8*)tgt + 0x08);
        size = *(s32*)((u8*)tgt + 0x04);
        buf = *(void**)((u8*)tgt + 0x00);
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
        sprintf(buf, &lbl_eu_8051A3CC[0x383], self->enable,
                self->cfg.data, self->width, self->height,
                self->frameCount, self->field_0x14, self->field_0x18,
                self->field_0x1c, self->field_0x20, self->field_0x24,
                self->poolCount, self->poolSize, self->pool);
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
extern const f32 lbl_eu_8051A3C8;

void mwsfcre_CalcWorkStmBuf(void* stm, u32* outA, u32* outB, u32* outC,
    u32* outD, u32* outE, u32* outF) {
    u8* s = (u8*)stm;
    s32 fmt = *(s32*)(s + 0x00);
    s32 size = *(s32*)(s + 0x04);
    s32 n = *(s32*)(s + 0x14);

    if (n <= 0)
        n = 1;

    if (*(u32*)(s + 0x38) != 0) {
        if (size != 0) {
            /* Int->float conversion plus sample-rate scale; clamped to the
             * 0x8000 minimum stream-buffer size. */
            s32 lim = (s32)(*(f32*)(s + 0x3C) * (f32)size);
            size = 0x8000;
            if (lim > 0x8000)
                size = lim;
        }
        n = 1;
    }

    if (fmt == 2 || fmt == 6 || fmt == 8 || fmt == 0xA) {
        *outA = (u32)(n * ((size / 8 / 2048) << 11));
        *outB = 0;
        *outC = 0;
        *outD = 0;
        *outE = 0;
        *outF = 0;
    } else if (fmt == 3 || fmt == 7) {
        s32 d = size / 8 / 2048;
        s32 base = d << 11;
        *outA = (u32)(n * base);
        *outB = 0;
        *outC = (u32)(base / 2) + 0x800;
        *outD = 0;
        *outE = 0;
        *outF = 0;
    } else {
        s32 d = size / 8 / 2048;
        s32 base = d << 11;
        *outA = (u32)(n * base);
        *outB = 0;
        *outC = (u32)(base / 2) + 0x800;
        *outD = 0x5DCC;
        *outE = 0x5F0C;
        *outF = 0x10000 - 0x3E40;
    }
}

/* Computes the total work-buffer size needed for one SFD player slot:
 * stream buffers + frame buffers (+ SFX handler block + optional AINF SJ
 * block of 0x20000 bytes). */
s32 mwPlyCalcWorkCprmSfd(MWSFDPLY* self) {
    u32 frames;
    s32 siz[2];
    u32 out[6];
    s32 sfxSize;
    s32 result;
    s32 total;
    s32 half;

    if (self == NULL) {
        MWSFSVM_Error(lbl_eu_8051A3CC + 0x25);
        return 0;
    }
    s32 mode = self->enable;
    if (mode == 7 || mode == 6 || mode == 0xa) {
        MWSFSVM_Error(lbl_eu_8051A3CC + 0x52);
        return 0;
    }
    if (mode == 9 || mode == 8 || mode == 0xb) {
        /* EU-only modes require the extra server probe to pass. */
        if (fn_803A7328() == 0) {
            MWSFSVM_Error(lbl_eu_8051A3CC + 0x85);
            return 0;
        }
    }

    mwsfcre_CalcWorkStmBuf(self, &out[0], &out[1], &out[2], &out[3], &out[4], &out[5]);

    if (mwsfcre_IsOuterFrmPoolUsed(self) == 1) {
        /* Frames come from the caller-supplied pool: no library allocation. */
        siz[1] = 0;
        siz[0] = 0;
    } else {
        /* Width/frame-count cache lives in callee-saved regs in retail:
         * the loads sit before the mode check, so they stay live across the
         * MWSFSVM_Error call below. */
        s32 h;
        s32 w;
        frames = self->frameCount;
        w = self->width;
        h = self->height;
        if (self->field_0x24 > 3) {
            MWSFSVM_Error(lbl_eu_8051A3CC);
        }
        s32 w16 = (w + 15) / 16;
        s32 h16 = (h + 15) / 16;
        s32 W = w16 << 4;
        s32 H = h16 << 4;
        s32 luma = ((w16 >> 27) & 1) + W;
        s32 chroma = ((h16 >> 27) & 1) + H;
        /* One Y frame + double-pitch chroma frame; the +0x20 header lands in
         * a separate variable so it gets its own register (retail shape). */
        s32 blkW = (((W + 0x1f) / 32) * 32);
        s32 frameSize = ((chroma >> 1) * ((((luma >> 1) + 0x1f) / 32) * 32)) * 2 +
                        H * blkW;
        s32 unit = frameSize + 0x20;
        siz[0] = frames * unit;
        siz[1] = unit * 2;
    }

    /* Retail shape: accumulator over (out0..szFrames,out4), a separate
     * (out1+out2+out3) subtree, then out5 and the 0x5060 header constant. */
    total = out[0] + siz[1] + siz[0] + out[4];
    half = out[1] + out[2] + out[3];
    total = half + total;
    total = out[5] + total;
    result = total + 0x5060;

    sfxSize = MWSFSFX_CalcHnWorkSiz(self->width, self->height);
    if (MWSFTAG_IsUseAinfSj(self) == 1) {
        sfxSize += 0x20000;
    }
    return result + sfxSize;
}

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
    /* Volatile forces MWCC to keep the retail store order (50c before 510). */
    *(volatile u32*)&self->field_0x50c = lbl_eu_805FF2E8 - 0x800;
    *(volatile u32*)&self->field_0x510 = 0x800;
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

/* Creates the SFD decoder handle for one player slot: allocates every work
 * buffer from the player heap and fills the SFD_Create parameter block.
 * Returns NULL (after releasing the heap) on any allocation/config failure. */
void* mwsfcre_CreateSfd(MWSFDPLY* ply, MWSFCRE_Para* para) {
    SfdWork* w = &lbl_eu_805FF2E0;
    SfdCreateCfg cfg;
    void* rbfOut[2];
    void* tabOut[16];
    u32 frames = para->frameCount;
    s32 width = para->width;
    s32 height = para->height;
    s32 rbfRc;
    s32 tabRc;
    void *ex0, *ex1;
    void *stmBuf, *adxtBuf, *buf800, *buf4000, *buf4e0, *buf100b, *buf120;
    u32* buf100;
    void* sfd;

    /* Stream-buffer sizing; results land in the shared scratch block. */
    mwsfcre_CalcWorkStmBuf(para, &w->outA, &w->outB,
        &w->outC, &w->outD, &w->outE, &w->outF);

    if (mwsfcre_IsOuterFrmPoolUsed((MWSFDPLY*)para) == 1) {
        s32 mode = para->mode;
        u32 cnt = para->frameCount;
        s32 pw = para->width;
        s32 ph = para->height;
        if ((u32)mode > 3) {
            MWSFSVM_Error(lbl_eu_8051A3CC);
        }
        /* Round up to macroblocks; one frame slot carries a 0x20 header. */
        s32 w16 = (pw + 15) / 16;
        s32 h16 = (ph + 15) / 16;
        s32 W = w16 << 4;
        s32 H = h16 << 4;
        s32 lh = (((w16 >> 4) & 1) + W) >> 1;
        s32 ch = (((h16 >> 4) & 1) + H) >> 1;
        s32 unit = H * (((W + 0x1f) / 32) * 32) +
                   ch * (((lh + 0x1f) / 32) * 32) * 2 + 0x20;
        w->size0 = unit * 2;
        w->size1 = cnt * unit;
    } else {
        w->size0 = 0;
        w->size1 = 0;
    }

    stmBuf = criware_803A3AE4(ply,
        w->outB + w->outC + w->outD + 0x20);
    adxtBuf = criware_803A3AE4(ply, w->outA + 0x40);
    rbfRc = mwsfcre_MallocRfb(ply, para, rbfOut);
    tabRc = mwsfcre_MallocTab(ply, para, tabOut);

    /* Formats 2/3/6/7/8/0xa pull frames from the caller's outer pool, so the
     * two extra work buffers are skipped. */
    if ((u32)(para->fmt - 6) <= 2 || (u32)(para->fmt - 2) <= 1 || para->fmt == 10) {
        ex0 = NULL;
        ex1 = NULL;
    } else {
        ex0 = criware_803A3AE4(ply, w->outE);
        ex1 = criware_803A3AE4(ply, w->outF);
    }

    buf800 = criware_803A3AE4(ply, 0x800);
    buf4000 = criware_803A3AE4(ply, 0x4000);
    buf100 = (u32*)criware_803A3AE4(ply, 0x100);
    buf4e0 = criware_803A3AE4(ply, 0x4e0);
    buf100b = criware_803A3AE4(ply, 0x100);
    ply->field_0x52c = 0x120;
    buf120 = criware_803A3AE4(ply, 0x120);
    ply->field_0x528 = (u32)buf120;

    if (stmBuf == NULL || adxtBuf == NULL || rbfRc != 0 || tabRc != 0 ||
        buf4000 == NULL || buf100 == NULL || buf800 == NULL ||
        buf100b == NULL || buf120 == NULL || buf4e0 == NULL) {
        MWSFSVM_Error(lbl_eu_8051A3CC + 0x1b2);
        criware_803A3A48(ply);
        return NULL;
    }
    if (!((u32)(para->fmt - 6) <= 2 || (u32)(para->fmt - 2) <= 1 || para->fmt == 10) &&
        (ex0 == NULL || ex1 == NULL)) {
        MWSFSVM_Error(lbl_eu_8051A3CC + 0x1cb);
        criware_803A3A48(ply);
        return NULL;
    }

    /* Publish the geometry the MPV decoder works from. */
    w->field_0x04 = ((u32)adxtBuf + 0x3f) & ~0x3fu;
    {
        s32 vw = para->width;
        s32 vh = para->height;
        lbl_eu_80567258.height = vh;
        lbl_eu_80567258.width = vw;
        lbl_eu_80567258.field_0x10 = 0;
        lbl_eu_80567258.yStride = ((vw / 2 + 0x1f) / 32) * 32;
        lbl_eu_80567258.chromaHeight = vh / 2;
        lbl_eu_80567258.width2 = vw;
        lbl_eu_80567258.height2 = vh;
        lbl_eu_80567258.frameCount = frames;
        lbl_eu_80567258.field_0x20 = 0;
    }
    lbl_eu_8056727C.field_0x08 = (u32)ex0;
    lbl_eu_8056727C.field_0x18 = (u32)ex1;
    criware_eu_803A29E0(para->fmt, (u32)para->field_0x40,
        (SfdCondTbl*)&cfg, ply);

    cfg.field_0x2c = lbl_eu_805660D0;
    if (w->outB != 0) {
        w->outB = w->outB - w->outB % lbl_eu_805660D0;
    }

    switch (para->mode) {
    case 0:
    case 3:
        cfg.modeIdx = 3;
        break;
    case 1:
        cfg.modeIdx = 1;
        break;
    case 2:
        cfg.modeIdx = 2;
        break;
    default:
        MWSFSVM_Error(lbl_eu_8051A3CC);
        cfg.modeIdx = 3;
        break;
    }

    cfg.stmBuf = stmBuf;
    cfg.field_0x08 = w->outB;
    cfg.field_0x0c = w->outC;
    cfg.field_0x10 = w->outD;
    cfg.field_0x28 = w->outA;
    cfg.field_0x30 = frames;
    cfg.field_0x34 = para->field_0x34;
    cfg.width = width;
    cfg.height = height;
    cfg.field_0x44 = (u32)buf4000;
    cfg.field_0x48 = 0x4000;
    cfg.field_0x4c = 0;

    SFD_SetMpvParaTbl((u32*)&lbl_eu_80567258, (u32*)rbfOut, (u32*)tabOut);
    if (para->fmt == 1 || para->fmt == 9) {
        SFD_SetAdxtPara(&lbl_eu_8056727C);
    }

    sfd = SFD_Create(&cfg, 0);
    if (sfd == NULL) {
        MWSFLIB_SetErrCode(-0x131);
        MWSFSVM_Error(lbl_eu_8051A3CC + 0x1e4);
        criware_803A3A48(ply);
        return NULL;
    }
    if (criware_803C0D94(sfd, MWSFLIB_SfdErrFunc, (u32)ply)) {
        MWSFLIB_SetErrCode(-0x12f);
        MWSFSVM_Error(lbl_eu_8051A3CC + 0x20f);
        criware_803A3A48(ply);
        return NULL;
    }

    *(u32*)buf100 = 1;
    ply->field_0x634 = (u32*)buf100;
    if (buf100 == NULL) {
        MWSFSVM_Error(lbl_eu_8051A3CC + 0x237);
        criware_803A3A48(ply);
        return NULL;
    }

    ply->field_0x4e8 = buf100b;
    ply->field_0x4ec = 0x100;
    ply->field_0x4a4 = (u32)buf800;
    ply->field_0x4a8 = 0x800;
    ply->field_0x4ac = 0x40;
    ply->picUsrTgt = &ply->field_0x4a4;
    ply->field_0x638 = 0;
    ply->field_0x678 = 0;
    ply->field_0x67c = 0;
    ply->field_0x680 = 0;
    return sfd;
}

void MWSFCRE_SetCondSfd(MWSFDPLY* ply, MWSFDPLY* user, u32 a, u32 b) {
    void* sfd = ply->sfd;
    SFD_SetCond(sfd, 8, 0);
    SFD_SetCond(sfd, 1, 1);
    SFD_SetCond(sfd, 0, 0);

    /* Single-precision compare: if the float addition rounded up past the
     * true integer value, walk the truncation back down by 1. The expression
     * is intentionally re-written at each use (no temp var): MWCC must not
     * CSE the four conversions. */
    s32 ms = a * b * 1000;
    s32 n;
    if ((f32)(s32)(lbl_eu_8051A3C8 + ms) > lbl_eu_8051A3C8 + ms) {
        n = (s32)(lbl_eu_8051A3C8 + ms) - 1;
    } else {
        n = (s32)(lbl_eu_8051A3C8 + ms);
    }

    SFD_SetCond(sfd, 0x2d, n);
    SFD_SetCond(sfd, 0x2c, n);
    SFD_SetCond(sfd, 0x2a, n);
    SFD_SetCond(sfd, 0xf, 2);
    SFD_SetCond(sfd, 0x33, 0);
    SFD_SetCond(sfd, 0xe, 0);
    SFD_SetCond(sfd, 0x1c, 0);
    SFD_SetMpvCond(sfd, 5, 0);
}

#pragma push
#pragma auto_inline off
/* Creates a full movie player in a free library slot: allocates the slot,
 * copies the user parameters in, then brings up the SFD core, ring buffers,
 * stream/sound handlers and tag/SFX layers. */
void* mwPlyCreateSofdec(MWSFDPLY* self) {
    u8* lib;
    MWSFDPLY* slots;
    MWSFDPLY* ply;
    void* sfd;
    void* stm;
    s32 slot;
    u32 outs[6];
    u32 hdr;
    s32 i;

    if (self == NULL) {
        MWSFSVM_Error(lbl_eu_8051A3CC + 0x3c0);
        return NULL;
    }
    {
        s32 ok = 1;
        if (self->field_0x24 != 0 && self->field_0x24 != 3) {
            MWSFSVM_Error(lbl_eu_8051A3CC + 0x3eb);
            ok = 0;
        }
        if (self->frameCount > 0xe) {
            MWSFSVM_Error(lbl_eu_8051A3CC + 0x40e);
            ok = 0;
        }
        if (ok != 1) {
            return NULL;
        }
    }

    lib = (u8*)MWSFLIB_GetLibWorkPtr();
    slots = (MWSFDPLY*)(lib + 0x70);
    slot = 0;
    if (slots[0].enable == 1) {
        slot = 1;
        while (slot < 8 && slots[slot].enable == 1) {
            slot++;
        }
    }
    if (slot == 8) {
        MWSFLIB_SetErrCode(-0xb);
        MWSFSVM_Error(lbl_eu_8051A3CC + 0x438);
        return NULL;
    }

    lib = (u8*)MWSFLIB_GetLibWorkPtr();
    if (self->field_0x18 == 0) {
        s32 ok = 0;
        if (*(u32*)(lib + 0x28) == 0) {
            ok = -1;
        }
        if (*(u32*)(lib + 0x2c) == 0) {
            ok = -1;
        }
        if (ok == -1) {
            MWSFSVM_Error(lbl_eu_8051A3CC + 0x488);
            return NULL;
        }
    }

    ply = &slots[slot];
    memset(ply, 0, 0x690);
    ply->field_0x540 = self->field_0x18;
    ply->field_0x544 = self->field_0x1c;
    ply->field_0x548 = self->field_0x18;
    ply->field_0x54c = 0;
    criware_803A3B98(ply);
    ply->field_0x558 = 0;
    for (i = 0; i < 31; i++) {
        ply->zeros[i] = 0;
    }
    ply->cfg = self->cfg;
    if (mwsfcre_IsOuterFrmPoolUsed(self) == 1) {
        ply->poolCount = ply->poolCount - 2;
    }

    sfd = mwsfcre_CreateSfd(ply, (MWSFCRE_Para*)&ply->width);
    ply->sfd = sfd;
    if (sfd == NULL) {
        MWSFSVM_Error(lbl_eu_8051A3CC + 0x4c1);
        fn_803A537C(ply);
        if (lbl_eu_805FF3A0 != NULL) {
            lbl_eu_80566280.self = (u32)ply;
            lbl_eu_805FF3A0->vtable->trace(lbl_eu_805FF3A0, &lbl_eu_80566280.field_0x04);
        }
        mwply_Destroy(ply);
        if (lbl_eu_805FF3A0 != NULL) {
            lbl_eu_805FF3A0->vtable->trace(lbl_eu_805FF3A0, &lbl_eu_80566280.field_0x6c);
        }
        (*(u32*)(lib + 0x34f4))--;
        return NULL;
    }

    mwsfcre_AttachPicUsrBuf(ply);
    mwsfcre_CalcWorkStmBuf(self, &outs[0], &outs[1], &outs[2], &outs[3],
        &outs[4], &outs[5]);
    MWSFCRE_SetCondSfd(ply, self, *(u32*)(lib + 0x8), *(u32*)(lib + 0xc));

    ply->rbf = SJRBF_Create(ply->field_0x508, ply->field_0x50c, ply->field_0x510);
    if (ply->rbf == NULL) {
        MWSFSVM_Error(lbl_eu_8051A3CC + 0x4e4);
        fn_803A537C(ply);
        if (lbl_eu_805FF3A0 != NULL) {
            lbl_eu_80566280.self = (u32)ply;
            lbl_eu_805FF3A0->vtable->trace(lbl_eu_805FF3A0, &lbl_eu_80566280.field_0x04);
        }
        mwply_Destroy(ply);
        if (lbl_eu_805FF3A0 != NULL) {
            lbl_eu_805FF3A0->vtable->trace(lbl_eu_805FF3A0, &lbl_eu_80566280.field_0x6c);
        }
        (*(u32*)(lib + 0x34f4))--;
        return NULL;
    }

    ply->uni = SJUNI_Create(1, ply->field_0x528, ply->field_0x52c);
    if (ply->uni == NULL) {
        MWSFSVM_Error(lbl_eu_8051A3CC + 0x506);
        fn_803A537C(ply);
        if (lbl_eu_805FF3A0 != NULL) {
            lbl_eu_80566280.self = (u32)ply;
            lbl_eu_805FF3A0->vtable->trace(lbl_eu_805FF3A0, &lbl_eu_80566280.field_0x04);
        }
        mwply_Destroy(ply);
        if (lbl_eu_805FF3A0 != NULL) {
            lbl_eu_805FF3A0->vtable->trace(lbl_eu_805FF3A0, &lbl_eu_80566280.field_0x6c);
        }
        (*(u32*)(lib + 0x34f4))--;
        return NULL;
    }

    ply->field_0x74 = outs[0];
    ply->field_0x54 = 1;
    ply->field_0x84 = 0;
    ply->field_0x68 = self->field_0x20;
    ply->field_0x6c = self->field_0x20;
    if (SFD_GetSofdecHeader(sfd, 3, &hdr)) {
        ply->field_0x60 = 0;
    } else {
        ply->field_0x60 = hdr;
    }
    ply->field_0x88 = 1;
    ply->field_0x8c = 1;
    ply->field_0x90[0] = 0;
    ply->field_0x90[1] = 0;
    ply->field_0x90[2] = 0;
    ply->field_0x90[3] = 0;
    ply->field_0x78 = 0;
    MWSFSVR_SetHnMwplySvrFlg(ply, 1);
    MWSFSVR_SetHnSfdSvrFlg(ply, 0);
    ply->field_0x70 = 1;
    ply->field_0xa4 = 0;

    stm = MWSTM_Create(ply->rbf);
    ply->stm = stm;
    if (stm == NULL) {
        fn_803A537C(ply);
        if (lbl_eu_805FF3A0 != NULL) {
            lbl_eu_80566280.self = (u32)ply;
            lbl_eu_805FF3A0->vtable->trace(lbl_eu_805FF3A0, &lbl_eu_80566280.field_0x04);
        }
        mwply_Destroy(ply);
        if (lbl_eu_805FF3A0 != NULL) {
            lbl_eu_805FF3A0->vtable->trace(lbl_eu_805FF3A0, &lbl_eu_80566280.field_0x6c);
        }
        (*(u32*)(lib + 0x34f4))--;
        return NULL;
    }

    MWSFPLY_SetFlowLimit(ply);
    ply->lsc = LSC_Create(ply->rbf);
    ply->field_0x94 = 0;
    LSC_SetStmHndl(ply->lsc, ply->stm);
    if (mwsfcre_MallocCompoWork(ply) == -1) {
        fn_803A537C(ply);
        if (lbl_eu_805FF3A0 != NULL) {
            lbl_eu_80566280.self = (u32)ply;
            lbl_eu_805FF3A0->vtable->trace(lbl_eu_805FF3A0, &lbl_eu_80566280.field_0x04);
        }
        mwply_Destroy(ply);
        if (lbl_eu_805FF3A0 != NULL) {
            lbl_eu_805FF3A0->vtable->trace(lbl_eu_805FF3A0, &lbl_eu_80566280.field_0x6c);
        }
        (*(u32*)(lib + 0x34f4))--;
        return NULL;
    }

    ply->sfx = MWSFSFX_Create(ply->compoWork, ply->compoWorkSize,
        self->width, self->height);
    if (ply->sfx == NULL) {
        MWSFSVM_Error(lbl_eu_8051A3CC + 0x52f);
        fn_803A537C(ply);
        if (lbl_eu_805FF3A0 != NULL) {
            lbl_eu_80566280.self = (u32)ply;
            lbl_eu_805FF3A0->vtable->trace(lbl_eu_805FF3A0, &lbl_eu_80566280.field_0x04);
        }
        mwply_Destroy(ply);
        if (lbl_eu_805FF3A0 != NULL) {
            lbl_eu_805FF3A0->vtable->trace(lbl_eu_805FF3A0, &lbl_eu_80566280.field_0x6c);
        }
        (*(u32*)(lib + 0x34f4))--;
        return NULL;
    }
    MWSFSFX_SetCompoMode(ply, ply->poolCount);

    ply->field_0x4c0 = MWSFTAG_CreateAinfSj(ply);
    if (MWSFTAG_SetAinfSj(ply)) {
        MWSFSVM_Error(lbl_eu_8051A3CC + 0x54a);
        fn_803A537C(ply);
        if (lbl_eu_805FF3A0 != NULL) {
            lbl_eu_80566280.self = (u32)ply;
            lbl_eu_805FF3A0->vtable->trace(lbl_eu_805FF3A0, &lbl_eu_80566280.field_0x04);
        }
        mwply_Destroy(ply);
        if (lbl_eu_805FF3A0 != NULL) {
            lbl_eu_805FF3A0->vtable->trace(lbl_eu_805FF3A0, &lbl_eu_80566280.field_0x6c);
        }
        (*(u32*)(lib + 0x34f4))--;
        return NULL;
    }

    MWSFTAG_InitTagInf(ply);
    MWSFFRM_InitSfhInfTable(ply);
    MWSFFRM_SetShfCbFn(ply);
    ply->enable = 1;
    ply->field_0x64c = 0;
    ply->field_0x63c = 0;
    ply->field_0x640 = 0;
    ply->field_0x644 = 0;
    ply->field_0x664 = 0;
    ply->field_0x660 = -1;
    ply->field_0x668 = 0;
    ply->field_0x66c = 0;
    ply->field_0x670 = 0;
    ply->field_0x674 = 0;
    (*(u32*)(lib + 0x34f4))++;
    return ply;
}
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
