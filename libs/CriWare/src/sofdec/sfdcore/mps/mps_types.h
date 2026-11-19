#pragma once

#include <types.h>

/*
 * Internal work layouts of the MPS (MPEG program stream) demuxer, as seen by
 * the mps_* modules of sfdcore.  An MPS entry handed out by MPS_Create()
 * (mps_lib) is one of these blocks: the error-handler hook plus the header
 * slots the decoders fill in place (mpsdec_DecPackHd / DecSysHd / DecPketHd)
 * and the callbacks MPS_Set*Fn installs.
 *
 * The header slots are what the public MPS_Get*Hd accessors (mps_get) copy
 * back out, so every offset below doubles as the source of those getters'
 * displacement.  Fields whose meaning is still unknown keep the
 * `field_0x....` spelling with their offset recorded.
 */

/* MPS pack header, decoded by mpsdec_DecPackHd (work + 0x18) */
typedef struct MPS_PACKHD {
    u32 scr_h;   /* 0x18: SCR bits [31:15] (2 + 15 bits) */
    u32 scr_l;   /* 0x1C: SCR low 15 bits */
    u32 flag;    /* 0x20: set when the 2-bit marker is zero */
    u32 rate;    /* 0x24: 22-bit mux rate */
} MPS_PACKHD;

/* MPS system header, decoded by mpsdec_DecSysHd (work + 0x28) */
typedef struct MPS_SYSHDR {
    u32 w0; /* 0x28 */
    u32 w1; /* 0x2C */
    u32 w2; /* 0x30 */
    u32 w3; /* 0x34 */
    u32 w4; /* 0x38 */
    u32 w5; /* 0x3C */
    u32 w6; /* 0x40 */
    u32 w7; /* 0x44 */
} MPS_SYSHDR;

/* MPS packet header, decoded by mpsdec_DecPketHd (work + 0xA8) */
typedef struct MPS_PKETHD {
    u32 pts_lo;     /* 0xA8 */
    u32 pts_hi;     /* 0xAC */
    u32 dts_lo;     /* 0xB0 */
    u32 dts_hi;     /* 0xB4 */
    u32 stream_id;  /* 0xB8 */
    u32 type;       /* 0xBC */
    u32 sub;        /* 0xC0 */
    u32 length;     /* 0xC4 */
    u32 mpeg1_pts;  /* 0xC8 */
    u32 payload;    /* 0xCC */
} MPS_PKETHD;

typedef struct MPS_WORK {
    u8         field_0x00[0x10]; /* 0x00 error hook / handle state */
    u32        field_0x10;       /* 0x10 packet-header stream count */
    u8         field_0x14[0x04]; /* 0x14 */
    MPS_PACKHD  pack;             /* 0x18 pack header */
    MPS_SYSHDR   sys;              /* 0x28 current system header */
    MPS_SYSHDR   sys_save[3];      /* 0x48 per-stream saved copies */
    MPS_PKETHD  pket;             /* 0xA8 packet header */
    u8         field_0xd0[0x04]; /* 0xD0 */
    void      *dec_fn;           /* 0xD4 MPS_SetDecHdFn hook */
    u8         field_0xd8[0x0C]; /* 0xD8 */
    void      *sys_fn;           /* 0xE4 MPS_SetSystemFn hook */
    void      *sys_obj;          /* 0xE8 */
    void      *psmap_fn;         /* 0xEC MPS_SetPsMapFn hook */
    void      *psmap_obj;        /* 0xF0 */
    void      *pes_fn;           /* 0xF4 MPS_SetPesFn hook */
    void      *pes_obj;          /* 0xF8 */
} MPS_WORK;
