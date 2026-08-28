// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/mpv/mpv_cdec
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
extern int mpvcdec_IntraBlocksInt1(void* self);
extern int mpvcdec_NintraBlocksInt1(void* self);
__declspec(section ".rodata") __attribute__((aligned(8))) const void* lbl_eu_8051C080[2] = { (void*)mpvcdec_IntraBlocksInt1, (void*)mpvcdec_NintraBlocksInt1 };

#include <string.h>

extern void fn_803A7770(void);
extern void fn_803BDF3C(void* a, s32 b);
extern void DCT_IsrInit(void);
extern void DCT_IsrInitScaleTbl(void* tbl);
extern volatile u32 lbl_eu_80602A68[];
extern volatile u32 lbl_eu_80602A6C[];

void MPVCDEC_Init(void* self) {
    u32* arr = (u32*)lbl_eu_80602A6C;
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
    *(u32*)((u8*)self + 0xc84) = (u32)DCT_IsrTrans;
    *(u32*)((u8*)self + 0x9b4) = lbl_eu_80602A68[0] + 0x1160;
    /* Table entry re-read for each store (matches retail's double load). */
    *(u32*)((u8*)self + 0xc68) = ((u32*)lbl_eu_80602A6C[2])[0];
    *(u32*)((u8*)self + 0xc6c) = ((volatile u32*)lbl_eu_80602A6C[2])[1];
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

/* Zero the two halves of the decoder work region (self+0 .. self+0x300).
 * Written pair-wise (odd word first) to match MWCC's fully-unrolled store
 * order; the base pointer advances mid-way so each half keeps its own
 * base register. */

#define Z2(a, i) do { a[(i)+1] = 0; a[i] = 0; } while (0)

/* Process intra blocks: zero the work region, build the block descriptor and
 * drive one decode/transform pass per macroblock type.
 */
int mpvcdec_IntraBlocksInt1(void* self) {
    MPVC_dec *dec = (MPVC_dec*)self;
    u32 *z = (u32*)self;
    u32 *z2 = (u32*)((u8*)self + 0x88);
    u32 h, r2, r3, r4, r5, r6;

    /* Clear the whole work region (0x300 bytes). */
    Z2(z, 0); Z2(z, 2); Z2(z, 4); Z2(z, 6); Z2(z, 8); Z2(z, 10);
    Z2(z, 12); Z2(z, 14); Z2(z, 16); Z2(z, 18); Z2(z, 20); Z2(z, 22);
    Z2(z, 24); Z2(z, 26); Z2(z, 28); Z2(z, 30); Z2(z, 32); Z2(z, 34);
    Z2(z, 36); Z2(z, 38); Z2(z, 40); Z2(z, 42); Z2(z, 44); Z2(z, 46);
    Z2(z, 48); Z2(z, 50); Z2(z, 52); Z2(z, 54); Z2(z, 56); Z2(z, 58);
    Z2(z, 60); Z2(z, 62); Z2(z, 64); Z2(z, 66);
    Z2(z2, 34); Z2(z2, 36); Z2(z2, 38); Z2(z2, 40); Z2(z2, 42);
    Z2(z2, 44); Z2(z2, 46); Z2(z2, 48); Z2(z2, 50); Z2(z2, 52);
    Z2(z2, 54); Z2(z2, 56); Z2(z2, 58); Z2(z2, 60); Z2(z2, 62);
    Z2(z2, 64); Z2(z2, 66); Z2(z2, 68); Z2(z2, 70); Z2(z2, 72);
    Z2(z2, 74); Z2(z2, 76); Z2(z2, 78); Z2(z2, 80); Z2(z2, 82);
    Z2(z2, 84); Z2(z2, 86); Z2(z2, 88); Z2(z2, 90); Z2(z2, 92);
    Z2(z2, 94); Z2(z2, 96); Z2(z2, 98); Z2(z2, 100); Z2(z2, 102);
    Z2(z2, 104); Z2(z2, 106); Z2(z2, 108); Z2(z2, 110); Z2(z2, 112);
    Z2(z2, 114); Z2(z2, 116); Z2(z2, 118); Z2(z2, 120); Z2(z2, 122);
    Z2(z2, 124); Z2(z2, 126); Z2(z2, 128); Z2(z2, 130); Z2(z2, 132);
    Z2(z2, 134); Z2(z2, 136); Z2(z2, 138); Z2(z2, 140); Z2(z2, 142);
    Z2(z2, 144); Z2(z2, 146); Z2(z2, 148); Z2(z2, 150); Z2(z2, 152);
    Z2(z2, 154); Z2(z2, 156);
#undef Z2

    /* Per-decode block descriptor at self+0x9c4 */
    {
    MPVC_Blk *blk = (MPVC_Blk*)((u8*)self + 0x9c4);
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
    }

    return 0;
}