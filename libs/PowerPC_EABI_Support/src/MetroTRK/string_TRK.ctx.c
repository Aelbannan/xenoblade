/* "libs/PowerPC_EABI_Support/src/MetroTRK/string_TRK.c" line 0 "PowerPC_EABI_Support/MetroTRK/string_TRK.h" */
#ifndef METROTRK_STRING_TRK
#define METROTRK_STRING_TRK

/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MetroTRK/string_TRK.h" line 3 "PowerPC_EABI_Support/MetroTRK/dstypes.h" */
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

int TRK_strlen(const char*);

#ifdef __cplusplus
}
#endif
#endif
/* end "PowerPC_EABI_Support/MetroTRK/string_TRK.h" */

int TRK_strlen(const char* s) {
    const unsigned char* p = (const unsigned char*)s - 1;
    int len = -1;
    unsigned char c;
    do {
        c = *++p;
        len++;
    } while (c != 0);
    return len;
}
