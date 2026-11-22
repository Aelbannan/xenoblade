#pragma once

#include <types.h>

/* SFD_Create parameter block.  mwsfdcre.c fills one and sfd_ply.c copies its
 * first 0x50 bytes into the player handle as the initial runtime config. */
typedef struct SfdCreateCfg {
    u32 field_0x00;         /* 0x00 */
    void* stmBuf;           /* 0x04 stream buffer, 32-byte aligned by sfply_InitHn */
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
    void* work;             /* 0x44 caller work area */
    u32 workSize;           /* 0x48 caller work area size */
    u32 field_0x4c;         /* 0x4c */
} SfdCreateCfg;             /* 0x50 */

/*
 * Internal work layouts of the Sofdec (SFD) movie player, as seen by the
 * sfdcore modules (sfmpv, sfmpvf, sflib, sftim, sfbuf, sftrn, sfsee, ...).
 *
 * Every offset below was derived from the retail access patterns of the units
 * that share these objects; the SFD handle is one 0x39b0-byte block that the
 * whole player indexes into.  Fields whose meaning is still unknown keep the
 * `field_0x....` spelling with their offset recorded.  SfdHandle is laid out
 * as the retail structure is: each sub-system's work area is a member at its
 * exact offset, and a work area that reaches over another sub-block (the SFTIM
 * context, which spans 0x0d98-0x1394 and contains the SFTST block) owns that
 * block as a nested member.
 */

/* ------------------------------------------------------------------------- */
/* Time-unit block (SFTIM ttu)                                               */
/* ------------------------------------------------------------------------- */

/* Per-frame/per-player time accumulator; SFTIM_InitTtu() clears one.  Also
 * embedded at handle+0x1098 and at frame+0x18. */
typedef struct SfdTimTtu {
    s32 field_0x00;        /* 0x00 */
    s32 field_0x04;        /* 0x04 */
    s32 field_0x08;        /* 0x08 */
    s32 field_0x0c;        /* 0x0c */
    s32 field_0x10;        /* 0x10 */
    s32 field_0x14;        /* 0x14 */
    s32 field_0x18;        /* 0x18 */
    s32 field_0x1c;        /* 0x1c */
    u16 field_0x20;        /* 0x20 */
    u16 field_0x22;        /* 0x22 */
    s32 field_0x24;        /* 0x24 init value */
    s32 field_0x28;        /* 0x28 one */
} SfdTimTtu;               /* 0x2c */

/* ------------------------------------------------------------------------- */
/* Frame records                                                             */
/* ------------------------------------------------------------------------- */

/* Application-visible frame information.  SFMPVF_GetRead() hands a pointer to
 * one of these to the caller of the movie API and SFMPVF_AddRead() gets the
 * same pointer back (sfmpv_SetFrmInf() fills in the picture fields). */
typedef struct SfdFrmInfo {
    u32 field_0x00;        /* 0x00 picture width */
    u32 field_0x04;        /* 0x04 picture height */
    u32 field_0x08;        /* 0x08 */
    u32 field_0x0c;        /* 0x0c */
    u32 field_0x10;        /* 0x10 picture type */
    s32 time;              /* 0x14 frame time */
    s32 timeUnit;          /* 0x18 frame time unit */
    s32 codecId;           /* 0x1c handle+0x40 */
    u32 field_0x20;        /* 0x20 picture buffer */
    s32 field_0x24;        /* 0x24 */
    s32 field_0x28;        /* 0x28 */
    s32 field_0x2c;        /* 0x2c picture order */
    s32 field_0x30;        /* 0x30 */
    s32 field_0x34;        /* 0x34 */
    s32 field_0x38;        /* 0x38 */
    s32 field_0x3c;        /* 0x3c */
    s32 field_0x40;        /* 0x40 */
    u8  field_0x44[0x04];  /* 0x44 */
    s32 field_0x48;        /* 0x48 */
    u8  field_0x4c[0x04];  /* 0x4c */
    s64 field_0x50;        /* 0x50 */
    s32 field_0x58;        /* 0x58 */
    s32 field_0x5c;        /* 0x5c */
    s32 field_0x60;        /* 0x60 */
    s32 field_0x64;        /* 0x64 */
    s16 field_0x68;        /* 0x68 */
    s16 field_0x6a;        /* 0x6a */
    u8  field_0x6c;        /* 0x6c */
    u8  field_0x6d;        /* 0x6d */
    u8  field_0x6e;        /* 0x6e */
    u8  field_0x6f;        /* 0x6f */
    u8  field_0x70;        /* 0x70 */
    u8  field_0x71;        /* 0x71 */
    u8  field_0x72;        /* 0x72 */
    u8  field_0x73;        /* 0x73 */
    u8  field_0x74;        /* 0x74 */
    u8  field_0x75;        /* 0x75 */
    u8  field_0x76;        /* 0x76 */
    u8  field_0x77;        /* 0x77 */
    u8  field_0x78;        /* 0x78 */
    u8  field_0x79;        /* 0x79 */
    u8  field_0x7a;        /* 0x7a */
    u8  field_0x7b[0x01];  /* 0x7b */
    s16 field_0x7c;        /* 0x7c */
    u16 field_0x7e;        /* 0x7e */
} SfdFrmInfo;              /* 0x80 */

/* ------------------------------------------------------------------------- */
/* MPV picture attribute record                                              */
/* ------------------------------------------------------------------------- */

/* The 0x80-byte "picture attribute" block the MPV decoder maintains per
 * picture.  Two places hold a copy: the decode session keeps the attributes of
 * the picture being decoded (SfdMpvSession::picAtr, filled by MPV_GetPicAtr())
 * and every frame record keeps the block the decoder wrote for it
 * (SfdFrame::picAtr, copied by MPV_DecodeFrmSj()).  Retail's
 * mpvlib_InitPicAtr() seeds the block one field at a time, which is where the
 * field widths below come from. */
typedef struct SfdMpvPicAtr {
    u32 width;             /* 0x00 picture width in pixels */
    u32 height;            /* 0x04 picture height in lines */
    u32 field_0x08;        /* 0x08 */
    u32 field_0x0c;        /* 0x0c */
    s32 rateIndex;         /* 0x10 index into the picture-rate table */
    s32 time;              /* 0x14 frame time */
    s32 type;              /* 0x18 picture type, 1 = I, 2 = P, 3 = B */
    s32 field_0x1c;        /* 0x1c */
    s32 field_0x20;        /* 0x20 */
    s32 field_0x24;        /* 0x24 */
    s32 field_0x28;        /* 0x28 */
    s32 field_0x2c;        /* 0x2c */
    s32 field_0x30;        /* 0x30 */
    s32 field_0x34;        /* 0x34 */
    s32 field_0x38;        /* 0x38 */
    s32 field_0x3c;        /* 0x3c */
    s32 field_0x40;        /* 0x40 */
    s32 field_0x44;        /* 0x44 */
    s32 field_0x48;        /* 0x48 */
    s32 field_0x4c;        /* 0x4c */
    s16 field_0x50;        /* 0x50 */
    s16 field_0x52;        /* 0x52 */
    s8  field_0x54;        /* 0x54 field id of the picture */
    s8  field_0x55;        /* 0x55 */
    s8  field_0x56;        /* 0x56 */
    s8  field_0x57;        /* 0x57 */
    s8  field_0x58;        /* 0x58 */
    u8  field_0x59;        /* 0x59 */
    u8  field_0x5a;        /* 0x5a */
    u8  field_0x5b;        /* 0x5b */
    u8  field_0x5c;        /* 0x5c */
    u8  field_0x5d;        /* 0x5d */
    s8  field_0x5e;        /* 0x5e */
    s8  field_0x5f;        /* 0x5f */
    s8  field_0x60;        /* 0x60 */
    u8  field_0x61;        /* 0x61 */
    u8  field_0x62;        /* 0x62 */
    u8  field_0x63;        /* 0x63 */
    u8  field_0x64;        /* 0x64 */
    u8  field_0x65[0x03];  /* 0x65 */
    s32 field_0x68;        /* 0x68 */
    u8  field_0x6c[0x14];  /* 0x6c */
} SfdMpvPicAtr;            /* 0x80 */

