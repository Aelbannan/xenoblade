// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/mpv/mpv_cdec
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include <string.h>

extern void fn_803A7770(void);
extern void fn_803BDF3C(void* a, s32 b);
extern void DCT_IsrInit(void);
extern void DCT_IsrInitScaleTbl(void* tbl);
extern u32 lbl_eu_8051C080;
extern u32 lbl_eu_80602A68[];
extern u32 lbl_eu_80602A6C[];

void MPVCDEC_Init(void* self) {
    u32* arr = lbl_eu_80602A6C;
    u32 val = (u32)&lbl_eu_8051C080;
    lbl_eu_80602A68[0] = (u32)self;
    fn_803BDF3C((void*)fn_803A7770, 0);
    DCT_IsrInit();
    DCT_IsrInitScaleTbl((u8*)self + 0x1160);
    arr[2] = val;
}

void fn_803A7770(void) { MPVERR_SetCode(0); }

extern void DCT_IsrTrans(void* self, s32 val);

void MPVCDEC_StartFrame(void* self) {
    u8* base = (u8*)self;
    *(void**)(base + 0xc84) = DCT_IsrTrans;
    *(u32*)(base + 0x9b4) = lbl_eu_80602A68[0] + 0x1160;
    *(u32*)(base + 0xc68) = ((u32*)lbl_eu_80602A6C[2])[0];
    *(u32*)(base + 0xc6c) = ((u32*)lbl_eu_80602A6C[2])[1];
}

typedef struct MPVC_Blk {
    u8  unk_0[0x1c];   /* 0x9c4..0x9df */
    u32 fld_0x1c;      /* 0x9e0 */
    u32 fld_0x20;      /* 0x9e4 */
    u32 fld_0x24;      /* 0x9e8 */
    u32 fld_0x28;      /* 0x9ec */
    u32 fld_0x2c;      /* 0x9f0 */
    u32 fld_0x30;      /* 0x9f4 */
} MPVC_Blk;

typedef struct MPVC_dec {
    u8  unk_0[0xc84];                  /* 0x00..0xc83 (work region + blk) */
    void (*field_0xc84)(u32, u32);     /* 0xc84 */
    u8  unk_c88[4];                    /* 0xc88 */
    u32 field_0xc8c;                   /* 0xc8c */
    u8  unk_c90[0x5c];                 /* 0xc90..0xceb */
    u32 field_0xcec;                   /* 0xcec */
    u32 field_0xcf0;                   /* 0xcf0 */
    u32 field_0xcf4;                   /* 0xcf4 */
    u32 field_0xcf8;                   /* 0xcf8 */
    u8  unk_cfc[0x10];                 /* 0xcfc..0xd0b */
    u32 field_0xd0c[7];                /* 0xd0c..0xd27 */
    u8  unk_d28[0x14];                 /* 0xd28..0xd3b */
    u32 (*field_0xd3c)(void*, void*);  /* 0xd3c */
    u32 (*field_0xd40)(void*, void*);  /* 0xd40 */
    u8  unk_d44[8];                    /* 0xd44..0xd4b */
    u32 field_0xd4c;                   /* 0xd4c */
    u32 field_0xd50;                   /* 0xd50 */
} MPVC_dec;

/* Process intra blocks (non-zeroing flavour). `blk` (self+0x9c4) is a
 * per-decode block descriptor; field_0xd0c[] holds the referenced block
 * pointers for the 6 MPEG macroblock types.
 */
int mpvcdec_NintraBlocksInt1(void* self) {
    MPVC_Blk *blk = (MPVC_Blk*)((u8*)self + 0x9c4);
    MPVC_dec *dec = (MPVC_dec*)self;
    u32 results[6];
    s32 r, i;

    blk->fld_0x24 = dec->field_0xc8c;
    blk->fld_0x20 = (u32)((u8*)self + 0x340);
    blk->fld_0x30 = 1;
    r = (s32)(dec->field_0xcec << 2);

    for (i = 0; i < 6; i++) {
        if (r < 0) {
            blk->fld_0x1c = dec->field_0xd0c[i];
            results[i] = dec->field_0xd40((void*)dec, blk);
        }
        r <<= 1;
    }

    r = (s32)(dec->field_0xcec << 2);
    if (r < 0) dec->field_0xc84(dec->field_0xd0c[0], results[0]);
    r <<= 1;
    if (r < 0) dec->field_0xc84(dec->field_0xd0c[1], results[1]);
    r <<= 1;
    if (r < 0) dec->field_0xc84(dec->field_0xd0c[2], results[2]);
    r <<= 1;
    if (r < 0) dec->field_0xc84(dec->field_0xd0c[3], results[3]);
    r <<= 1;
    if (r < 0) dec->field_0xc84(dec->field_0xd0c[4], results[4]);
    r <<= 1;
    if (r < 0) dec->field_0xc84(dec->field_0xd0c[5], results[5]);

    return 0;
}

/* Process intra blocks: zero the work region, build the block descriptor and
 * drive one decode/transform pass per macroblock type.
 */
int mpvcdec_IntraBlocksInt1(void* self) {
    MPVC_dec *dec = (MPVC_dec*)self;
    MPVC_Blk *blk = (MPVC_Blk*)((u8*)self + 0x9c4);
    u32 h, r2, r3, r4, r5, r6;

    memset(self, 0, 0x300);

    blk->fld_0x1c = dec->field_0xd0c[0];
    blk->fld_0x20 = (u32)((u8*)self + 0x300);
    blk->fld_0x24 = dec->field_0xc8c;
    blk->fld_0x28 = (u32)((u8*)self + 0xcf0);
    blk->fld_0x2c = dec->field_0xd4c;
    blk->fld_0x30 = 0;
    h = dec->field_0xd3c(self, blk);

    blk->fld_0x1c = dec->field_0xd0c[1];
    r2 = dec->field_0xd3c(self, blk);

    blk->fld_0x1c = dec->field_0xd0c[2];
    r3 = dec->field_0xd3c(self, blk);

    blk->fld_0x1c = dec->field_0xd0c[3];
    r4 = dec->field_0xd3c(self, blk);

    blk->fld_0x2c = dec->field_0xd50;
    blk->fld_0x28 = (u32)((u8*)self + 0xcf4);
    blk->fld_0x1c = dec->field_0xd0c[4];
    r5 = dec->field_0xd3c(self, blk);

    blk->fld_0x28 = (u32)((u8*)self + 0xcf8);
    blk->fld_0x1c = dec->field_0xd0c[5];
    r6 = dec->field_0xd3c(self, blk);

    dec->field_0xc84(dec->field_0xd0c[0], h);
    dec->field_0xc84(dec->field_0xd0c[1], r2);
    dec->field_0xc84(dec->field_0xd0c[2], r3);
    dec->field_0xc84(dec->field_0xd0c[3], r4);
    dec->field_0xc84(dec->field_0xd0c[4], r5);
    dec->field_0xc84(dec->field_0xd0c[5], r6);

    return 0;
}