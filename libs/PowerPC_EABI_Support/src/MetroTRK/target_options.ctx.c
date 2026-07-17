/* "libs/PowerPC_EABI_Support/src/MetroTRK/target_options.c" line 0 "PowerPC_EABI_Support/MetroTRK/target_options.h" */
#ifndef _TRK_TARGET_OPTIONS_H
#define _TRK_TARGET_OPTIONS_H

/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MetroTRK/target_options.h" line 3 "PowerPC_EABI_Support/MetroTRK/dstypes.h" */
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

void SetUseSerialIO(ui8);
ui8 GetUseSerialIO(void);

#ifdef __cplusplus
}
#endif
#endif
/* end "PowerPC_EABI_Support/MetroTRK/target_options.h" */

static ui8 bUseSerialIO;


void SetUseSerialIO(ui8 sio){
    bUseSerialIO = sio;
    return;
}

ui8 GetUseSerialIO(void) {
    return bUseSerialIO;
}
