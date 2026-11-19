#ifndef CRIWARE_ADX_SJD_H
#define CRIWARE_ADX_SJD_H

#include <types.h>
#include "libs/CriWare/src/adx/adxt/adx_bsc.h"

/* One chunk of the ring buffer an SJ stream object hands out (the pair
 * SJ_SplitChunk() produces and the SJ commit/unget entry points consume). */
typedef struct SjChunk {
    void* ptr;             /* 0x00 */
    s32 len;               /* 0x04 */
} SjChunk;

/*
 * One of the 16 stream-decoder handles ADXSJD_Create() carves out of the
 * module work area.  A handle owns the ADXB decoder it created, the input SJ
 * it pulls coded data from, one output SJ per channel and the chunk that is
 * currently staged for the decoder.
 */
typedef struct ADXSJD {
    s8   inUse;            /* 0x00 1 = handle in use */
    s8   status;           /* 0x01 decoder state */
    s8   numChan;          /* 0x02 channel count */
    s8   ahxFlag;          /* 0x03 stream carries an AHX footer */
    ADXB* adxb;            /* 0x04 bound ADXB handle */
    void* inSj;            /* 0x08 input stream journal */
    void* chan[2];         /* 0x0c per-channel output SJs (numChan used) */
    SjChunk chunk;         /* 0x14 chunk staged for the decoder */
    SjChunk chanChunk[2];  /* 0x1c per-channel chunk handed to the SJ writes */
    u32  totalSmpl;        /* 0x2c decoded samples so far */
    u32  totalDtLen;       /* 0x30 coded bytes so far */
    u32  decPos;           /* 0x34 decoder position in samples */
    u32  maxDecSmpl;       /* 0x38 ADXSJD_SetMaxDecSmpl */
    s32  trapNumSmpl;      /* 0x3c trap point, -1 = none */
    u32  trapCnt;          /* 0x40 samples decoded at the trap point */
    u32  trapDtLen;        /* 0x44 coded bytes decoded at the trap point */
    void* trapFunc;        /* 0x48 trap callback */
    void* trapCtx;         /* 0x4c */
    void* fltFunc;         /* 0x50 stream-property filter callback */
    void* fltCtx;          /* 0x54 */
    void* dataCb;          /* 0x58 decoded-data callback */
    void* dataCbCtx;       /* 0x5c */
    u8   spsd[0x40];       /* 0x60 inline SPSD information */
    u32  hdrLen;           /* 0xa0 coded bytes consumed by the header */
    u32  lnkSw;            /* 0xa4 link switch */
    s32  insertCnt;        /* 0xa8 samples still to insert */
    s32  discardCnt;       /* 0xac samples still to discard */
    u32  pcmLoop;          /* 0xb0 non-zero keeps looping PCM alive */
} ADXSJD;                  /* 0xb4 */

#endif