/* One entry of the 16-slot video frame (VFRM) queue that pairs every decoder
 * frame record with the picture information handed to the application.  The
 * queue is allocated/cleared by sfbuf_InitVfrmBuf(). */
typedef struct SfdVfrm {
    s32 state;             /* 0x00 1 = handed to the application */
    s32 field_0x04;        /* 0x04 */
    SfdFrmInfo info;       /* 0x08 */
} SfdVfrm;                 /* 0x88 */

/* Decoder frame record; the handle owns 16 of them. */
typedef struct SfdFrame {
    s32 state;             /* 0x00 0 free, 1 held, 2 standby, 3 read, 4 ref'd */
    s32 field_0x04;        /* 0x04 non-zero: record cannot be allocated */
    u8* picBuf;            /* 0x08 picture buffer handed to the decoder */
    u16 field_0x0c;        /* 0x0c */
    u16 field_0x0e;        /* 0x0e */
    u16 field_0x10;        /* 0x10 */
    u16 field_0x12;        /* 0x12 */
    s32 field_0x14;        /* 0x14 */
    SfdTimTtu ttu;         /* 0x18 time accumulator for this record */
    s32 time;              /* 0x44 */
    s32 timeUnit;          /* 0x48 */
    s32 field_0x4c;        /* 0x4c */
    s32 field_0x50;        /* 0x50 */
    s32 order;             /* 0x54 decode order */
    s32 field_0x58;        /* 0x58 */
    s32 field_0x5c;        /* 0x5c */
    s32 field_0x60;        /* 0x60 address of this record's picUsr band pair */
    s32 dispId;            /* 0x64 display counter, -1 = not displayed yet */
    SfdMpvPicAtr picAtr;   /* 0x68 picture attributes of the decoded picture */
    s64 field_0xe8;        /* 0xe8 */
    s32 field_0xF0;        /* 0xf0 */
    s32 field_0xF4;        /* 0xf4 */
    s32 field_0xF8;        /* 0xf8 */
    s32 field_0xFC;        /* 0xfc set once the picture was copied out */
    u16 field_0x100;       /* 0x100 */
    u8  field_0x102[0x0e]; /* 0x102 */
} SfdFrame;                /* 0x110 */

/* ------------------------------------------------------------------------- */
/* MPV decode session                                                        */
/* ------------------------------------------------------------------------- */

/* Picture-plane descriptor the frame-parameter block is built from: three
 * plane addresses plus the 32-line luma/chroma line sizes the decoder works
 * in.  SfdMpvSession keeps the two the current frame layout produces and
 * sfmpv_SetFrmPara() hands them to MPV_DecodeFrmSj() as-is unless the stream
 * carries two reference frames. */
typedef struct SfdMpvPlane {
    u32 field_0x00;        /* 0x00 plane base + luma bytes */
    u32 field_0x04;        /* 0x04 plane base + luma + chroma bytes */
    u32 base;              /* 0x08 picture plane base */
    s16 uvLineSiz;         /* 0x0c chroma line size, 32-line aligned */
    s16 lineSiz;           /* 0x0e luma line size, 32-line aligned */
} SfdMpvPlane;             /* 0x10 */

/* Frame-parameter block MPV_DecodeFrmSj() takes: the two plane descriptors,
 * the destination buffers and the byte-accounting outputs the caller adds to
 * the handle's DCT counters. */
typedef struct SfdMpvFrmPara {
    SfdMpvPlane plane[2];  /* 0x00 decoded plane geometry */
    u32 field_0x20[4];     /* 0x20 */
    u32 picBuf;            /* 0x30 decoder destination buffer */
    u32 picAtr;            /* 0x34 picture attribute destination */
    u32 decoded;           /* 0x38 bytes the decoder consumed */
    u32 field_0x3c;        /* 0x3c */
    s16 field_0x40;        /* 0x40 */
    u8  field_0x42[0x02];  /* 0x42 */
} SfdMpvFrmPara;           /* 0x44 */

/* Per-handle MPV decode session; SFMPV_Create() points transport 2's hdr
 * field at handle+0x2450 and every sfmpv_* module works through it. */
typedef struct SfdMpvSession {
    void* mpv;             /* 0x000 MPV decoder handle */
    s32 state;             /* 0x004 5 = idle, 2 = seeked, 3 = decoding */
    s32 pattern;           /* 0x008 delimiter pattern accepted next */
    s32 field_0x00c;       /* 0x00c */
    s32 frmSeq;            /* 0x010 frames handed out so far */
    SfdMpvPicAtr picAtr;   /* 0x014 attributes of the picture being decoded */
    s32 pts;               /* 0x094 presentation stamp of the last picture */
    s32 repFld;            /* 0x098 repeat-field flag of the last picture */
    s32 vbvLimit;          /* 0x09c bytes the VBV may hold */
    u32 ptsBase[2];        /* 0x0a0 stamp the picture run is measured from */
    u32 field_0x0a8;       /* 0x0a8 */
    u32 field_0x0ac;       /* 0x0ac */
    s32 field_0x0b0;       /* 0x0b0 */
    s32 field_0x0b4;       /* 0x0b4 */
    u32 field_0x0b8;       /* 0x0b8 */
    s32 field_0x0bc;       /* 0x0bc */
    s32 planeSelA;         /* 0x0c0 plane used for the top field */
    s32 planeSelB;         /* 0x0c4 plane used for the bottom field */
    SfdMpvPlane plane[2];  /* 0x0c8 plane geometry of the current layout */
    SfdFrame* refTerm;     /* 0x0e8 reference frame to release */
    SfdFrame* refPrev;     /* 0x0ec previous reference frame */
    SfdFrame* cur;         /* 0x0f0 frame the decoder is filling */
    s32 field_0x0f4;       /* 0x0f4 */
    s32 field_0x0f8;       /* 0x0f8 */
    s32 field_0x0fc;       /* 0x0fc */
    s32 ptsHi;             /* 0x100 stamp saved by the picture-attribute path */
    s32 ptsLo;             /* 0x104 */
    u8  field_0x108[0x68]; /* 0x108 */
} SfdMpvSession;           /* 0x170 */

