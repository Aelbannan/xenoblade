#pragma once

#include <types.h>

/*
 * Layout of the mwPly (MWSFD) movie-player handle, shared by the mwply
 * modules.  One handle is 0x690 bytes; the player pool (mwsfdsvr) carries a
 * slot per open stream.  Offsets come from the retail access patterns of the
 * units that share the object (mwsfdcre, mwsfdfrm, mwsfdsfx, ...); members
 * whose meaning is still unknown keep the `field_0x....` spelling with their
 * offset recorded.
 */

/* Per-frame FX conversion record at handle+0xE0 (8 of them, stride 0x38).
 * The SFD stream header selects one per multiplexed frame; mwsfdfrm turns it
 * into the frame information and mwsfdsfx reads the FX type and the column /
 * field adjustment flags from it. */
typedef struct MwlSfdFrame {
    s32 state;             /* 0x00 1 = entry in use */
    u8  field_0x04[0x0c];  /* 0x04 */
    s32 picW;              /* 0x10 picture width of the frame */
    s32 picH;              /* 0x14 picture height of the frame */
    s32 colAdj;            /* 0x18 column adjustment supported */
    s32 colAdjFile;        /* 0x1c column adjustment of the file */
    s32 field_0x20;        /* 0x20 */
    s32 fxType;            /* 0x24 FX conversion type (mwPlyFxCnvFrm*) */
    u8  field_0x28[0x10];  /* 0x28 */
} MwlSfdFrame;             /* 0x38 */

/* Picture user buffer descriptor (handle+0x4a4): the caller's buffer, its
 * byte size and the line count it holds.  mwPlyAttachPicUsrBuf() validates the
 * size against the height, MWSFCRE_AttachPicUsrBuf() hands it to the SFD
 * core. */
typedef struct MwsfPicUsrBuf {
    void* buf;      /* 0x00 */
    s32 size;       /* 0x04 */
    s32 count;      /* 0x08 */
} MwsfPicUsrBuf;

/* One converted frame-information record (0xA8 bytes).  mwsfdfrm builds it
 * from the SFD stream header (mwl_convFrmInfFromSFD) and hands it to the SFX
 * layer, which reads the frame slot at +0x38 and the SUD data pointer/size at
 * +0xA0/+0xA4. */
typedef struct MwlSfdFrmInf {
    s32 field_0x00;         /* 0x00 */
    s32 field_0x04;         /* 0x04 */
    s32 field_0x08;         /* 0x08 */
    s32 field_0x0c;         /* 0x0c */
    s32 field_0x10;         /* 0x10 */
    s32 field_0x14;         /* 0x14 */
    s32 field_0x18;         /* 0x18 */
    s32 field_0x1c;         /* 0x1c */
    s32 field_0x20;         /* 0x20 */
    s32 field_0x24;         /* 0x24 */
    s32 field_0x28;         /* 0x28 */
    s32 field_0x2c;         /* 0x2c */
    s32 field_0x30;         /* 0x30 */
    s32 field_0x34;         /* 0x34 */
    s32 field_0x38;         /* 0x38 frame slot index */
    s32 field_0x3c;         /* 0x3c */
    s32 field_0x40;         /* 0x40 */
    s32 field_0x44;         /* 0x44 */
    s32 field_0x48;         /* 0x48 */
    s32 field_0x4c;         /* 0x4c */
    s32 field_0x50;         /* 0x50 */
    s32 field_0x54;         /* 0x54 */
    u8  _58[0x10];          /* 0x58..0x67 */
    u8  field_0x68[0x38];   /* 0x68..0x9f */
    u32 field_0xa0;         /* 0xa0 SUD data pointer */
    u32 field_0xa4;         /* 0xa4 SUD data size */
} MwlSfdFrmInf;             /* 0xa8 */

/* One 16-byte entry in the 32-slot stream description table at handle+0x2A4:
 * whether the stream is present, its channel mode, sampling rate and stream id. */
