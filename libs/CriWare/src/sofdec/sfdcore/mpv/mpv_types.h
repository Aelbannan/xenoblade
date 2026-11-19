#ifndef CRIWARE_SOFDEC_MPV_TYPES_H
#define CRIWARE_SOFDEC_MPV_TYPES_H

#include <types.h>

#include "libs/CriWare/src/sofdec/sfdcore/sfd/sfd_types.h"

/* ------------------------------------------------------------------------- */
/* Module work area                                                          */
/* ------------------------------------------------------------------------- */

/* The 0x60-byte MPV configuration block MPV_Init() fills in and every handle
 * copies at creation time (the copy lives at handle+0xB10, 0x40 bytes of the
 * 0x60 -- the last 0x20 bytes are the per-handle clip table pointer set).
 * Retail references this block through its own symbol, so it is a type of its
 * own and also embedded in MpvWork below. */
typedef struct MpvCfg {
    u8  _pad0x00[0x48];     /* 0x00 */
    u32 flushSw;            /* 0x48 non-zero = cache-flush the work blocks */
    u32 hnBase;             /* 0x4c handle block base */
    u32 vlcBase;            /* 0x50 VLC/decode block base */
    u32 hnNum;              /* 0x54 number of handler blocks */
    u32 memBase;            /* 0x58 aligned work-area base */
    u8  _pad0x5c[0x04];     /* 0x5c */
} MpvCfg;                   /* 0x60 */

/* The module work area starts at 0x80602B80: an 8-byte header, then the
 * configuration block, then the 0x400-byte clip table mpvlib_InitClip0255()
 * seeds.  MPV_Init() addresses all three through this one symbol. */
typedef struct MpvWork {
    u32 field_0x00;         /* 0x00 */
    u32 verPtr;             /* 0x04 version string pointer */
    MpvCfg cfg;             /* 0x08 configuration block (own symbol too) */
    u8  clipTbl[0x400];     /* 0x68 clip table (lbl_eu_80602BE8) */
    u32 clipBuf;            /* 0x468 clip working copy inside the VLC block */
} MpvWork;

/* ------------------------------------------------------------------------- */
/* Decoder handle                                                            */
/* ------------------------------------------------------------------------- */

/* One MPV decoder handle.  MPV_Init() hands the module a work area that is
 * carved into 0xDC0-byte blocks, one per stream; every block starts with
 * state == 1 (free) and MPV_Create() flips it to 2 (in use).  The picture
 * attribute and error records are the shared Sofdec ones because both are
 * handed to the application verbatim. */
#define MPV_HN_SIZE      0x0DC0
#define MPV_HN_STATE_FREE 1
#define MPV_HN_STATE_USED 2

/* One motion-compensation output-row descriptor: a plane base and its stride. */
typedef struct MpvMcRow {
    void* base;             /* +0x0 plane base */
    s32 stride;             /* +0x4 row stride */
} MpvMcRow;                 /* 0x8 */

typedef struct MpvHn {
    u8  _pad0x000[0x380];   /* 0x000 decoder state (mpv_dec.c owns it) */
    u8  mcWork[0x600];      /* 0x380 motion-compensation work area */
    u32 bitsHi;             /* 0x980 bit accumulator, high word */
    u32 bitsLo;             /* 0x984 bit accumulator, low word */
    s32 bitCnt;             /* 0x988 bit count in the accumulator */
    u8* bitPtr;             /* 0x98c word cursor of the coded stream */
    u32 vlcTbl[7];          /* 0x990 copied-in code-table addresses */
    u8  _pad0x9ac[0x04];    /* 0x9ac */
    u32 vlcBufA;            /* 0x9b0 VLC block + 0x1100 */
    u8  _pad0x9b4[0x04];    /* 0x9b4 */
    u32 vlcBufB;            /* 0x9b8 VLC block + 0x11e0 */
    u32 vlcBufC;            /* 0x9bc VLC block + 0x1200 */
    u32 clipTbl;            /* 0x9c0 clip table (0x180 = unity entry) */
    u8  _pad0x9c4[0xA10 - 0x9c4];
    u32 dctCntFwd;          /* 0xa10 forward DCT count (MPV_GetDctCnt) */
    u32 dctCntBwd;          /* 0xa14 backward DCT count */
    u8  _pad0xa18[0xA90 - 0xa18];
    u32 clipTblRef;         /* 0xa90 clip table again */
    struct MpvHn* hnSelf;   /* 0xa94 */
    u8* sub380;             /* 0xa98 handle + 0x380 */
    u8* sub500;             /* 0xa9c handle + 0x500 */
    u32 mcRowCount;         /* 0xaa0 motion-compensation output row count */
    s32 mcRows[12];         /* 0xaa4 output row bases/strides (six pairs) */
    u32 mcOiTaCount;        /* 0xad4 one-reference output row count */
    MpvMcRow mcOiTa[6];     /* 0xad8 one-reference output row descriptors */
    s32 state;              /* 0xb08 MPV_HN_STATE_* */
    u32 field_0xb0c;        /* 0xb0c */
    u32 cfgImg[0x10];       /* 0xb10 64-byte copy of the MpvCfg block */
    u32 mbVal;              /* 0xb50 value reloaded into 0xd48 by the decoder */
    void (*mbCb)(void*);    /* 0xb54 macroblock callback */
    void* mbCtx;            /* 0xb58 its context */
    SfdMpvPicAtr picAtr;    /* 0xb5c attributes of the current picture */
    u8  errInfo[0xC0C - 0xBDC]; /* 0xbdc MPVERR error record */
    s16 outStride;          /* 0xc0c output frame luma stride */
    s16 outChromaStride;    /* 0xc0e output frame chroma stride */
    u8  errInfoTail[0xCFC - 0xC10]; /* 0xc10 MPVERR error record tail */
    u8  _pad0xcfc[0xD0C - 0xcfc];
    u8* plane[6];           /* 0xd0c reference plane pointers handed to MC */
    u8  _pad0xd24[0xD38 - 0xd24];
    u32 field_0xd38;        /* 0xd38 */
    u8  _pad0xd3c[0xD54 - 0xd3c];
    void* m2v;              /* 0xd54 motion-vector helper (MPVM2V_Create) */
    u8  _pad0xd58[0xDA8 - 0xd58];
    u32 field_0xda8;        /* 0xda8 */
    u8  _pad0xdac[MPV_HN_SIZE - 0xdac];
} MpvHn;                    /* 0xDC0 */

#endif