/* ------------------------------------------------------------------------- */
/* MPV picture parameter table                                               */
/* ------------------------------------------------------------------------- */

/* Global copy of the parameter table (lbl_eu_80619B20).  It carries the
 * requested user-buffer geometry at its tail, which the per-handle copy does
 * not keep. */
typedef struct SfdMpvParaGlobal {
    s32 para[9];           /* 0x00 picture parameter words (0x1c: frame count) */
    s32 field_0x24;        /* 0x24 */
    s32 stride[2];         /* 0x28 luma/chroma line strides, 32-byte aligned */
    s32 frameSize[16];     /* 0x30 per-frame byte budget */
    u32 usrWidth;          /* 0x70 requested user picture width */
    u32 usrHeight;         /* 0x74 requested user picture height */
    u32 usrStride;         /* 0x78 requested user picture line stride */
} SfdMpvParaGlobal;        /* 0x7c */

/* Per-handle copy of the parameter table (SfdHandle::para).  The word at
 * global +0x24 is not mirrored here, so the strides and everything after
 * them sit four bytes lower than in the global copy. */
typedef struct SfdMpvParaWork {
    s32 para[9];           /* 0x00 picture parameter words (0x1c: frame count) */
    s32 stride[2];         /* 0x24 luma/chroma line strides */
    s32 frameSize[16];     /* 0x2c per-frame byte budget */
} SfdMpvParaWork;          /* 0x6c */

/* ------------------------------------------------------------------------- */
/* User picture buffer                                                       */
/* ------------------------------------------------------------------------- */

/* Geometry of the application picture buffer requested by SFD_SetPicUsrBuf().
 * The 16 bands address the buffer in 16-line steps (top/bottom field). */
typedef struct SfdPicUsrBuf {
    u32 width;             /* 0x00 */
    u32 height;            /* 0x04 */
    u32 stride;            /* 0x08 */
    u32 curWidth;          /* 0x0c */
    u32 field_0x10;        /* 0x10 */
    struct {
        u32 top;           /* +0x00 */
        u32 bottom;        /* +0x04 */
    } band[16];            /* 0x14 */
} SfdPicUsrBuf;            /* 0x94 */

/* ------------------------------------------------------------------------- */
/* Parsed Sofdec header                                                      */
/* ------------------------------------------------------------------------- */

/* Result of running the SFH analyzer over the in-handle header image; it sits
 * at handle+0x88 and is (re)filled by sfhds_DoProcessHdr(). */
typedef struct SfdHdrInfo {
    s32 active;            /* 0x00 header analysed */
    s32 toolVerMajor;      /* 0x04 */
    s32 toolVerMinor;      /* 0x08 */
    s32 byteRate;          /* 0x0c */
    s32 hdrSize;           /* 0x10 */
    s32 packType;          /* 0x14 */
    s32 pketSizLen;        /* 0x18 */
    s32 packSiz;           /* 0x1c */
    s32 numElemTot;        /* 0x20 */
    s32 numElemAud;        /* 0x24 */
    s32 numElemVid;        /* 0x28 */
    s32 numElemPrv;        /* 0x2c */
    s32 maxPlyLenAud;      /* 0x30 */
    s32 maxPlyLenVid;      /* 0x34 */
    s32 maxFrmNum;         /* 0x38 */
    s32 audStmId;          /* 0x3c */
    s32 vidStmId;          /* 0x40 */
    s32 audElemId;         /* 0x44 */
    s32 vidElemId;         /* 0x48 */
    s32 elemCodecAud;      /* 0x4c */
    s32 elemLayer;         /* 0x50 */
    s32 elemChNum;         /* 0x54 */
    s32 elemSmpHz;         /* 0x58 */
    s32 video[13];         /* 0x5c: sfhds_AnlyVideo() results (0x7c pic rate,
                            *      0x80 effect present, 0x8c fix flag,
                            *      0x98 gop M) */
} SfdHdrInfo;              /* 0x90 */

/* ------------------------------------------------------------------------- */
/* PTS queue                                                                 */
/* ------------------------------------------------------------------------- */

/* One queued presentation timestamp with the packet it belongs to. */
typedef struct SfdPtsEntry {
    union {
        s64 pts;           /* 0x00 */
        u32 hw[2];         /* 0x00 raw halves */
    } u;
    u32 pos;               /* 0x08 byte position */
    u32 size;              /* 0x0c byte size */
} SfdPtsEntry;             /* 0x10 */

/* Ring of SfdPtsEntry, one per buffer slot (SfdBuf::pts). */
typedef struct SfdPtsQue {
    SfdPtsEntry* entries;  /* 0x00 */
    s32 maxIdx;            /* 0x04 ring capacity in entries */
    s32 count;             /* 0x08 valid entries */
    s32 wr;                /* 0x0c write index */
    s32 rd;                /* 0x10 read index */
} SfdPtsQue;               /* 0x14 */

/* ------------------------------------------------------------------------- */
/* Buffer slots                                                              */
/* ------------------------------------------------------------------------- */

/* Supply part of a buffer slot (SfdBuf::sup): what the application hands to
 * SFBUF_SetSupplySj() plus the DLM marker the reader tracks. */
typedef struct SfdSupply {
    u32 memPtr;            /* 0x00 supplied memory */
    void* sj;              /* 0x04 SJ ring-buffer object */
    u32 base;              /* 0x08 ring memory base */
    u32 size;              /* 0x0c ring capacity in bytes */
    u32 extra;             /* 0x10 bytes reserved for the wrap marker */
    u32 field_0x14;        /* 0x14 */
    u32 dlmPos;            /* 0x18 DLM marker position, 0 = no live data */
    u32 dlmSize;           /* 0x1c DLM marker size */
    u32 wTot;              /* 0x20 running write total, negative = disabled */
    u32 rTot;              /* 0x24 running read total, negative = disabled */
} SfdSupply;               /* 0x28 */

/* One of the 8 supply/ring slots at handle+0x13b8 (stride 0x74).  Slots 0-2
 * are ring buffers, 3 and 5 video frame queues, 4 and 6 audio ring buffers;
 * the middle block is written by SFBUF_SetSupplySj(). */
typedef struct SfdBuf {
    s32 state;             /* 0x00 5 = active ring, 4 = ready, 1 = vfrm, 2 = aring */
    s32 valid;             /* 0x04 non-zero: slot allocated */
    u32 prepFlg;           /* 0x08 SFBUF_SetPrepFlg */
    u32 termFlg;           /* 0x0c SFBUF_SetTermFlg */
    SfdSupply sup;         /* 0x10 supplied memory and DLM markers */
    SfdPtsQue pts;         /* 0x38 PTS queue for this slot */
    s32 inTrn;             /* 0x4c transport that feeds this slot */
    s32 outTrn;            /* 0x50 transport this slot feeds */
    u8  field_0x54[0x20];  /* 0x54 user output channel block */
} SfdBuf;                  /* 0x74 */

/* ------------------------------------------------------------------------- */
/* Transport slots                                                           */
/* ------------------------------------------------------------------------- */

