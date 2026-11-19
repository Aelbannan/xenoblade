#ifndef CRIWARE_ADX_TLK_H
#define CRIWARE_ADX_TLK_H

#include <types.h>

/* One ADXT decoder handle.  ADXT_Create() takes a work area out of the module
 * work array and lays this object over it; the Sofdec ADX transport also reads
 * the RNA decoder object pointer at +0x0c to seed its audio-out interface. */
typedef struct ADXT_OBJ_ {
    u8 active;      /* 0x00 */
    u8 stat;        /* 0x01 */
    u8 mode;        /* 0x02 */
    u8 numChan;     /* 0x03 */
    void* sjd;      /* 0x04 */
    void* stm;      /* 0x08 */
    void* rna;      /* 0x0C */
    void* mainSj;   /* 0x10 */
    void* inSj;     /* 0x14 */
    void* chans[2]; /* 0x18 */
    u8* workBase;   /* 0x20 */
    u32 workSize;   /* 0x24 */
    u32 f28;        /* 0x28 */
    u8* alignedWork;/* 0x2C */
    u32 f30;        /* 0x30 */
    u32 f34;        /* 0x34 */
    u32 svrFreq;    /* 0x38 */
    s16 numSector;  /* 0x3C */
    s16 sectorSize; /* 0x3E */
    s16 f40;        /* 0x40 */
    s16 pan[2];     /* 0x42 */
    s16 f46;        /* 0x46 */
    u8 _pad1[0x54 - 0x48];
    u32 f54;        /* 0x54 */
    u32 f58;        /* 0x58 */
    u32 f5C;        /* 0x5C */
    s16 f60;        /* 0x60 */
    u8 _pad2[0x64 - 0x62];
    u32 f64;        /* 0x64 */
    s16 f68;        /* 0x68 */
    s16 f6A;        /* 0x6A */
    u8 lpFlg;       /* 0x6C */
    u8 f6D;         /* 0x6D */
    u8 _pad3[0x72 - 0x6E];
    u8 pauseFlg;    /* 0x72 */
    u8 f73;         /* 0x73 */
    void* f74_amp;  /* 0x74 */
    u8 _pad4[0x88 - 0x78];
    u32 timeOfst;   /* 0x88 */
    u8 _pad5[0x94 - 0x8C];
    void* lsc;      /* 0x94 */
    u8 f98;         /* 0x98 */
    u8 _pad6[0x9C - 0x99];
    u32 time;       /* 0x9C */
    u32 tick;       /* 0xA0 */
    u32 fA4;        /* 0xA4 */
    u8 fA8;         /* 0xA8 */
    u8 volSw;       /* 0xA9 */
    u8 _pad7[0xAC - 0xAA];
    u8* sjTail;     /* 0xAC */
} ADXT_OBJ_;

#endif
