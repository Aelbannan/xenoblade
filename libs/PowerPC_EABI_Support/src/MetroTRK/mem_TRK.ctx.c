/* "libs/PowerPC_EABI_Support/src/MetroTRK/mem_TRK.c" line 0 "PowerPC_EABI_Support/MetroTRK/mem_TRK.h" */
#ifndef METROTRK_MEM_TRK_H
#define METROTRK_MEM_TRK_H

/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MetroTRK/mem_TRK.h" line 3 "PowerPC_EABI_Support/MetroTRK/dstypes.h" */
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

#ifdef __cplusplus
extern "C"{
#endif

void* TRK_memcpy(void* dst, const void* src, int size);
void* TRK_memset(void* dst, int val, int size);


#ifdef __cplusplus
}
#endif
#endif
/* end "PowerPC_EABI_Support/MetroTRK/mem_TRK.h" */


static ui8 ppc_readbyte1(const ui8* ptr){
    ui32* alignedPtr = (ui32 *)((ui32)ptr & ~3);
    return (ui8)(*alignedPtr >> ((3 - ((ui32)ptr - (ui32)alignedPtr)) << 3));
}

static void ppc_writebyte1(ui8* ptr, ui8 val){
    ui32* alignedPtr = (ui32 *)((ui32)ptr & ~3);
    ui32 v = *alignedPtr;
    ui32 uVar3 = 0xff << ((3 - ((ui32)ptr - (ui32)alignedPtr)) << 3);
    ui32 iVar1 = (3 - ((ui32)ptr - (ui32)alignedPtr)) << 3;
    *alignedPtr = (v & ~uVar3) | (uVar3 & (val << iVar1));
}

void* TRK_memcpy(void* dst, const void* src, int size){
    ui8* srcTemp = (ui8*)src;
    ui8* dstTemp = (ui8*)dst;
    int i;
    
    for(i = 0; i != size; i++){
        ppc_writebyte1(dstTemp, ppc_readbyte1(srcTemp));
        srcTemp++;
        dstTemp++;
    }

    return dst;
}

void TRK_fill_mem(ui8* dst, int val, int size){
    ui8 b = val;
    int i;

    for(i = 0; i != size; i++){
        ppc_writebyte1(dst, b);
        dst++;
    }
}

void* TRK_memset(void* dst, int val, int size)
{
    TRK_fill_mem(dst, val, size);
    return dst;
}