/* One of the 9 transport records at handle+0x1fd8 (stride 0x44): transport
 * prep/termination flags plus the buffer slots it feeds.  Transport 1 is the
 * MPS (program stream) demuxer: its hdr field holds the SfdMps work area,
 * bufIdx the input ring and the three remaining slots the audio ring, the
 * video ring and the user output channel. */
typedef struct SfdTrn {
    u32 prepFlg;           /* 0x00 preparation done */
    u32 termFlg;           /* 0x04 transport finished */
    void* hdr;             /* 0x08 per-transport object (transport 1: SfdMps) */
    void* table;           /* 0x0c transport operation table */
    s32 bufIdx;            /* 0x10 buffer slot this transport reads, 8 = none */
    s32 termIdx;           /* 0x14 first output slot, 8 = none */
    s32 vidIdx;            /* 0x18 second output slot, 8 = none */
    s32 usrIdx;            /* 0x1c third output slot, 8 = none */
    s32 field_0x20;        /* 0x20 transport 1: stream position of the endcode */
    u8  field_0x24[0x20];  /* 0x24 */
} SfdTrn;                  /* 0x44 */

/* ------------------------------------------------------------------------- */
/* Time controller                                                           */
/* ------------------------------------------------------------------------- */

/* Time-code block: four of them (current / previous / ...) live in SfdTim.
 * sftim_Tc2Time*() unpack the code into these fields. */
typedef struct SfdTc {
    s32 flag;              /* 0x00 */
    s32 field_0x04;        /* 0x04 */
    s32 hour;              /* 0x08 */
    s32 minute;            /* 0x0c */
    s32 second;            /* 0x10 */
    s32 frame;             /* 0x14 */
    s32 frame2;            /* 0x18 picture rate numerator */
    s16 field_0x1c;        /* 0x1c */
    s16 field;             /* 0x1e field flag */
    s32 field_0x20;        /* 0x20 */
    s32 ttime;             /* 0x24 */
    s32 ftime;             /* 0x28 */
} SfdTc;                    /* 0x2c */

/* Picture-header info block copied into the decode-session header slot. */
typedef struct SfdMpvHdInfo {
    u32 w[11];             /* 0x00 */
} SfdMpvHdInfo;             /* 0x2c */

/* Concatenated-file playback bookkeeping at the tail of SfdTime. */
typedef struct SfdConcat {
    s32 time;              /* 0x00 playback position */
    s32 idx;               /* 0x04 queue write index */
    s32 timeQueue[32];     /* 0x08 */
    s32 value;             /* 0x88 */
    s32 totalSmpl;         /* 0x8c */
    s32 field_0x90;        /* 0x90 */
    s32 wr;                /* 0x94 */
    s32 rd;                /* 0x98 */
    s32 smplQueue[32];     /* 0x9c */
} SfdConcat;               /* 0x11c */

/* ------------------------------------------------------------------------- */
/* Test/debug control block                                                  */
/* ------------------------------------------------------------------------- */

/* SFTST_* control block (handle+0x1098, i.e. SfdTim::tst): the movie-API
 * trace/error test helpers.  The two-word members hold (value, unit) pairs,
 * which the setters store one word at a time. */
typedef struct SfdTst {
    s32 tstFlg;            /* 0x000 SFTST_SetTstFlg */
    s32 pause;             /* 0x004 SFTST_Pause */
    s32 field_0x008;       /* 0x008 */
    s32 adjFlg;            /* 0x00c SFTST_SetAdjFlg */
    s32 movaveRange;       /* 0x010 SFTST_SetMovaveRange */
    s32 field_0x014;       /* 0x014 */
    u8  table[0x0f0];      /* 0x018 s16 error/time tables */
    s32 field_0x108;       /* 0x108 */
    s32 field_0x10c;       /* 0x10c */
    s32 field_0x110;       /* 0x110 */
    s32 field_0x114;       /* 0x114 */
    s32 field_0x118;       /* 0x118 */
    s32 field_0x11c;       /* 0x11c */
    s32 field_0x120;       /* 0x120 */
    s32 field_0x124;       /* 0x124 */
    s32 position[2];       /* 0x128 accumulated picture position */
    s32 step[2];           /* 0x130 position advance per frame */
    s32 tolerance[4];      /* 0x138 SFTST_SetTolerance */
    s32 excessErr[4];      /* 0x148 SFTST_SetExcessErr */
    s32 adjStart[4];       /* 0x158 SFTST_SetAdjStart */
    s32 adjPoff[4];        /* 0x168 SFTST_SetAdjPoff */
    s32 prevTime[2];       /* 0x178 -1 until the first frame was seen */
    s32 field_0x180[2];    /* 0x180 -1 */
    s32 field_0x188[2];    /* 0x188 */
    s32 baseTime[2];       /* 0x190 in-point the next frame is measured from */
    s32 speedNum;          /* 0x198 SFTST_SetSpeed */
    s32 speedDen;          /* 0x19c */
    u8  field_0x1a0[0x30]; /* 0x1a0 */
} SfdTst;                   /* 0x1d0 */

/* Per-handle time controller at handle+0xd98 (the SFTIM context): the
 * pluggable clock getters, the time-code blocks the decoder rotates, the
 * concatenated-file bookkeeping, the inter-picture time tracking and the
 * test/debug block SFTST_* operates on. */
