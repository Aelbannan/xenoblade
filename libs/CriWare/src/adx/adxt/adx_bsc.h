#ifndef CRIWARE_ADX_BSC_H
#define CRIWARE_ADX_BSC_H

#include <types.h>

/*
 * One of the 16 ADXB decoder handles.  ADXB_Create() carves a handle out of the
 * module work array, creates the ADXPD decoder object it owns and records the
 * PCM/work buffers the application supplied.  The decode-math fields below
 * (0x4c-0x74) are the running positions ADXB_EvokeDecode()/ADXB_EndDecode()
 * advance while a block is decoded; they are seeded from the header
 * information and from the get-write-position callback.
 */
typedef struct ADXB {
    s16  state;            /* 0x00 1 = handle in use */
    s16  field_0x02;       /* 0x02 */
    u32  stat;             /* 0x04 ADXB_STAT_* playback state */
    void* pd;              /* 0x08 ADXPD decoder object */
    s8   encoding;         /* 0x0c codec/encoding id of the stream */
    s8   bitPerSmpl;       /* 0x0d bits per sample */
    s8   numChan;          /* 0x0e channel count */
    s8   blkByte;          /* 0x0f bytes per coded block */
    u32  blkSmpl;          /* 0x10 samples per coded block */
    u32  sfreq;            /* 0x14 sample rate */
    u32  totalSmpl;        /* 0x18 total sample count */
    s16  coefPos;          /* 0x1c ADPCM coefficient set */
    s16  field_0x1e;       /* 0x1e */
    u32  field_0x20;       /* 0x20 loop header field */
    s16  numLoop;          /* 0x24 loop count */
    s16  field_0x26;       /* 0x26 */
    u32  lpStartPos;       /* 0x28 loop start position */
    u32  lpStartOfst;      /* 0x2c loop start offset */
    u32  lpEndPos;         /* 0x30 loop end position */
    u32  lpEndOfst;        /* 0x34 loop end offset */
    void* ctx;             /* 0x38 user context */
    void* pcmBuf;          /* 0x3c decoded PCM buffer */
    void* workBuf;         /* 0x40 loop-copy work buffer */
    u32  workSize;         /* 0x44 its size in samples */
    void* data;            /* 0x48 coded data handed to ADXB_EntryData */
    u32  maxBlks;          /* 0x4c block capacity of the PCM buffer */
    s32  blkSmplW;         /* 0x50 samples per block used by the decoder */
    s32  blkSmplP;         /* 0x54 samples per block reported by ADXPD */
    s32  blkByteW;         /* 0x58 bytes per block used by the decoder */
    u32  pcmBase;          /* 0x5c PCM buffer base address */
    s32  pcmBufSize;       /* 0x60 PCM buffer size in samples */
    s32  chByte;           /* 0x64 bytes per channel block */
    s32  pcmOfst;          /* 0x68 current block offset in the PCM buffer */
    s32  maxWrPos;         /* 0x6c write position limit */
    s32  wrPos;            /* 0x70 current write position */
    u32  field_0x74;       /* 0x74 */
    void* getWr;           /* 0x78 get-write-position callback */
    void* getWrCtx;        /* 0x7c its context */
    void* endCb;           /* 0x80 end-of-stream callback */
    void* endCbCtx;        /* 0x84 its context */
    u32  readSmpl;         /* 0x88 samples read out so far */
    u32  decPos;           /* 0x8c decode position (block units) */
    u32  decNumSmpl;       /* 0x90 samples decoded by the last block */
    u32  decDtLen;         /* 0x94 data length decoded by the last block */
    s16  fmt;              /* 0x98 stream format (0 = ADX) */
    s16  defFmt;           /* 0x9a */
    s16  fmtSub;           /* 0x9c format sub type */
    s16  field_0x9e;       /* 0x9e */
    s16  key[3];           /* 0xa0 decode keys */
    s16  extPrm[3];        /* 0xa6 extended decoder parameters */
    u32  lnkSw;            /* 0xac link switch */
    s16  dly[2];           /* 0xb0 decoder delay snapshot */
    s16  dly2[2];          /* 0xb4 */
    void* ainf;            /* 0xb8 AINF chunk of an AHX stream */
    u32  field_0xbc;       /* 0xbc */
    u32  field_0xc0;       /* 0xc0 */
    u32  ainfLen;          /* 0xc4 AINF chunk length */
    u8   loopTbl[0x10];    /* 0xc8 loop bookkeeping */
    s16  defOutVol;        /* 0xd8 default output volume */
    s16  defPan[2];        /* 0xda default pan */
    u32  field_0xe0;       /* 0xe0 two-channel (PL2) decoder object */
    u8   field_0xe4[0x08]; /* 0xe4 */
    u8   version;          /* 0xec stream version */
    u8   flags;            /* 0xed stream flags */
    u8   field_0xee[0x02]; /* 0xee */
    u32  prevDecDtLen;     /* 0xf0 data length reported by the previous block */
    u32  field_0xf4;       /* 0xf4 */
    void* wrCb;            /* 0xf8 decoded-data callback */
    void* wrCbCtx;         /* 0xfc its context */
} ADXB;                    /* 0x100 */

#endif
