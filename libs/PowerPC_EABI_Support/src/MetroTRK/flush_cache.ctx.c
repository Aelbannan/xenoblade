/* "libs/PowerPC_EABI_Support/src/MetroTRK/flush_cache.c" line 0 "PowerPC_EABI_Support/MetroTRK/Processor/ppc/Generic/flush_cache.h" */
#ifndef METROTRK_FLUSH_CACHE
#define METROTRK_FLUSH_CACHE

/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MetroTRK/Processor/ppc/Generic/flush_cache.h" line 3 "PowerPC_EABI_Support/MetroTRK/dstypes.h" */
#ifndef METROTRK_DSTYPES
#define METROTRK_DSTYPES

#ifdef __cplusplus
extern "C" {
#endif // ifdef __cplusplus

typedef unsigned char ui8;
typedef unsigned short ui16;
typedef unsigned long ui32;
typedef unsigned long long ui64;
typedef unsigned char ui128[16];

typedef int bool;

#define true 1
#define false 0
#define NULL 0

#ifdef __cplusplus
};
#endif // ifdef __cplusplus

#endif
/* end "PowerPC_EABI_Support/MetroTRK/dstypes.h" */

asm void TRK_flush_cache(ui32 addr, ui32 length);

#endif
/* end "PowerPC_EABI_Support/MetroTRK/Processor/ppc/Generic/flush_cache.h" */

asm void TRK_flush_cache(ui32 addr, ui32 length) {
    nofralloc
    lis r5, 0xFFFFFFF1@h
    ori r5, r5, 0xFFFFFFF1@l
    and r5, r5, r3
    subf r3, r5, r3
    add r4, r4, r3
loop:
    dcbst 0, r5
    dcbf 0, r5
    sync
    icbi 0, r5
    addic r5, r5, 0
    addic. r4, r4, -8
    bge loop
    isync
    blr
}