typedef struct SfdTim {
    void* getTimeFn[6];    /* 0x000 clock getter table (SFTIM_SetTimeFn) */
    void* field_0x018;     /* 0x018 user skip check callback */
    s32 tc[8];             /* 0x01c time code of the picture being decoded */
    SfdMpvHdInfo hdInfo;   /* 0x03c */
    SfdTc tcA;             /* 0x068 */
    SfdTc tcB;             /* 0x094 */
    SfdTc tcC;             /* 0x0c0 */
    SfdTc tcD;             /* 0x0ec */
    s32 field_0x118[0x09]; /* 0x118 */
    s32 limit;             /* 0x13c time limit of the current stream */
    s32 limitUnit;         /* 0x140 its unit */
    s32 startTimeHi;       /* 0x144 SFTIM_SetStartTime */
    s32 startTimeLo;       /* 0x148 */
    s32 field_0x14c;       /* 0x14c */
    s32 streamPosHi;       /* 0x150 byte position taken from the header, -1 */
    s32 streamPosLo;       /* 0x154 */
    s32 posDeltaHi;        /* 0x158 difference to the demuxer position */
    s32 posDeltaLo;        /* 0x15c */
    s32 field_0x160;       /* 0x160 */
    SfdConcat concat;      /* 0x164 */
    s32 curTime;           /* 0x280 time of the picture being read (-5 = none) */
    s32 curTimeUnit;       /* 0x284 its unit */
    s32 maxTime;           /* 0x288 highest playback time seen, -5 = none */
    s32 maxTimeUnit;       /* 0x28c its unit */
    s32 time;              /* 0x290 playback time (SFTIM_GetTime) */
    s32 timeUnit;          /* 0x294 its unit */
    s32 lastItime;         /* 0x298 SFTIM_UpdateItime */
    s32 nextItime;         /* 0x29c */
    s32 maxDelta;          /* 0x2a0 */
    s32 minDelta;          /* 0x2a4 */
    s32 field_0x2a8;       /* 0x2a8 */
    volatile s32 vsyncTime; /* 0x2ac time derived from the vsync counter */
    s32 speedNum;          /* 0x2b0 SFTIM_SetSpeed */
    s32 speedDen;          /* 0x2b4 */
    s32 field_0x2b8;       /* 0x2b8 */
    s32 field_0x2bc;       /* 0x2bc */
    s32 field_0x2c0;       /* 0x2c0 */
    s32 field_0x2c4;       /* 0x2c4 */
    s32 field_0x2c8;       /* 0x2c8 */
    s32 field_0x2cc;       /* 0x2cc */
    s32 field_0x2d0;       /* 0x2d0 */
    volatile s32 field_0x2d4; /* 0x2d4 audio time, updated by the server */
    s32 field_0x2d8;       /* 0x2d8 */
    void* usrTimeFn;       /* 0x2dc SFD_SetUsrTimeFn callback */
    void* usrTimeFnArg;    /* 0x2e0 */
    void* extClockFn;      /* 0x2e4 SFD_SetExtClockFn callback */
    s32 field_0x2e8;       /* 0x2e8 */
    s32 field_0x2ec;       /* 0x2ec */
    s32 field_0x2f0;       /* 0x2f0 */
    s32 extClockWrap;      /* 0x2f4 external clock wrap value */
    void* extClockArg;     /* 0x2f8 */
    u8  field_0x2fc[0x04]; /* 0x2fc */
    SfdTst tst;            /* 0x300 test/debug control block */
    s16 table[0x80];       /* 0x4d0 s16 tables used by the repeat-field code */
    u8  ptsBlk0[0x0c];     /* 0x5d0 time block handed to the pts callback */
    u8  ptsBlk1[0x0c];     /* 0x5dc time block handed to the pts callback */
    s32 field_0x5e8;       /* 0x5e8 */
    s32 field_0x5ec;       /* 0x5ec */
    s32 field_0x5f0;       /* 0x5f0 */
    s32 field_0x5f4;       /* 0x5f4 */
    s32 limitTime;         /* 0x5f8 SFD_SetLimitTime */
} SfdTim;                  /* 0x5fc */

/* ------------------------------------------------------------------------- */
/* Seek-skip control                                                         */
/* ------------------------------------------------------------------------- */

/* SFSEEKI_InitHn() clears this block at handle+0x2680; while a seek is in
 * flight SFSEEKI_IsSkip() drops frames until the playback position passes it. */
typedef struct SfdSeeki {
    u32 skipCount;         /* 0x00 frames still to skip */
    u32 field_0x04;        /* 0x04 */
    u32 targetTime;        /* 0x08 seek target */
    u32 field_0x0c;        /* 0x0c */
    u32 field_0x10;        /* 0x10 */
    u32 field_0x14;        /* 0x14 */
    u32 targetEnd;         /* 0x18 */
} SfdSeeki;                /* 0x1c */

/* ------------------------------------------------------------------------- */
/* Audio-out implementation object                                           */
/* ------------------------------------------------------------------------- */

/* SFAOAP_Create() points transport 7 at this block (handle+0x261c); the
 * playback API calls the out-volume/speed hooks through it. */
typedef struct SfdAoapIf {
    u32  field_0x00;       /* 0x00 */
    void (*setOutPan)(void* self);                 /* 0x04 */
    void (*getOutPan)(void* self);                 /* 0x08 */
    void (*setOutVol)(void* self, s32 vol);        /* 0x0c */
    s32  (*getOutVol)(void* self);                 /* 0x10 */
    void (*setSpeed)(void* self, s32 a, s32 b);    /* 0x14 */
    u8  field_0x18[0x3c];  /* 0x18 */
} SfdAoapIf;               /* 0x54 */

/* ------------------------------------------------------------------------- */
/* ADX transport stream info                                                 */
/* ------------------------------------------------------------------------- */

/* Created by SFADXT_Create() at handle+0x25c0 and parked in transport 3. */
typedef struct SfdAdxtInf {
    void* adxt;            /* 0x00 ADXT decoder handle */
    void* sj;              /* 0x04 SJ ring buffer */
    u32 field_0x08;        /* 0x08 */
    u32 field_0x0c;        /* 0x0c */
    void* pool;            /* 0x10 stream memory */
    void* sjPool;          /* 0x14 ring buffer memory */
    u32 field_0x18;        /* 0x18 */
    u32 field_0x1c;        /* 0x1c */
    void* field_0x20;      /* 0x20 */
    s32 time;              /* 0x24 decoded sample position */
    s32 timeUnit;          /* 0x28 */
    s32 field_0x2c;        /* 0x2c */
    s32 total;             /* 0x30 supplied samples */
    s32 field_0x34;        /* 0x34 */
    s32 syncOffset;        /* 0x38 */
    void (*feed)(void* self, s32 idx, s32 len, s32* out); /* 0x3c */
    s32 field_0x40;        /* 0x40 */
    s32 sfreq;             /* 0x44 sample rate */
    s32 copyLen;           /* 0x48 bytes copied so far */
    u8  field_0x4c[0x10];  /* 0x4c */
} SfdAdxtInf;              /* 0x5c */

/* ------------------------------------------------------------------------- */
/* Header work area                                                          */
/* ------------------------------------------------------------------------- */

/* Header image work area at handle+0x88: the parsed header plus the raw
 * header image it was produced from (SFHDS_*). */
typedef struct SfdFhd {
    SfdHdrInfo info;       /* 0x000 parsed header */
    s32 hdrSize;           /* 0x090 header image bytes in hdrBuf */
    u8  hdrBuf[0x800];     /* 0x094 header image */
} SfdFhd;                   /* 0x894 */

/* ------------------------------------------------------------------------- */
/* MPS demux work area                                                       */
/* ------------------------------------------------------------------------- */

/* SFMPS_Create() carves this block out of the player handle at +0x2240 and
 * parks a pointer to it transport 1's hdr field.  The per-stream output SJ
 * table covers stream ids 0xbc-0xff. */
typedef struct SfdMps {
    void* mps;             /* 0x000 MPS library work object */
    s32 field_0x004;       /* 0x004 output stream count */
    s32 field_0x008;       /* 0x008 output stream count, second kind */
    u8  field_0x00c[0x04]; /* 0x00c */
    s64 ptsMin;            /* 0x010 lowest PTS seen so far */
    s64 ptsMax;            /* 0x018 highest PTS seen so far */
    s32 endcodeCnt;        /* 0x020 endcodes seen */
    s32 prvVid;            /* 0x024 previous video stream id */
    s32 prvAud;            /* 0x028 previous audio stream id */
    s32 firstVid;          /* 0x02c first video stream id, -1 = none yet */
    s32 firstAud;          /* 0x030 first audio stream id */
    s32 selVid;            /* 0x034 video stream selected for output */
    s32 selAud;            /* 0x038 audio stream selected for output */
    s32 endcodeFlg;        /* 0x03c endcode was delimited */
    void* sjOut[68];       /* 0x040 per-stream (0xbc-0xff) output SJ */
    void (*notify)(s32 arg, s32 stmId); /* 0x150 packet-copied hook */
    s32 notifyArg;         /* 0x154 */
    s32 pketPos;           /* 0x158 read position inside the current packet */
    u8  field_0x15c[0xb4]; /* 0x15c */
} SfdMps;                   /* 0x210 */

