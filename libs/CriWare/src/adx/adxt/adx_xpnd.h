#ifndef CRIWARE_ADX_XPND_H
#define CRIWARE_ADX_XPND_H

#include <types.h>

/*
 * One of the 16 ADXPD decoder objects.  ADXPD_Create() carves a slot out of
 * the module work array (0x3c bytes each, so the array is 16 * 60 bytes);
 * ADXB_EntryData() books a coded block into it, ADXPD_ExecHndl() runs the
 * ADPCM decoder over it and ADXB reads the block count and the two history
 * buffers back out.
 */
typedef struct ADXPD {
    u32  inUse;            /* 0x00 1 = slot in use */
    u32  index;            /* 0x04 slot index inside the work array */
    u32  field_0x08;       /* 0x08 */
    u32  stat;             /* 0x0c 0 idle, 1 booked, 2 decoding, 3 done */
    u32  numBlk;           /* 0x10 blocks decoded by the last ExecHndl */
    s32  mode;             /* 0x14 1 = mono, 2 = stereo */
    const u8* in;          /* 0x18 coded block */
    s32  blkNum;           /* 0x1c blocks to decode */
    s16* out1;             /* 0x20 first output buffer */
    s16* out2;             /* 0x24 second output buffer */
    s16  dly[2][2];        /* 0x28 previous-sample history per channel */
    s16  coef[2];          /* 0x30 ADPCM coefficients */
    s16  extPrm[3];        /* 0x34 decoder extension parameters */
    s16  field_0x3a;       /* 0x3a */
} ADXPD;                   /* 0x3c */

#endif
