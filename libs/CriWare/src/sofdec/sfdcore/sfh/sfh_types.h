#pragma once

#include <types.h>

/* Work area built by SFH_Create() and handed to the SFH version analysers.
 * The analysers keep their parse state, the header image they are reading and
 * the decoded version numbers here.  0x18 bytes. */
typedef struct SFHContext {
    s32  state;      /* 0x00 0 = created, -1 = invalid, 2 = analysed */
    u8*  header;     /* 0x04 Sofdec header image under analysis */
    u32  hdrSize;    /* 0x08 byte size of the image */
    u32  toolVer;    /* 0x0C tool version packed by VER1_IsSfdHeader */
    u32  sfhVer;     /* 0x10 SFH format version (0x100 for version 1) */
    u32  moduleVer;  /* 0x14 module version */
} SFHContext;