/* ------------------------------------------------------------------------- */
/* AV-playback object                                                        */
/* ------------------------------------------------------------------------- */

/* MPS header block inside SfdAvp (+0x8a0), kept so the demuxer can be
 * restarted on a concatenated file: the decoded pack/system header plus the
 * two raw header images MPS_DecHd() is re-run over. */
typedef struct SfdMpsHd {
    s32 state;             /* 0x000 non-zero: block already filled */
    s32 picRate;           /* 0x004 picture rate * 50 */
    s32 timeUnit;          /* 0x008 */
    s32 field_0x00c;       /* 0x00c */
    s32 field_0x010;       /* 0x010 */
    s32 field_0x014;       /* 0x014 */
    s32 bytePosHi;         /* 0x018 stream position the header was taken at */
    s32 bytePosLo;         /* 0x01c */
    s32 field_0x020;       /* 0x020 demuxer PTS low-water mark */
    s32 field_0x024;       /* 0x024 */
    s32 field_0x028;       /* 0x028 */
    s32 field_0x02c;       /* 0x02c */
    u8  packHdr[0xb0];     /* 0x030 raw pack header image */
    u8  sysHdr[0xb0];      /* 0x0e0 raw system header image */
    s32 packSize;          /* 0x190 bytes stored in packHdr */
    s32 sysSize;           /* 0x194 bytes stored in sysHdr */
} SfdMpsHd;                 /* 0x198 */

/* Header/sync information of the ADX transport: the head-analysis state and
 * byte rate the seek/head analysis publishes, plus the decoder parameters
 * sfadxt_SetAdxtHd() records once the stream is running. */
typedef struct SfdAdxtSync {
    s32 seek;              /* 0x00 0 = not analysed yet */
    s32 byteRate;          /* 0x04 */
    s32 timeUnit;          /* 0x08 */
    s32 numChan;           /* 0x0c decoder channel count */
    s32 sfreq;             /* 0x10 decoder sample rate */
    s32 numSmpl;           /* 0x14 decoder sample count */
    u8  field_0x18[0x84];  /* 0x18 */
} SfdAdxtSync;             /* 0x9c */

/* Snapshot of the picture-header analysis the AV-playback object keeps at
 * +0xad0: sfmpv_SetMpvHd() records the raw header image plus the header
 * fields of the first decoded picture, and sfmpv_ReprocessShc() replays them
 * through MPV_DecodePicAtr() after a seek. */
typedef struct SfdAvpHdCopy {
    s32 state;             /* 0x000 1 = snapshot filled */
    s32 byteRate;          /* 0x004 byte rate the header carried * 50 */
    s32 timeUnit;          /* 0x008 its unit */
    SfdMpvHdInfo hdInfo;   /* 0x00c header fields at snapshot time */
    u8  image[0x200];      /* 0x038 raw header image */
    s32 size;              /* 0x238 bytes stored in image */
} SfdAvpHdCopy;            /* 0x23c */

/* Per-handle AV-playback object (SfdSee::avplay), created by the host.  The
 * current header image is mirrored at +0x000c and the recalculated byte rate
 * the time conversion uses lives at +0x0da8. */
typedef struct SfdAvp {
    s32 state;             /* 0x000 1 = head analysis done */
    s32 byteRate;          /* 0x004 byte rate the time conversion uses */
    s32 timeUnit;          /* 0x008 its unit */
    s32 hdrImage;          /* 0x00c first word of the mirrored header image
                            *       (a full SfdFhd is mirrored from here) */
    u8  field_0x010[0x08]; /* 0x010 */
    s32 hdrByteRate;       /* 0x018 byte rate carried by the header */
    u8  field_0x01c[0x24]; /* 0x01c */
    s32 hdrTotalSize;      /* 0x040 total stream size carried by the header */
    u8  field_0x044[0x85c];/* 0x044 */
    SfdMpsHd mpsHd;        /* 0x8a0 */
    u8  field_0xa38[0x98]; /* 0xa38 */
    SfdAvpHdCopy audHd;    /* 0xad0 audio header analysis snapshot */
    SfdAdxtSync adxtSync;  /* 0xd0c ADX transport header/sync information */
    s32 eByteRate;         /* 0xda8 effective byte rate */
    s32 finTime;           /* 0xdac fin-analysis time */
    s32 finSize;           /* 0xdb0 fin-analysis size */
    s32 finTimeUnit;       /* 0xdb4 */
    s32 fixTime;           /* 0xdb8 SFSEE_FixAvPlay, < 0 = not fixed */
    s32 fixSize;           /* 0xdbc */
    u8  field_0xdc0[0x04]; /* 0xdc0 */
    s32 fileSize;          /* 0xdc4 SFD_SetFileSize */
    s32 totalSize;         /* 0xdc8 SFD_SetTotTime */
    s32 totalTime;         /* 0xdcc */
    s32 setByteRate;       /* 0xdd0 SFD_SetByteRate */
    s32 seekPos;           /* 0xdd4 SFD_SetSeekPos */
} SfdAvp;                   /* 0xdd8 */

/* Seek/head-analysis control block at handle+0x2670 (SFSEE_*). */
typedef struct SfdSee {
    SfdAvp* avplay;        /* 0x000 AV-playback object */
    s32 field_0x004;       /* 0x004 */
    s32 seekTime;          /* 0x008 seek/analysis time, -3 = none yet */
    s32 seekTimeUnit;      /* 0x00c its unit */
} SfdSee;                   /* 0x010 */

/* ------------------------------------------------------------------------- */
/* Trace sink                                                                */
/* ------------------------------------------------------------------------- */

/* Software-filter trace context (gpSfdCoreTraceCb): slot 9 of the vtable is
 * the "write one trace record" hook every module calls with a pointer into
 * its shared static record. */
typedef struct SfdTraceVtbl {
    void (*field_0x00)(void);                             /* 0x00 */
    void (*field_0x04)(void);                             /* 0x04 */
    void (*field_0x08)(void);                             /* 0x08 */
    void (*field_0x0c)(void);                             /* 0x0c */
    void (*field_0x10)(void);                             /* 0x10 */
    void (*field_0x14)(void);                             /* 0x14 */
    void (*field_0x18)(void);                             /* 0x18 */
    void (*field_0x1c)(void);                             /* 0x1c */
    void (*field_0x20)(void);                             /* 0x20 */
    void (*trace)(void* ctx, void* rec);                  /* 0x24 */
} SfdTraceVtbl;

typedef struct SfdTraceCtx {
    SfdTraceVtbl* vtbl;    /* 0x00 */
} SfdTraceCtx;

/* Trace records handed to SfdTraceVtbl::trace.  Each module function owns one
 * static record in sfd_lib's .data: the function name, then an argument list
 * for the entry event (+0x04) and, for most functions, one for the exit event
 * (+0x6C).  Each argument slot is { pointer to the traced value, value-type
 * code, argument name } (type 0x4 = s32, 0x5 = error code, 0x6 = byte count,
 * 0xA = string, 0xB = handle).  The caller patches the value pointers just
 * before firing the event. */