typedef struct MwsfSndInf {
    s32 used;       /* 0x00 1 = stream in use */
    s32 ch;         /* 0x04 channel mode */
    s32 rate;       /* 0x08 sampling rate */
    s32 stmId;      /* 0x0c stream id */
} MwsfSndInf;       /* 0x10 */

/* Sub-object at handle+0x5D8 and +0x600: state of one stereo stream. */
typedef struct MwsfSstHn {
    s32 state;             /* 0x00 1 = enabled */
    u8  field_0x04[0x14];  /* 0x04..0x17 */
    s32 enable;            /* 0x18 1 = channel selection valid */
    s32 ch;                /* 0x1c selected channel */
    u8  field_0x20[0x08];  /* 0x20..0x27 */
} MwsfSstHn;               /* 0x28 */

typedef struct HnObjVtable {
    u8 pad_0x00[0x0C];
    void (*destroy)(void* self);
} HnObjVtable;
typedef struct HnObj {
    HnObjVtable* vtable;
} HnObj;

/* Caller parameter block for player creation. Passed as-is to
 * mwPlyCreateSofdec and embedded (copied wholesale) in the player handle at
 * offset 4. */
typedef struct PlyCprm {
    u32 field_0x00;         /* 0x00 */
    u32 field_0x04;         /* 0x04 */
    s32 width;              /* 0x08 */
    s32 height;             /* 0x0c */
    s32 frameCount;         /* 0x10 */
    u32 field_0x14;         /* 0x14 */
    s32 field_0x18;         /* 0x18 */
    u32 field_0x1c;         /* 0x1c */
    u32 field_0x20;         /* 0x20 */
    s32 mode;               /* 0x24 */
    s32 poolCount;          /* 0x28 */
    s32 poolSize;           /* 0x2c */
    void** pool;            /* 0x30 */
    u8 pad_0x34[0x18];      /* 0x34..0x4b */
} PlyCprm;

/* MWSFDPLY movie-player handle (one slot per 0x690 bytes). Partial layout
 * covering the fields touched by the creator helpers in this TU. */
typedef struct MWSFDPLY {
    s32 enable;             /* 0x000 */
    PlyCprm prm;            /* 0x004..0x04f: copied-in parameter image */
    u8 pad_0x50[0x04];      /* 0x050..0x053 */
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
    u8 field_0xd4[0x04];    /* 0x0d4..0x0d7 */
    s32 frmIdx;             /* 0x0d8 index of the frame being converted */
    u8 field_0xdc[0x04];    /* 0x0dc..0x0df */
    MwlSfdFrame frmFx[8];   /* 0x0e0 per-frame FX conversion records */
    s32 audInfEnable;       /* 0x2a0 1 = stream description table is ready */
    MwsfSndInf audInf[32];  /* 0x2a4..0x4a3 32 stream descriptions */
    MwsfPicUsrBuf picUsr;   /* 0x4a4 picture user buffer description */
    u8 pad_0x4b0[0x0c];     /* 0x4b0..0x4bb */
    void* picUsrTgt;        /* 0x4bc */
    HnObj* ainfSj;          /* 0x4c0 AddIn file SJ object */
    void* ainfBuf;          /* 0x4c4 AddIn file SJ work buffer */
    u32 ainfBufSize;        /* 0x4c8 its byte size */
    void* tagBuf;           /* 0x4cc CRITAGS copy destination */
    u8 field_0x4d0[0x04];   /* 0x4d0 */
    s32 tagState;           /* 0x4d4 -1 until MWSFTAG_InitTagInf */
    s32 tagValid;           /* 0x4d8 1 = tag information available */
    u32 tagData;            /* 0x4dc tag data pointer */
    u32 tagSize;            /* 0x4e0 tag data size */
    u8 field_0x4e4[0x04];   /* 0x4e4 */
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
    MwsfSstHn sst[2];       /* 0x5d8 stereo stream sub-objects */
    u8 field_0x628[0x0C];   /* 0x628..0x633 */
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
    s32 field_0x684;        /* 0x684 copied picture width */
    s32 field_0x688;        /* 0x688 frame count */
    s32 field_0x68c;        /* 0x68c computed frame duration */
} MWSFDPLY;             /* 0x690 */
