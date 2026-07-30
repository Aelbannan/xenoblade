#include <harness_catalog.h>
#include <string.h>

extern int UTY_MemcpyDword(u32 *, const u32 *, int);
extern void MPVABDEC_Init(void *);

extern u32 lbl_eu_8051BFC0[];  /* reference tables */
extern u32 lbl_eu_80602A10[];  /* work area */
extern u32 lbl_eu_80604660[];
extern u32 lbl_eu_8060465C[];
extern u32 lbl_eu_80604658[];
extern u32 lbl_eu_80604654[];
extern u32 lbl_eu_80604650[];
extern u32 lbl_eu_8060464C[];

extern u32 lbl_eu_806046A8[];
extern u32 lbl_eu_806033F8[];
extern u32 lbl_eu_80603480[];
extern u32 lbl_eu_80604644[];
extern u32 lbl_eu_80604648[];

/* Initialize MPEG video bitstream decoder */
void MPVBDEC_Init(void *handle) {
    u8 table[0x40];
    int i, j;
    u8 *tmp;
    u32 *src_words;
    u32 *dst_words;
    u32 offset;

    /* Build 64-byte lookup table on stack: values 0..63 in byte order */
    for (i = 0; i < 64; i++)
        table[i] = (u8)i;

    /* Permute and copy to handle+0x1120 using lookup tables */
    {
        u8 *perm = (u8 *)&lbl_eu_8051BFC0[8];    /* +0x20 */
        u8 *val_tbl = (u8 *)&lbl_eu_8051BFC0[24]; /* +0x60 */
        u32 *work = &lbl_eu_80602A10[0];           /* +0x00 */
        u32 *work2 = &lbl_eu_80602A10[16];         /* +0x40 */
        u32 *work3 = &lbl_eu_80602A10[19];         /* +0x4C */
        u32 *dst = (u32 *)((u8 *)handle + 0x1120);

        for (j = 0; j < 8; j++) {
            u8 *out = (u8 *)dst + j * 8;

            for (i = 0; i < 8; i++) {
                u8 p = perm[j * 8 + i];
                u8 v = val_tbl[j * 8 + i];
                out[i] = table[p];
                ((u8 *)work)[(s8)table[j * 8 + i]] = v;
            }
        }

        /* Store pointers to work area */
        work2[2] = (u32)((u8 *)handle + 0x1120); /* work2+8 */
        work3[2] = (u32)work;                     /* work3+8 */
    }

    /* Copy reference table to handle+0x1100 (8 dwords) */
    UTY_MemcpyDword((u32 *)((u8 *)handle + 0x1100),
        &lbl_eu_8051BFC0[0], 8);

    /* Copy another reference to handle+0x11E0 (32 bytes) */
    memcpy((u8 *)handle + 0x11E0,
        (u8 *)&lbl_eu_8051BFC0[40], 0x20);

    /* Set up decoder parameters at handle+0x1200..0x122C */
    {
        u32 *p = (u32 *)((u8 *)handle + 0x1200);
        p[0] = lbl_eu_80604660[0] - 0x10;  /* width */
        p[1] = 0x15;                        /* height? */
        p[2] = lbl_eu_8060465C[0] - 0x20;  /* something */
        p[3] = 0x13;
        p[4] = lbl_eu_80604658[0] - 0x20;
        p[5] = 0x12;
        p[6] = lbl_eu_80604654[0] - 0x20;
        p[7] = 0x11;
        p[8] = lbl_eu_80604650[0] - 0x20;
        p[9] = 0x10;
        p[10] = lbl_eu_8060464C[0] - 0x20;
        p[11] = 0x0F;
    }

    MPVABDEC_Init(handle);
}

/* Start decoding a frame */
void MPVBDEC_StartFrame(void *handle) {
    u32 *p = (u32 *)handle;
    u32 flag = p[0xD00 / 4];
    u32 *tbl = (u32 *)lbl_eu_806046A8;
    u32 *param_tbl = (u32 *)tbl[2];  /* tbl[2] = ptr to param array */

    if (flag == 8) {
        /* progressive frame - skip field setup */
    } else {
        /* interlaced - select field parameters */
        if (p[0xD38 / 4] == 0) {
            p[0xD3C / 4] = param_tbl[0];
        } else {
            p[0xD3C / 4] = param_tbl[1];
        }
        p[0xD40 / 4] = param_tbl[2];
        p[0xD44 / 4] = 0;
    }

    /* Set decode parameters */
    if (p[0xD38 / 4] == 0) {
        p[0xD4C / 4] = lbl_eu_806033F8[0];
        p[0xD50 / 4] = lbl_eu_80603480[0];
    } else {
        p[0xD4C / 4] = lbl_eu_80604644[0];
        p[0xD50 / 4] = lbl_eu_80604648[0];
    }
}