typedef struct SfdTrcArg {      /* name-guess: CRI type name unknown */
    void* value;                  /* 0x00 */
    s32 type;                     /* 0x04 */
    const char* name;             /* 0x08 */
} SfdTrcArg;                    /* 0x0C */

typedef struct SfdTrcArgs {     /* name-guess */
    s32 field_0x00;               /* 0x00 always 0 in retail data */
    s32 count;                    /* 0x04 number of used args */
    SfdTrcArg args[8];          /* 0x08 */
} SfdTrcArgs;                   /* 0x68 */

typedef struct SfdTrcRec {      /* name-guess */
    const char* func;             /* 0x00 */
    SfdTrcArgs entry;           /* 0x04 */
    SfdTrcArgs exit;            /* 0x6C */
} SfdTrcRec;                    /* 0xD4 */

typedef struct SfdTrcRecIn {    /* name-guess: entry-only record */
    const char* func;             /* 0x00 */
    SfdTrcArgs entry;           /* 0x04 */
} SfdTrcRecIn;                  /* 0x6C */

/* Transfer-module interface (JP map: SFD_tr_in_mem, SFD_tr_sd_mps,
 * SFD_tr_vd_mpv, ...).  Slot order follows the SFxxx_ entry points each
 * module exports.  The slots stay untyped (void*) because the decompiled
 * entry points do not yet agree on one signature per slot (int/s32/void*
 * returns differ between modules). */
typedef struct SfdTrIf {          /* name-guess */
    void* init;                   /* 0x00 */
    void* finish;                 /* 0x04 */
    void* execServer;             /* 0x08 */
    void* create;                 /* 0x0C */
    void* destroy;                /* 0x10 */
    void* requestStop;            /* 0x14 */
    void* start;                  /* 0x18 */
    void* stop;                   /* 0x1C */
    void* pause;                  /* 0x20 */
    void* getWrite;               /* 0x24 */
    void* addWrite;               /* 0x28 */
    void* getRead;                /* 0x2C */
    void* addRead;                /* 0x30 */
    void* seek;                   /* 0x34 */
    void* ext;                    /* 0x38 */
} SfdTrIf;                        /* 0x3C */


/* ------------------------------------------------------------------------- */
/* System work area                                                          */
/* ------------------------------------------------------------------------- */

/* Time-unit context of the global work area, set up by SFTIM_Init(). */
typedef struct SfdTimCtx {
    s32 field_0x00;        /* 0x00 */
    s32 field_0x04;        /* 0x04 */
    s32 field_0x08;        /* 0x08 clock unit handed to the callers */
    s32 field_0x0c;        /* 0x0c tick divisor */
    s32 field_0x10;        /* 0x10 */
} SfdTimCtx;               /* 0x14 */

/* Process-wide SFD work area (sSfdGlob): the configuration image the
 * first created player copies in, plus the shared time/buffer/transport
 * contexts and the library version word. */
typedef struct SfdSys {
    u8  field_0x000[0x190];/* 0x000 configuration image */
    s32 field_0x190;       /* 0x190 */
    s32 field_0x194;       /* 0x194 */
    s32 field_0x198;       /* 0x198 */
    SfdTimCtx tim;         /* 0x19c time-unit context */
    u8  field_0x1b0[0x04]; /* 0x1b0 buffer context scratch */
    u8  trn[0x3c];         /* 0x1b4 transport setup table */
    s32 field_0x1f0;       /* 0x1f0 */
    s32 field_0x1f4;       /* 0x1f4 */
    s32 field_0x1f8;       /* 0x1f8 */
    s32 field_0x1fc[0x08]; /* 0x1fc */
} SfdSys;                  /* 0x21c */

/* ------------------------------------------------------------------------- */
/* SJ buffer object                                                          */
/* ------------------------------------------------------------------------- */

/* Method table of an SJ ring/memory buffer object (its first word points
 * here): slot 3 destroys it, slot 9 reports the available bytes, slot 11
 * installs the error callback. */
typedef struct SfdSjVtbl {
    void (*field_0x00)(void* self);                             /* 0x00 */
    void (*field_0x04)(void* self);                             /* 0x04 */
    void (*field_0x08)(void* self);                             /* 0x08 */
    void (*destroy)(void* self);                                /* 0x0c */
    void* (*getUuid)(void* self);                               /* 0x10 */
    void (*reset)(void* self);                                  /* 0x14 */
    int (*getChunk)(void* self, int mode, int size, void* out); /* 0x18 */
    int (*ungetChunk)(void* self, int mode, void* chunk);       /* 0x1c */
    int (*putChunk)(void* self, int mode, void* chunk);         /* 0x20 */
    int (*getAvail)(void* self, int mode);                      /* 0x24 */
    int (*isGetChunk)(void* self, int mode, int size, int* out);/* 0x28 */
    void (*entryErrFunc)(void* self, void* cb, void* arg);      /* 0x2c */
} SfdSjVtbl;

typedef struct SfdSjObj {
    SfdSjVtbl* vtbl;       /* 0x00 */
} SfdSjObj;

/* ------------------------------------------------------------------------- */
/* Sofdec handle                                                             */
/* ------------------------------------------------------------------------- */

typedef struct SfdHandle {
    u8  field_0x0000[0x08]; /* 0x0000 */
    s32 field_0x0008;       /* 0x0008 */
    u8  field_0x000c[0x20]; /* 0x000c */
    s32 field_0x002c;       /* 0x002c */
    s32 field_0x0030;       /* 0x0030 */
    u8  field_0x0034[0x0c]; /* 0x0034 */
    s32 field_0x0040;       /* 0x0040 codec id of the current picture */
    u8  field_0x0044[0x0c]; /* 0x0044 */
    s32 field_0x0050;       /* 0x0050 */
    s32 errorState;         /* 0x0054 0 = handle not open (SFLIB_CheckHn) */
    s32 substatus;          /* 0x0058 player sub-state (sfply_StatPrep) */
    s32 field_0x005c;       /* 0x005c */
    s32 field_0x0060;       /* 0x0060 */
    s32 field_0x0064;       /* 0x0064 */
    s32 field_0x0068;       /* 0x0068 play mode, 2 = playing */
    s32 displayCount;       /* 0x006c */
    s32 field_0x0070;       /* 0x0070 */
    s32 field_0x0074;       /* 0x0074 */
    u8  field_0x0078[0x10]; /* 0x0078 */
    SfdFhd fhd;             /* 0x0088 header work area */
    s32 imageWidth;         /* 0x091c */
    s32 imageHeight;        /* 0x0920 */
    u32 field_0x0924;       /* 0x0924 */
    u32 field_0x0928;       /* 0x0928 */
    s32 field_0x092c;       /* 0x092c */
    s32 field_0x0930;       /* 0x0930 picture rate */
    s32 field_0x0934;       /* 0x0934 packed header field (hdr->picRate/50) */
    s32 field_0x0938;       /* 0x0938 system header field */
    s32 field_0x093c;       /* 0x093c */
    s32 field_0x0940;       /* 0x0940 cached hdrInfo.numElemVid */
    s32 field_0x0944;       /* 0x0944 cached hdrInfo.numElemPrv */
    s32 field_0x0948;       /* 0x0948 cached hdrInfo.maxPlyLenAud */
    u8  field_0x094c[0x14]; /* 0x094c */
    s32 field_0x0960;       /* 0x0960 */
    s32 field_0x0964;       /* 0x0964 */
    s32 field_0x0968;       /* 0x0968 decoded DCT count */
    s32 field_0x096c;       /* 0x096c */
    s32 field_0x0970;       /* 0x0970 empty-B frames */
    s32 field_0x0974;       /* 0x0974 */
    s32 field_0x0978;       /* 0x0978 */
    s32 field_0x097c;       /* 0x097c */
    u8  field_0x0980[0x08]; /* 0x0980 */
    s32 field_0x0988;       /* 0x0988 */
    s32 field_0x098c;       /* 0x098c */
    u64 field_0x0990;       /* 0x0990 */
    u64 supplied;           /* 0x0998 bytes handed to the MPS demuxer */
    u64 consumed;           /* 0x09a0 bytes the demuxer consumed */
    u64 discarded;          /* 0x09a8 bytes the demuxer skipped */
    u64 field_0x09b0;       /* 0x09b0 flow counter block ("supplied") */
    s64 field_0x09b8;       /* 0x09b8 consumed bytes */
    u64 field_0x09c0;       /* 0x09c0 "discarded" */
    u32 flowVidAHi;         /* 0x09c8 video ring flow counter, high half */
    u32 flowVidALo;         /* 0x09cc */
    u32 flowVidBHi;         /* 0x09d0 second video ring half */
    u32 flowVidBLo;         /* 0x09d4 */
    s64 flowSkip;           /* 0x09d8 samples excluded by the seek/skip path */
    u32 flowAudAHi;         /* 0x09e0 audio ring flow counter, high half */
    u32 flowAudALo;         /* 0x09e4 */
    u32 flowAudBHi;         /* 0x09e8 second audio ring half */
    u32 flowAudBLo;         /* 0x09ec */
    u8  field_0x09f0[0x18]; /* 0x09f0 */
    void (*errFn)(u32 arg, u32 code); /* 0x0a08 per-handle error callback */
    u32 errFnArg;           /* 0x0a0c */
    u32 errCode;            /* 0x0a10 first error code reported */
    s32 field_0x0a14;       /* 0x0a14 */
    s32 field_0x0a18;       /* 0x0a18 */
    u32 cond[100];          /* 0x0a1c player condition words (SFSET_GetCond) */
    u32 condBak[100];       /* 0x0bac condition words saved across a seek */
    void* hdrCb;            /* 0x0d3c SFD_SetSfdHeaderFn callback */
    void* hdrCbArg;         /* 0x0d40 */
    void* field_0x0d44;     /* 0x0d44 */
    void* field_0x0d48;     /* 0x0d48 */
    void* field_0x0d4c;     /* 0x0d4c */
    void* field_0x0d50;     /* 0x0d50 */
    u8  field_0x0d54[0x08]; /* 0x0d54 */
    void (*usrPesFn)(s32 arg, void* inf, s32 a, s32 b); /* 0x0d5c */
    s32 usrPesFnArg;        /* 0x0d60 */
    void* delimCb;          /* 0x0d64 demux delimiter callback */
    void* delimCbArg;       /* 0x0d68 */
    void (*field_0x0d6c)(void*, void*, u32*); /* 0x0d6c decoded-picture hook */
    void* field_0x0d70;     /* 0x0d70 decoded-picture hook arg */
    void (*field_0x0d74)(void*, void*, u32*); /* 0x0d74 skipped-picture hook */
    void* field_0x0d78;     /* 0x0d78 skipped-picture hook arg */
    s32 (*chkImageSize)(void*); /* 0x0d7c */
    void* chkImageSizeArg;  /* 0x0d80 */
    u32 field_0x0d84;       /* 0x0d84 multi-buffer callback enable */
    u32 field_0x0d88;       /* 0x0d88 multi-buffer callback argument */
    void* field_0x0d8c;     /* 0x0d8c multi-buffer callback */
    void* field_0x0d90;     /* 0x0d90 */
    void* field_0x0d94;     /* 0x0d94 */
    SfdTim tim;             /* 0x0d98 time controller (SFTIM context) */
    u8  field_0x1394[0x24]; /* 0x1394 */
    SfdBuf buf[8];          /* 0x13b8 supply/ring slots */
    SfdVfrm vfrm[16];       /* 0x1758 video frame queue */
    SfdTrn trn[9];          /* 0x1fd8 transport slots */
    u8  field_0x223c[0x04]; /* 0x223c */
    SfdMps mps;             /* 0x2240 SFMPS demux work area */
    SfdMpvSession mpvSession;/* 0x2450 SFMPV decode session */
    SfdAdxtInf adxtInf;     /* 0x25c0 ADX transport stream info */
    SfdAoapIf aoapIf;       /* 0x261c audio-out implementation */
    SfdSee see;             /* 0x2670 seek/head-analysis control block */
    SfdSeeki seeki;         /* 0x2680 seek-skip control block */
    u8  field_0x269c[0x04]; /* 0x269c */
    u8  field_0x26a0[0xa0]; /* 0x26a0 five 0x20-byte timer accumulators */
    u8  field_0x2740[0x20]; /* 0x2740 last-frame accumulator */
    u64 field_0x2760;       /* 0x2760 last frame timer stamp */
    u64 field_0x2768;       /* 0x2768 current timer stamp */
    u64 field_0x2770;       /* 0x2770 timer unit */
    s32 field_0x2778;       /* 0x2778 frame-rate numerator */
    f32 field_0x277c;       /* 0x277c measured frame rate */
    SfdMpvParaWork para;    /* 0x2780 MPV parameter table */
    s32 frameCount;         /* 0x27ec frame records in use */
    s32 termFlag;           /* 0x27f0 termination requested */
    SfdFrame* curFrame;     /* 0x27f4 frame currently handed out */
    SfdFrame frames[16];    /* 0x27f8 */
    SfdPicUsrBuf picUsr;    /* 0x38f8 */
    u32 field_0x398c;       /* 0x398c */
    u32 audStereoStat;      /* 0x3990 // name-guess: mwsffrm_ChangeSettingSyncPlayback checks it for channel mode 2 */
    u32 audNoChanStat;      /* 0x3994 // name-guess: mwsffrm_ChangeSettingSyncPlayback checks it for channel mode 0 */
    u32 field_0x3998;       /* 0x3998 */
    u32 field_0x399c;       /* 0x399c */
    void (*writeNotify)(s32 arg, u32 addr); /* 0x39a0 SFMPS_AddWrite hook */
    s32 writeNotifyArg;     /* 0x39a4 */
    u32 writeTotal;         /* 0x39a8 bytes written so far */
    u32 notifyAddr;         /* 0x39ac address reported last */
} SfdHandle;                /* 0x39b0 */
