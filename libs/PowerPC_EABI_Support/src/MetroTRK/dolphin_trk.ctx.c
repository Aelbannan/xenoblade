/* "libs/PowerPC_EABI_Support/src/MetroTRK/dolphin_trk.c" line 0 "PowerPC_EABI_Support/MetroTRK/trk.h" */
#ifndef _METROTRK_TRK_H
#define _METROTRK_TRK_H

/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MetroTRK/trk.h" line 3 "PowerPC_EABI_Support/MetroTRK/msgcmd.h" */
#ifndef _TRK_MSGCMD_H
#define _TRK_MSGCMD_H

/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MetroTRK/msgcmd.h" line 3 "PowerPC_EABI_Support/MetroTRK/dstypes.h" */
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

#define DS_PROTOCOL_MAJOR_VERSION_1    1
#define DS_PROTOCOL_MINOR_VERSION_10   10

#define DS_PROTOCOL_MAJOR_VERSION      DS_PROTOCOL_MAJOR_VERSION_1
#define DS_PROTOCOL_MINOR_VERSION      DS_PROTOCOL_MINOR_VERSION_10

typedef enum {
    kDSPing = 0x00,
    kDSConnect = 0x01,
    kDSDisconnect = 0x02,
    kDSReset = 0x03,
    kDSVersions = 0x04,
    kDSSupportMask = 0x05,
    kDSOverride = 0x07,
    kDSReadMemory = 0x10,
    kDSWriteMemory = 0x11,
    kDSReadRegisters = 0x12,
    kDSWriteRegisters = 0x13,
    kDSSetOption = 0x17,
    kDSContinue = 0x18,
    kDSStep = 0x19,
    kDSStop = 0x1a,
    kDSReplyACK = 0x80,
    kDSReplyNAK = 0xFF,
    kDSNotifyStopped = 0x90,
    kDSNotifyException = 0x91,
    kDSWriteFile = 0xD0,
    kDSReadFile = 0xD1,
    kDSOpenFile = 0xD2,
    kDSCloseFile = 0xD3,
    kDSPositionFile = 0xD4
} MessageCommandID;


#define DS_MIN_MSG_LENGTH   1
#define DS_MIN_REPLY_LENGTH 2

#define DS_MIN_MSG_VERSIONS_LENGTH           DS_MIN_MSG_LENGTH
#define DS_MIN_REPLY_VERSIONS_LENGTH         (DS_MIN_REPLY_LENGTH + 4)



/*
** CPU Major types.
*/

#define DS_CPU_MAJOR_PPC                     0x00
#define DS_CPU_MAJOR_MIPS                    0x01
#define DS_CPU_MAJOR_NEC_V8XX                0x02
#define DS_CPU_MAJOR_MOT_DSP                 0x03
#define DS_CPU_MAJOR_ARM                     0x04
#define DS_CPU_MAJOR_X86                     0x05
#define DS_CPU_MAJOR_MCORE                   0x06
#define DS_CPU_MAJOR_M68K                    0x07    /* includes ColdFire */
#define DS_CPU_MAJOR_SUPERH                  0x08

/*
** MIPS CPU minor types.
*/

#define DS_CPU_MIPS_R3000                    0x00
#define DS_CPU_MIPS_R3081E                   0x01

#define DS_CPU_MIPS_VR4100                   0x20
#define DS_CPU_MIPS_VR4300                   0x21
#define DS_CPU_MIPS_VR4500                   0x22
#define DS_CPU_MIPS_VR4111                   0x23
#define DS_CPU_MIPS_TR4101                   0x24

#define DS_CPU_MIPS_VR5000                   0x40

/*
** PowerPC CPU minor types.
*/

#define DS_CPU_PPC_403                       0x00

#define DS_CPU_PPC_5XX                       0x20    /* used when not precisely known */
#define DS_CPU_PPC_505                       0x21
#define DS_CPU_PPC_509                       0x22
#define DS_CPU_PPC_50X                       0x27    /* used when not precisely known */
#define DS_CPU_PPC_555                       0x28

#define DS_CPU_PPC_603E                      0x40

#define DS_CPU_PPC_7XX                       0x50    /* used when not precisely known */
#define DS_CPU_PPC_740                       0x51
#define DS_CPU_PPC_750                       0x52
#define DS_CPU_PPC_7400                      0x53
#define DS_CPU_PPC_7410                      0x54

#define DS_CPU_PPC_8260                      0x61

#define DS_CPU_PPC_8XX                       0x80    /* used when not precisely known */
#define DS_CPU_PPC_821                       0x81
#define DS_CPU_PPC_823                       0x82
#define DS_CPU_PPC_860                       0x83


/*
** NEC V8xx CPU minor types.
*/

#define DS_CPU_NEC_V8XX_V810                 0x00
#define DS_CPU_NEC_V8XX_V821                 0x01

#define DS_CPU_NEC_V8XX_V830                 0x10
#define DS_CPU_NEC_V8XX_V831                 0x11
#define DS_CPU_NEC_V8XX_V832                 0x12

#define DS_CPU_NEC_V8XX_V850                 0x20
#define DS_CPU_NEC_V8XX_V853                 0x21
#define DS_CPU_NEC_V8XX_V850E                0x22
#define DS_CPU_NEC_V8XX_V850_SA1             0x23

/*
** M68K/Coldfire CPU minor types.
*/

#define DS_CPU_M68K_68000                    0x00
#define DS_CPU_M68K_68020                    0x02
#define DS_CPU_M68K_68030                    0x04
#define DS_CPU_M68K_68040                    0x06
#define DS_CPU_M68K_68060                    0x08

#define DS_CPU_M68K_68302                    0x20
#define DS_CPU_M68K_68306                    0x22
#define DS_CPU_M68K_68307                    0x24
#define DS_CPU_M68K_68328                    0x26
#define DS_CPU_M68K_68EZ328                  0x28
#define DS_CPU_M68K_68VZ328                  0x29
#define DS_CPU_M68K_68340                    0x2a
#define DS_CPU_M68K_68360                    0x2c

#define DS_CPU_M68K_CF5102                   0x80
#define DS_CPU_M68K_CF5202                   0x82
#define DS_CPU_M68K_CF5204                   0x84
#define DS_CPU_M68K_CF5206                   0x86
#define DS_CPU_M68K_CF5206E                  0x88
#define DS_CPU_M68K_CF5307                   0x8a

/*
** MCore CPU minor types.
*/

#define DS_CPU_MCORE_200                     0x00
#define DS_CPU_MCORE_340                     0x20


typedef ui8 DSSupportMask[32];             /* 256 bits total */

#define DS_MIN_MSG_SUPPORTMASK_LENGTH        DS_MIN_MSG_LENGTH
#define DS_MIN_REPLY_SUPPORTMASK_LENGTH      (DS_MIN_REPLY_LENGTH + sizeof(DSSupportMask) + 1)

#if DS_PROTOCOL == DS_PROTOCOL_RTOS
    #define DS_MIN_MSG_READMEMORY_LENGTH     (DS_MIN_MSG_LENGTH + 15)
#else
    #define DS_MIN_MSG_READMEMORY_LENGTH     (DS_MIN_MSG_LENGTH + 7)
#endif
#define DS_MIN_REPLY_READMEMORY_LENGTH       (DS_MIN_REPLY_LENGTH + 2)

#if DS_PROTOCOL == DS_PROTOCOL_RTOS
    #define DS_MIN_MSG_WRITEMEMORY_LENGTH    (DS_MIN_MSG_LENGTH + 15)
#else
    #define DS_MIN_MSG_WRITEMEMORY_LENGTH    (DS_MIN_MSG_LENGTH + 7)
#endif
#define DS_MIN_REPLY_WRITEMEMORY_LENGTH      (DS_MIN_REPLY_LENGTH + 2)

//Memory commands options
#define DS_MSG_MEMORY_SEGMENTED 0x01 /* non-flat addr space */
#define DS_MSG_MEMORY_EXTENDED  0x02 /* > 32-bit data addr */
#define DS_MSG_MEMORY_PROTECTED 0x04 /* non-user memory */
#define DS_MSG_MEMORY_USERVIEW  0x08 /* breakpoints are invisible */


#define DS_MSG_MEMORY_SPACE_PROGRAM 0x00
#define DS_MSG_MEMORY_SPACE_DATA    0x40
#define DS_MSG_MEMORY_SPACE_IO      0x80

#define TRK_MSG_HEADER_LENGTH                DS_MSG_MEMORY_SPACE_DATA
#define TRK_MSG_REPLY_HEADER_LENGTH          (TRK_MSG_HEADER_LENGTH + DS_MIN_MSG_LENGTH)

//Others

#define DS_MSG_MEMORY_SPACE_MASK    0xC0

#define DS_MAXREADWRITELENGTH   0x0800  /* 2K data portion */
#define DS_MAXMESSAGESIZE (DS_MAXREADWRITELENGTH + 0x80)
                                            /* max size of all message including cmd hdr */
#define MAXMESSAGESIZE DS_MAXMESSAGESIZE

#define DS_MIN_MSG_FLUSHCACHE_LENGTH         (DS_MIN_MSG_LENGTH + 9)
#define DS_MIN_REPLY_FLUSHCACHE_LENGTH       (DS_MIN_REPLY_LENGTH)

#if DS_PROTOCOL == DS_PROTOCOL_RTOS
    #define DS_MIN_MSG_READREGISTERS_LENGTH  (DS_MIN_MSG_LENGTH + 13)
#else
    #define DS_MIN_MSG_READREGISTERS_LENGTH  (DS_MIN_MSG_LENGTH + 5)
#endif
#define DS_MIN_REPLY_READREGISTERS_LENGTH    DS_MIN_REPLY_LENGTH

#if DS_PROTOCOL == DS_PROTOCOL_RTOS
    #define DS_MIN_MSG_WRITEREGISTERS_LENGTH (DS_MIN_MSG_LENGTH + 13)
#else
    #define DS_MIN_MSG_WRITEREGISTERS_LENGTH (DS_MIN_MSG_LENGTH + 5)
#endif
#define DS_MIN_REPLY_WRITEREGISTERS_LENGTH   DS_MIN_REPLY_LENGTH

//Register commands options
typedef enum {
    kDSRegistersDefault   = 0x0,        /* Default register block */
    kDSRegistersFP        = 0x1,        /* floating point registers */
    kDSRegistersExtended1 = 0x2,        /* Extended register set 1 */
    kDSRegistersExtended2 = 0x3         /* Extended register set 2 */
} DSMessageRegisterOptions;


#if DS_PROTOCOL == DS_PROTOCOL_RTOS
    #define DS_MIN_MSG_STEP_COUNT_LENGTH     (DS_MIN_MSG_LENGTH + 10)
    #define DS_MIN_MSG_STEP_RANGE_LENGTH     (DS_MIN_MSG_LENGTH + 17)
#else
    #define DS_MIN_MSG_STEP_COUNT_LENGTH     (DS_MIN_MSG_LENGTH + 2)
    #define DS_MIN_MSG_STEP_RANGE_LENGTH     (DS_MIN_MSG_LENGTH + 9)
#endif
#define DS_MIN_MSG_STEP_LENGTH               (DS_MIN_MSG_STEP_COUNT_LENGTH)
#define DS_MIN_REPLY_STEP_LENGTH             DS_MIN_REPLY_LENGTH

//Step command options
typedef enum {
    kDSStepIntoCount = 0x00,         /* Exec count instructions & stop */
    kDSStepIntoRange = 0x01,         /* Exec until PC is out of specified range */
    kDSStepOverCount = 0x10,         /* Step over 1*count instructions & stop */
    kDSStepOverRange = 0x11          /* Step over until PC is out of specified range */
} DSMessageStepOptions;


/*
** These are the predefined file handles.  Additional files may
** be opened via the kDSOpenFile command.  New file handles
** returned by that command may be used in addition to the
** ones defined here.
*/

typedef enum DSFileHandle
{
    kDSStdin  = 0x00,
    kDSStdout = 0x01,
    kDSStderr = 0x02
} DSFileHandle;

/*
** The result of an I/O command can be any one of the following.
*/

typedef enum DSIOResult
{
    kDSIONoError    = 0x00,
    kDSIOError      = 0x01,
    kDSIOEOF        = 0x02
} DSIOResult;


//Reply errors
typedef enum {
    kDSReplyNoError = 0x00,
    kDSReplyError = 0x01,
    kDSReplyPacketSizeError = 0x02,
    kDSReplyCWDSError = 0x03,
    kDSReplyEscapeError = 0x04,
    kDSReplyBadFCS = 0x05,
    kDSReplyOverflow = 0x06,
    kDSReplySequenceMissing = 0x07,
    kDSReplyUnsupportedCommandError = 0x10,
    kDSReplyParameterError = 0x11,
    kDSReplyUnsupportedOptionError = 0x12,
    kDSReplyInvalidMemoryRange = 0x13,
    kDSReplyInvalidRegisterRange = 0x14,
    kDSReplyCWDSException = 0x15,
    kDSReplyNotStopped = 0x16,
    kDSReplyBreakpointsFull = 0x17,
    kDSReplyBreakpointConflict = 0x18,
    kDSReplyOsError = 0x20,
    kDSReplyInvalidProcessId = 0x21,
    kDSReplyInvalidThreadId = 0x22,
    kDSDebugSecurityError = 0x23
} DSReplyError;

#endif
/* end "PowerPC_EABI_Support/MetroTRK/msgcmd.h" */
/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MetroTRK/trk.h" line 4 "PowerPC_EABI_Support/MetroTRK/dserror.h" */
#ifndef METROTRK_DSERROR
#define METROTRK_DSERROR

#ifdef __cplusplus
extern "C" {
#endif // ifdef __cplusplus

typedef enum {
    kNoError = 0,
    kStepError = 1,
    kParameterError = 2,
    kEventQueueFull = 0x100,
    kNoMessageBufferAvailable = 0x300,
    kMessageBufferOverflow = 0x301,
    kMessageBufferReadError = 0x302,
    kDispatchError = 0x500,
    kInvalidMemory = 0x700,
    kInvalidRegister = 0x701,
    kCWDSException = 0x702,
    kUnsupportedError = 0x703,
    kInvalidProcessId = 0x704,
    kInvalidThreadId = 0x705,
    kOsError = 0x706,
    kWaitACKError = 0x800
} DSError;

#ifdef __cplusplus
};
#endif // ifdef __cplusplus

#endif
/* end "PowerPC_EABI_Support/MetroTRK/dserror.h" */
/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MetroTRK/trk.h" line 5 "PowerPC_EABI_Support/MetroTRK/UART.h" */
#ifndef _UART_H
#define _UART_H

typedef int UARTError;

 enum {
    kUARTNoError = 0,
    kUARTUnknownBaudRate,
    kUARTConfigurationError,
    kUARTBufferOverflow, /* specified buffer was too small */
    kUARTNoData          /* no data available from polling */
};

typedef enum {
    kBaudHWSet  = -1,  /* use HW settings such as DIP switches */
    kBaud300    = 300, /* valid baud rates */
    kBaud600    = 600,
    kBaud1200   = 1200,
    kBaud1800   = 1800,
    kBaud2000   = 2000,
    kBaud2400   = 2400,
    kBaud3600   = 3600,
    kBaud4800   = 4800,
    kBaud7200   = 7200,
    kBaud9600   = 9600,
    kBaud19200  = 19200,
    kBaud38400  = 38400,
    kBaud57600  = 57600,
    kBaud115200 = 115200,
    kBaud230400 = 230400
} UARTBaudRate;

extern UARTError InitializeUART(UARTBaudRate baudRate);
extern UARTError WriteUARTN(const void* bytes, unsigned long length);

#endif
/* end "PowerPC_EABI_Support/MetroTRK/UART.h" */

#endif
/* end "PowerPC_EABI_Support/MetroTRK/trk.h" */
/* "libs/PowerPC_EABI_Support/src/MetroTRK/dolphin_trk.c" line 1 "PowerPC_EABI_Support/MetroTRK/mem_TRK.h" */
#ifndef METROTRK_MEM_TRK_H
#define METROTRK_MEM_TRK_H

/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MetroTRK/mem_TRK.h" line 3 "PowerPC_EABI_Support/MetroTRK/dstypes.h" */
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
/* "libs/PowerPC_EABI_Support/src/MetroTRK/dolphin_trk.c" line 2 "PowerPC_EABI_Support/MetroTRK/dolphin_trk.h" */
#ifndef METROTRK_DOLPHIN_TRK_H
#define METROTRK_DOLPHIN_TRK_H

#ifdef __cplusplus
extern "C" {
#endif

void InitMetroTRK(void);
void InitMetroTRK_BBA(void);

void EnableMetroTRKInterrupts(void);

#ifdef __cplusplus
}
#endif
#endif
/* end "PowerPC_EABI_Support/MetroTRK/dolphin_trk.h" */
// Local declarations from dolphin_trk_glue.h (full header drags in
// <revolution/OS.h> -> OSFastCast.h statics that retail link-strips).
void InitMetroTRKCommTable(int);
void EnableEXI2Interrupts();
/* "libs/PowerPC_EABI_Support/src/MetroTRK/dolphin_trk.c" line 7 "revolution/os/OSInterrupt.h" */
#ifndef RVL_SDK_OS_INTERRUPT_H
#define RVL_SDK_OS_INTERRUPT_H
/* "libs/RVL_SDK/include/revolution/os/OSInterrupt.h" line 2 "types.h" */
#ifndef TYPES_H
#define TYPES_H

#ifdef __cplusplus
/* "include/types.h" line 4 "cstdarg" */
#ifndef MSL_CPP_CSTDARG_H
#define MSL_CPP_CSTDARG_H
/* "libs/PowerPC_EABI_Support/include/stl/cstdarg" line 2 "stdarg.h" */
#ifndef MSL_STDARG_H
#define MSL_STDARG_H
#ifdef __cplusplus
extern "C" {
#endif

typedef enum _va_arg_type {
    arg_ARGPOINTER,
    arg_WORD,
    arg_DOUBLEWORD,
    arg_ARGREAL
} _va_arg_type;

typedef struct __va_list_struct {
    char gpr;
    char fpr;
    char reserved[2];
    char* input_arg_area;
    char* reg_save_area;
} va_list[1];

void* __va_arg(va_list argp, int type);

#define va_start(VA_LIST, ARG) ((void)ARG, __builtin_va_info(&VA_LIST))
#define va_end(VA_LIST) ((void)VA_LIST)
#define va_arg(VA_LIST, ARG_TYPE)                                              \
    (*(ARG_TYPE*)__va_arg(VA_LIST, _var_arg_typeof(ARG_TYPE)))

#ifdef __cplusplus
}
#endif
#endif
/* end "stdarg.h" */
#ifdef __cplusplus

namespace std {
using ::va_list;
} // namespace std

#endif
#endif
/* end "cstdarg" */
/* "include/types.h" line 5 "cstddef" */
#ifndef MSL_CPP_CSTDDEF_H
#define MSL_CPP_CSTDDEF_H
/* "libs/PowerPC_EABI_Support/include/stl/cstddef" line 2 "stddef.h" */
#ifndef MSL_STDDEF_H
#define MSL_STDDEF_H
#ifdef __cplusplus
extern "C" {
#endif

#ifndef NULL
#define NULL 0
#endif // NULL

#ifndef nullptr
#define nullptr 0
#endif // nullptr

#define offsetof(ST, M) ((size_t) & (((ST*)0)->M))

typedef signed long ptrdiff_t;
typedef unsigned long size_t;

#ifndef __cplusplus
typedef unsigned short wchar_t;
#endif

typedef wchar_t wint_t;

typedef void (*funcptr_t)(void);

#ifdef __cplusplus
}
#endif
#endif
/* end "stddef.h" */
#ifdef __cplusplus

namespace std {
using ::ptrdiff_t;
using ::size_t;
} // namespace std

#endif
#endif
/* end "cstddef" */
/* "include/types.h" line 6 "cstdint" */
#ifndef MSL_CPP_CSTDINT_H
#define MSL_CPP_CSTDINT_H
/* "libs/PowerPC_EABI_Support/include/stl/cstdint" line 2 "stdint.h" */
#ifndef MSL_STDINT_H
#define MSL_STDINT_H

/* "libs/PowerPC_EABI_Support/include/stl/stdint.h" line 3 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif // ifdef __cplusplus

typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;
typedef unsigned long  uint32_t;

typedef signed char int8_t;
typedef short       int16_t;
typedef long        int32_t;

typedef unsigned long long uint64_t;
typedef long long          int64_t;

typedef uint32_t uintptr_t;
typedef int32_t  intptr_t;

#ifdef __cplusplus
};
#endif // ifdef __cplusplus

#endif
/* end "stdint.h" */
#ifdef __cplusplus

namespace std {
using ::uint8_t;
using ::uint16_t;
using ::uint32_t;
using ::int8_t;
using ::int16_t;
using ::int32_t;
using ::uint64_t;
using ::int64_t;
using ::uintptr_t;
using ::intptr_t;
} // namespace std

#endif
#endif
/* end "cstdint" */
/* "include/types.h" line 7 "new" */
#ifndef MSL_CPP_NEW_H
#define MSL_CPP_NEW_H
/* "libs/PowerPC_EABI_Support/include/stl/new" line 2 "stddef.h" */
/* end "stddef.h" */
#ifdef __cplusplus
/* "libs/PowerPC_EABI_Support/include/stl/new" line 4 "exception" */
#ifndef MSL_CPP_EXCEPTION_H
#define MSL_CPP_EXCEPTION_H

namespace std{
    class exception{
    public:
        exception(){}
    };

    class bad_exception : public exception {
    public:
        bad_exception(){}
    };

    typedef void (*unexpected_handler)();
    unexpected_handler set_unexpected(unexpected_handler handler);
    void unexpected();

    typedef void (*terminate_handler)();
    terminate_handler set_terminate(terminate_handler handler);
    void terminate();

}

#endif
/* end "exception" */

namespace std{
    class bad_alloc : public exception {
    public:
        virtual ~bad_alloc(){}
        virtual const char* what() const{
            return "bad_alloc";
        }
    };

    struct nothrow_t {};
    extern const nothrow_t nothrow;
}

inline void* operator new(size_t size, void* ptr) {
#pragma unused(size)
    return ptr;
}

inline void* operator new[](size_t size, void* ptr) {
#pragma unused(size)
    return ptr;
}

#endif
#endif
/* end "new" */
#else
/* "include/types.h" line 9 "stdarg.h" */
/* end "stdarg.h" */
/* "include/types.h" line 10 "stddef.h" */
/* end "stddef.h" */
/* "include/types.h" line 11 "stdint.h" */
/* end "stdint.h" */
#endif

/* "include/types.h" line 14 "compat.h" */
/**
 * Compatability macros for language parsers such as Clangd and Intellisense
 */

#ifndef COMPAT_H
#define COMPAT_H

/******************************************************************************
 *
 * Detect language parsers
 *
 ******************************************************************************/

// Detect Visual Studio Intellisense
#if defined(__INTELLISENSE__)
#define COMPAT_INTELLISENSE
#endif

// Detect Clangd
#if defined(__clang__)
#define COMPAT_CLANGD
#endif

// Define macro to test for ANY language parser
#if defined(COMPAT_INTELLISENSE) || defined(COMPAT_CLANGD)
#define COMPAT_ANY
#endif

/******************************************************************************
 *
 * Common parser fixes
 *
 ******************************************************************************/

#if defined(COMPAT_ANY)

// Hide assembly keywords
#define register
#define asm

// Hide inline assembly
#define ASM(...)
#define ASM_VOLATILE(...)

// Hide attributes
#define __attribute__(...)
#define __declspec(...)

// Fix decltype
#define DECLTYPE(x) __decltype(x)

// Hide memory-mapped symbols
#define DECL_ADDRESS(...)

// Hide flexible arrays
#define FLEXIBLE_ARRAY(NAME) NAME[0]

// Fix __vec2x32float__
typedef int __vec2x32float__;

#else

// Keep inline assembly
#define ASM asm
#define ASM_VOLATILE asm volatile

// decltype
#define DECLTYPE(x) __decltype__(x)

// Place a symbol at a specific address
#define DECL_ADDRESS(addr) : addr

// Allow flexible arrays
#define FLEXIBLE_ARRAY(NAME) NAME[]

#endif

/******************************************************************************
 *
 * Intellisense fixes
 *
 ******************************************************************************/

#if defined(COMPAT_INTELLISENSE)

// Redirect compiler intrinsics
#define __alloca(...)
#define __memclr(...)

#endif

/******************************************************************************
 *
 * Clangd fixes
 *
 ******************************************************************************/

#if defined(COMPAT_CLANGD)

// Redirect compiler intrinsics
#define __alloca __builtin_alloca
#define __memclr(var, size) __builtin_memset((var), 0, size)
#define offsetof __builtin_offsetof

// Fix variadic argument macros
// TODO(kiwi) gotta be a less intrusive way
#undef va_start
#define va_start(...)
#undef va_arg
#define va_arg(v, t) t()

#endif

/******************************************************************************
 *
 * PowerPC compiler intrinsics
 *
 ******************************************************************************/

#if defined(COMPAT_ANY)

// Hide PowerPC intrinsics
#define __abs(...) 0
#define __cntlzw(...) 0
#define __dcbf(...)
#define __dcbst(...)
#define __dcbt(...)
#define __dcbtst(...)
#define __dcbz(...)
#define __divw(...) 0
#define __divwu(...) 0
#define __eieio(...)
#define __fabs(...) 0
#define __fabsf(...) 0
#define __fmadd(...) 0
#define __fmadds(...) 0
#define __fmsub(...) 0
#define __fmsubs(...) 0
#define __fnabs(...) 0
#define __fnabsf(...) 0
#define __fnmadd(...) 0
#define __fnmadds(...) 0
#define __fnmsub(...) 0
#define __fnmsubs(...) 0
#define __fres(...) 0
#define __frsqrte(...) 0
#define __fsel(...) 0
#define __isync(...)
#define __labs(...) 0
#define __lhbrx(...) 0
#define __lwbrx(...) 0
#define __mffs(...) 0
#define __mulhw(...) 0
#define __mulhwu(...) 0
#define __rlwimi(...) 0
#define __rlwinm(...) 0
#define __rlwnm(...) 0
#define __setflm(...)
#define __sthbrx(...)
#define __strcpy(...)
#define __stwbrx(...)
#define __sync(...)

#endif

#endif
/* end "compat.h" */
/* "include/types.h" line 15 "decomp.h" */
/**
 * Codewarrior tricks for matching decomp
 * (Macros generate prototypes to satisfy -requireprotos)
 */

#ifndef DECOMP_H
#define DECOMP_H

/* "include/decomp.h" line 8 "macros.h" */
/**
 * Common macros
 */

#ifndef MACROS_H
#define MACROS_H

/******************************************************************************
 *
 * Strings
 *
 ******************************************************************************/

// Stringify expression
#define __STR(x) #x
#define STR(x) __STR(x)

// Concatenate strings
#define __CONCAT(x, y) x##y
#define CONCAT(x, y) __CONCAT(x, y)

// Multi-character character constants
// clang-format off
#define TWOCC(c0, c1)                                                          \
    (u32)((c0 & 0xFF) << 8  | (c1 & 0xFF))
#define THREECC(c0, c1, c2)                                                    \
    (u32)((c0 & 0xFF) << 16 | (c1 & 0xFF) << 8  | (c2 & 0xFF))
#define FOURCC(c0, c1, c2, c3)                                                 \
    (u32)((c0 & 0xFF) << 24 | (c1 & 0xFF) << 16 | (c2 & 0xFF) << 8 | (c3 & 0xFF))
// clang-format on

/******************************************************************************
 *
 * Arithmetic
 *
 ******************************************************************************/

// Min/max expression
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN(x, y) ((x) < (y) ? (x) : (y))

// Clamp to a range
#define CLAMP(low, high, x)                                                    \
    ((x) > (high) ? (high) : ((x) < (low) ? (low) : (x)))

// Round up value
#define ROUND_UP(x, align) (((x) + (align) - 1) & (-(align)))
#define ROUND_UP_PTR(x, align)                                                 \
    ((void*)((((u32)(x)) + (align) - 1) & (~((align) - 1))))

// Round down value
#define ROUND_DOWN(x, align) ((x) & (-(align)))
#define ROUND_DOWN_PTR(x, align) ((void*)(((u32)(x)) & (~((align) - 1))))

// Distance between pointers
#define PTR_DISTANCE(start, end) ((u8*)(end) - (u8*)(start))

/******************************************************************************
 *
 * Arrays
 *
 ******************************************************************************/

// Size of compile-time arrays
#define ARRAY_SIZE(x) (sizeof((x)) / sizeof((x)[0]))
#define LENGTHOF(x) ARRAY_SIZE(x)

// Declare an array of hardware registers
#define DECL_HW_REGS(NAME) FLEXIBLE_ARRAY(NAME##_HW_REGS)

/******************************************************************************
 *
 * Intrinsics
 *
 ******************************************************************************/

// Memory clear intrinsic
#define MEMCLR(x) __memclr((x), sizeof(*(x)))

/******************************************************************************
 *
 * Attributes
 *
 ******************************************************************************/

// Alignment attribute
#define ALIGN(x) __attribute__((aligned(x)))

// Place a symbol in a specific ELF section
#define DECL_SECTION(x) __declspec(section x)

// Give a symbol weak linkage
#define DECL_WEAK __declspec(weak)

#endif
/* end "macros.h" */

// Compile without matching hacks.
#if defined(NONMATCHING) || defined(COMPAT_ANY)
#define DECOMP_FORCEACTIVE(module, ...)
#define DECOMP_FORCELITERAL(module, ...)
#define DECOMP_FORCEACTIVE_DTOR(module, cls)
#define DECOMP_INLINE
#define DECOMP_DONT_INLINE
#define DECOMP_PPC_RLWINM(value, rot, mb, me) ((value) << (rot))
#define DECOMP_PPC_SHL1_U32(value) ((value) << 1)
#define DECOMP_ASM_INSN_BEGIN
#define DECOMP_ASM_INSN_END
// Compile with matching hacks.
// (This version of CW does not support pragmas inside macros.)
#else
// Force reference specific data
#define DECOMP_FORCEACTIVE(module, ...)                                        \
    void fake_function(...);                                                   \
    void CONCAT(FORCEACTIVE##module, __LINE__)(void);                          \
    void CONCAT(FORCEACTIVE##module, __LINE__)(void) {                         \
        fake_function(__VA_ARGS__);                                            \
    }

// Force literal ordering, such as floats in sdata2
#define DECOMP_FORCELITERAL(module, ...)                                       \
    void CONCAT(FORCELITERAL##module, __LINE__)(void);                         \
    void CONCAT(FORCELITERAL##module, __LINE__)(void) {                        \
        (__VA_ARGS__);                                                         \
    }

// Force reference destructor
#define DECOMP_FORCEACTIVE_DTOR(module, cls)                                   \
    void CONCAT(FORCEDTOR##module##cls, __LINE__)(void);                       \
    void CONCAT(FORCEDTOR##module##cls, __LINE__)(void) {                      \
        cls dummy;                                                             \
        dummy.~cls();                                                          \
    }

#define DECOMP_INLINE inline
#define DECOMP_DONT_INLINE __attribute__((never_inline))

/**
 * MWCC PPC rotate-mask intrinsics (PLAN.md section 17.6).
 * Same builtin family as SDK __rlwimi / __rlwinm; counts as high-level C, not asm.
 */
#define DECOMP_PPC_RLWINM(value, rot, mb, me) __rlwinm((value), (rot), (mb), (me))
/** slwi expansion: rlwinm rD,rA,1,0,30 */
#define DECOMP_PPC_SHL1_U32(value) DECOMP_PPC_RLWINM((value), 1, 0, 30)

/**
 * Markers for single-instruction asm carve-out (PLAN.md section 17.6).
 * Place MWCC asm { } between BEGIN and END; log policy_exception in attempts.jsonl.
 */
#define DECOMP_ASM_INSN_BEGIN
#define DECOMP_ASM_INSN_END

#endif

#endif
/* end "decomp.h" */
/* "include/types.h" line 16 "lang.h" */
/**
 * Compatability macros for deprecated/future C++ features
 */

#ifndef LANG_H
#define LANG_H

#if __cplusplus < 201103L
#define noexcept throw()
#define override
#endif

#endif
/* end "lang.h" */
/* "include/types.h" line 17 "macros.h" */
/**
 * Common macros
 */

#ifndef MACROS_H
#define MACROS_H

/******************************************************************************
 *
 * Strings
 *
 ******************************************************************************/

// Stringify expression
#define __STR(x) #x
#define STR(x) __STR(x)

// Concatenate strings
#define __CONCAT(x, y) x##y
#define CONCAT(x, y) __CONCAT(x, y)

// Multi-character character constants
// clang-format off
#define TWOCC(c0, c1)                                                          \
    (u32)((c0 & 0xFF) << 8  | (c1 & 0xFF))
#define THREECC(c0, c1, c2)                                                    \
    (u32)((c0 & 0xFF) << 16 | (c1 & 0xFF) << 8  | (c2 & 0xFF))
#define FOURCC(c0, c1, c2, c3)                                                 \
    (u32)((c0 & 0xFF) << 24 | (c1 & 0xFF) << 16 | (c2 & 0xFF) << 8 | (c3 & 0xFF))
// clang-format on

/******************************************************************************
 *
 * Arithmetic
 *
 ******************************************************************************/

// Min/max expression
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN(x, y) ((x) < (y) ? (x) : (y))

// Clamp to a range
#define CLAMP(low, high, x)                                                    \
    ((x) > (high) ? (high) : ((x) < (low) ? (low) : (x)))

// Round up value
#define ROUND_UP(x, align) (((x) + (align) - 1) & (-(align)))
#define ROUND_UP_PTR(x, align)                                                 \
    ((void*)((((u32)(x)) + (align) - 1) & (~((align) - 1))))

// Round down value
#define ROUND_DOWN(x, align) ((x) & (-(align)))
#define ROUND_DOWN_PTR(x, align) ((void*)(((u32)(x)) & (~((align) - 1))))

// Distance between pointers
#define PTR_DISTANCE(start, end) ((u8*)(end) - (u8*)(start))

/******************************************************************************
 *
 * Arrays
 *
 ******************************************************************************/

// Size of compile-time arrays
#define ARRAY_SIZE(x) (sizeof((x)) / sizeof((x)[0]))
#define LENGTHOF(x) ARRAY_SIZE(x)

// Declare an array of hardware registers
#define DECL_HW_REGS(NAME) FLEXIBLE_ARRAY(NAME##_HW_REGS)

/******************************************************************************
 *
 * Intrinsics
 *
 ******************************************************************************/

// Memory clear intrinsic
#define MEMCLR(x) __memclr((x), sizeof(*(x)))

/******************************************************************************
 *
 * Attributes
 *
 ******************************************************************************/

// Alignment attribute
#define ALIGN(x) __attribute__((aligned(x)))

// Place a symbol in a specific ELF section
#define DECL_SECTION(x) __declspec(section x)

// Give a symbol weak linkage
#define DECL_WEAK __declspec(weak)

#endif
/* end "macros.h" */
/* "include/types.h" line 18 "build_settings.h" */
#ifndef BUILD_SETTINGS_H
#define BUILD_SETTINGS_H

//Enable bugfixes if the debug flag is enabled
#if defined(DEBUG) && !defined(BUGFIX)
    #define BUGFIX
#endif

#endif
/* end "build_settings.h" */

typedef unsigned long long u64;
typedef signed long long s64;

typedef unsigned long u32;
typedef signed long s32;

typedef unsigned short u16;
typedef signed short s16;

typedef unsigned char u8;
typedef signed char s8;

typedef float f32;
typedef double f64;

typedef unsigned int uint;

typedef int UNKWORD;
typedef void UNKTYPE;

enum { FALSE, TRUE };
typedef int BOOL;

#endif
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations
typedef struct OSContext;

// Create mask from interrupt ID
#define OS_INTR_MASK(intr) (1 << (31 - intr))

typedef enum {
    OS_INTR_MEM_0,
    OS_INTR_MEM_1,
    OS_INTR_MEM_2,
    OS_INTR_MEM_3,
    OS_INTR_MEM_ADDRESS,
    OS_INTR_DSP_AI,
    OS_INTR_DSP_ARAM,
    OS_INTR_DSP_DSP,
    OS_INTR_AI_AI,
    OS_INTR_EXI_0_EXI,
    OS_INTR_EXI_0_TC,
    OS_INTR_EXI_0_EXT,
    OS_INTR_EXI_1_EXI,
    OS_INTR_EXI_1_TC,
    OS_INTR_EXI_1_EXT,
    OS_INTR_EXI_2_EXI,
    OS_INTR_EXI_2_TC,
    OS_INTR_PI_CP,
    OS_INTR_PI_PE_TOKEN,
    OS_INTR_PI_PE_FINISH,
    OS_INTR_PI_SI,
    OS_INTR_PI_DI,
    OS_INTR_PI_RSW,
    OS_INTR_PI_ERROR,
    OS_INTR_PI_VI,
    OS_INTR_PI_DEBUG,
    OS_INTR_PI_HSP,
    OS_INTR_PI_ACR,
    OS_INTR_28,
    OS_INTR_29,
    OS_INTR_30,
    OS_INTR_31,

    OS_INTR_MAX
} OSInterruptType;

//TODO: is this s16 or s32???
typedef void (*OSInterruptHandler)(s32 intr, struct OSContext* ctx);

extern u32 __OSLastInterruptSrr0;
extern s16 __OSLastInterrupt;
extern s64 __OSLastInterruptTime;

BOOL OSDisableInterrupts(void);
BOOL OSEnableInterrupts(void);
BOOL OSRestoreInterrupts(BOOL status);

OSInterruptHandler __OSSetInterruptHandler(OSInterruptType type,
                                           OSInterruptHandler handler);
OSInterruptHandler __OSGetInterruptHandler(OSInterruptType type);

void __OSInterruptInit(void);

u32 __OSMaskInterrupts(u32 userMask);
u32 __OSUnmaskInterrupts(u32 userMask);
void __OSDispatchInterrupt(u8 intr, struct OSContext* ctx);

void __RAS_OSDisableInterrupts_begin(void);
void __RAS_OSDisableInterrupts_end(void);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/os/OSInterrupt.h" */
/* "libs/PowerPC_EABI_Support/src/MetroTRK/dolphin_trk.c" line 8 "revolution/os/OSReset.h" */
#ifndef RVL_SDK_OS_RESET_H
#define RVL_SDK_OS_RESET_H
/* "libs/RVL_SDK/include/revolution/os/OSReset.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

typedef BOOL (*OSShutdownFunction)(BOOL final, u32 event);

typedef enum {
    OS_SD_EVENT_FATAL,
    OS_SD_EVENT_1,
    OS_SD_EVENT_SHUTDOWN,
    OS_SD_EVENT_3,
    OS_SD_EVENT_RESTART,
    OS_SD_EVENT_RETURN_TO_MENU,
    OS_SD_EVENT_LAUNCH_APP,
} OSShutdownEvent;

typedef struct OSShutdownFunctionInfo {
    OSShutdownFunction func;             // at 0x0
    u32 prio;                            // at 0x4
    struct OSShutdownFunctionInfo* next; // at 0x8
    struct OSShutdownFunctionInfo* prev; // at 0xC
} OSShutdownFunctionInfo;

typedef struct OSShutdownFunctionQueue {
    OSShutdownFunctionInfo* head; // at 0x0
    OSShutdownFunctionInfo* tail; // at 0x4
} OSShutdownFunctionQueue;

void OSRegisterShutdownFunction(OSShutdownFunctionInfo* info);
BOOL __OSCallShutdownFunctions(u32 pass, u32 event);
void __OSShutdownDevices(u32 event);
void OSShutdownSystem(void);
void OSRestart(u32 resetCode);
void __OSReturnToMenu(u8 menuMode);
void OSReturnToMenu(void);
void __OSReturnToMenuForError(void);
void __OSHotResetForError(void);
u32 OSGetResetCode(void);
void OSResetSystem(BOOL reset, u32 resetCode, BOOL forceMenu);
extern volatile BOOL __OSIsReturnToIdle;

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/os/OSReset.h" */
/* "libs/PowerPC_EABI_Support/src/MetroTRK/dolphin_trk.c" line 9 "PowerPC_EABI_Support/MetroTRK/__exception.h" */
#ifndef _TRK_EXCEPTION_H
#define _TRK_EXCEPTION_H

extern ui8 gTRKInterruptVectorTable[];

#endif
/* end "PowerPC_EABI_Support/MetroTRK/__exception.h" */
/* "libs/PowerPC_EABI_Support/src/MetroTRK/dolphin_trk.c" line 10 "PowerPC_EABI_Support/MetroTRK/mpc_7xx_603e.h" */
#ifndef TRK_MPC_7XX_603E
#define TRK_MPC_7XX_603E

/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MetroTRK/mpc_7xx_603e.h" line 3 "PowerPC_EABI_Support/MetroTRK/dstypes.h" */
/* end "PowerPC_EABI_Support/MetroTRK/dstypes.h" */

#ifdef __cplusplus
extern "C"{
#endif

void TRKSaveExtended1Block();
void TRKRestoreExtended1Block();

#ifdef __cplusplus
}
#endif
#endif
/* end "PowerPC_EABI_Support/MetroTRK/mpc_7xx_603e.h" */
/* "libs/PowerPC_EABI_Support/src/MetroTRK/dolphin_trk.c" line 11 "PowerPC_EABI_Support/MetroTRK/Processor/ppc/Generic/ppc_targimpl.h" */
#ifndef METROTRK_PPC_TARGIMPL
#define METROTRK_PPC_TARGIMPL

/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MetroTRK/Processor/ppc/Generic/ppc_targimpl.h" line 3 "PowerPC_EABI_Support/MetroTRK/dstypes.h" */
/* end "PowerPC_EABI_Support/MetroTRK/dstypes.h" */
/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MetroTRK/Processor/ppc/Generic/ppc_targimpl.h" line 4 "PowerPC_EABI_Support/MetroTRK/trk.h" */
/* end "PowerPC_EABI_Support/MetroTRK/trk.h" */
/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MetroTRK/Processor/ppc/Generic/ppc_targimpl.h" line 5 "PowerPC_EABI_Support/MetroTRK/Processor/ppc/Board/dolphin/target.h" */
#ifndef METROTRK_DOLPHIN_TARGET
#define METROTRK_DOLPHIN_TARGET

/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MetroTRK/Processor/ppc/Board/dolphin/target.h" line 3 "PowerPC_EABI_Support/MetroTRK/Processor/ppc/Generic/ppc_version.h" */
#ifndef TRK_PPC_TARGIMPL
#define TRK_PPC_TARGIMPL

#define DS_KERNEL_MAJOR_VERSION     0
#define DS_KERNEL_MINOR_VERSION     4

#endif/* end "PowerPC_EABI_Support/MetroTRK/Processor/ppc/Generic/ppc_version.h" */
/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MetroTRK/Processor/ppc/Board/dolphin/target.h" line 4 "PowerPC_EABI_Support/MetroTRK/UART.h" */
/* end "PowerPC_EABI_Support/MetroTRK/UART.h" */
/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MetroTRK/Processor/ppc/Board/dolphin/target.h" line 5 "PowerPC_EABI_Support/MetroTRK/msgcmd.h" */
/* end "PowerPC_EABI_Support/MetroTRK/msgcmd.h" */


#define DS_TARGET_NAME "Revolution"

#define TRK_BAUD_RATE kBaud38400

#ifndef DS_PROTOCOL
    #define DS_PROTOCOL DS_PROTOCOL_BOARD
#endif

#define BOARD_WELCOME_STRING "MetroTRK for Revolution v0.4"

#define TRK_TRANSPORT_INT_DRIVEN 1

#define TRK_TRANSPORT_INT_KEY 0x00000500

//Target specific includes
/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MetroTRK/Processor/ppc/Board/dolphin/target.h" line 23 "PowerPC_EABI_Support/MetroTRK/Processor/ppc/Board/dolphin/target_asm.h" */
#ifndef TRK_DOLPHIN_TARGET_ASM
#define TRK_DOLPHIN_TARGET_ASM

#define EXCEPTION_SET_MSR_BITS 0x30

/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MetroTRK/Processor/ppc/Board/dolphin/target_asm.h" line 5 "PowerPC_EABI_Support/MetroTRK/Processor/ppc/Generic/proc_mpc_7xx_603e.h" */
#ifndef TRK_PROC_MPC_7XX_603E_H
#define TRK_PROC_MPC_7XX_603E_H

#define __PPC_FPU__         1
#define __PPC_FPU_FPECR__   0
#define __PPC_DCACHE__      1
#define __PPC_ICACHE__      1
#define __PPC_BOOT_CONFIG__ 0

#define PPC_INSTR_SKIP_EXCEPTION1 0x1000
#define PPC_INSTR_SKIP_EXCEPTION2 0x1100
#define PPC_INSTR_SKIP_EXCEPTION3 0x1200
#define PPC_INSTR_SKIP_EXCEPTION4 0x1300

#define __PPC_TGPR__ 1


#endif
/* end "PowerPC_EABI_Support/MetroTRK/Processor/ppc/Generic/proc_mpc_7xx_603e.h" */

#endif
/* end "PowerPC_EABI_Support/MetroTRK/Processor/ppc/Board/dolphin/target_asm.h" */
/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MetroTRK/Processor/ppc/Board/dolphin/target.h" line 24 "PowerPC_EABI_Support/MetroTRK/Processor/ppc/Export/m7xx_m603e_reg.h" */
#ifndef TRK_M7XX_M603E_REG_H
#define TRK_M7XX_M603E_REG_H

/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MetroTRK/Processor/ppc/Export/m7xx_m603e_reg.h" line 3 "PowerPC_EABI_Support/MetroTRK/dstypes.h" */
/* end "PowerPC_EABI_Support/MetroTRK/dstypes.h" */
/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MetroTRK/Processor/ppc/Export/m7xx_m603e_reg.h" line 4 "PowerPC_EABI_Support/MetroTRK/Processor/ppc/Export/ppc_reg.h" */
#ifndef METROTRK_PPC_REG
#define METROTRK_PPC_REG

/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MetroTRK/Processor/ppc/Export/ppc_reg.h" line 3 "PowerPC_EABI_Support/MetroTRK/dstypes.h" */
/* end "PowerPC_EABI_Support/MetroTRK/dstypes.h" */
/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MetroTRK/Processor/ppc/Export/ppc_reg.h" line 4 "PowerPC_EABI_Support/MetroTRK/trk.h" */
/* end "PowerPC_EABI_Support/MetroTRK/trk.h" */

/*
 *    Default register block (all registers typically requested by debugger)
 */

typedef ui32 DefaultType;
typedef ui32 Extended1Type;
typedef ui64 FloatType;
typedef FloatType FPType;    /* TRK core calls it FPType */

typedef struct Default_PPC{
    DefaultType GPR[32];
    DefaultType PC;
    DefaultType LR;
    DefaultType CR;
    DefaultType CTR;
    DefaultType XER;
} Default_PPC;

/*
** Register indices
*/
#define TRK_DEFAULT_GPR(x)            (x)
#define TRK_DEFAULT_PC                32
#define TRK_DEFAULT_LR                33
#define TRK_DEFAULT_CR                34
#define TRK_DEFAULT_CTR               35
#define TRK_DEFAULT_XER               36

#define TRK_DEFAULT_MIN_REGISTER      0
#define TRK_DEFAULT_MAX_REGISTER      (sizeof(Default_PPC) / sizeof(DefaultType) - 1)

#define TRK_DEFAULT_SIZE              (sizeof(DefaultType))

typedef struct Float_PPC{
    FloatType FPR[32];
    FloatType FPSCR;
    FloatType FPECR;
} Float_PPC;

/*
** Register indices
*/
#define TRK_FLOAT_FPR(x)               (x)
#define TRK_FLOAT_FPSCR                32
#define TRK_FLOAT_FPECR                33

#define TRK_FLOAT_MIN_REGISTER         0
#define TRK_FLOAT_MAX_REGISTER         (sizeof(Float_PPC) / sizeof(FloatType) - 1)

#define TRK_FLOAT_SIZE                 (sizeof(FloatType))


/*
 * Info stored in NotifyStopped message.
 */

typedef DefaultType    PCType;
typedef ui32            InstructionType;
typedef ui16            ExceptionCauseType;

typedef struct StopInfo_PPC{
    PCType PC;
    InstructionType PCInstruction;
    ExceptionCauseType exceptionID;
} StopInfo_PPC;

//
// Info stored in NotifyException message.
//

typedef StopInfo_PPC ExceptionInfo_PPC;

/*
** Info used by breakpoint code to restore (unset)
** breakpoints.  For software breakpoints it is just
** the saved instruction.
*/

typedef InstructionType BreakpointRestore;

#define TRK_EXTENDED2_SIZE            (sizeof(Extended2Type))


//Special purpose registers
#define SPR_XER                      1

#define SPR_LR                       8
#define SPR_CTR                      9

#define SPR_DSISR                    18
#define SPR_DAR                      19

#define SPR_DEC                      22

#define SPR_SDR1                     25
#define SPR_SRR0                     26
#define SPR_SRR1                     27

#define TBR_TBL_READ                 268
#define TBR_TBU_READ                 269

#define SPR_SPRG0                    272
#define SPR_SPRG1                    273
#define SPR_SPRG2                    274
#define SPR_SPRG3                    275

#define SPR_EAR                      282

#define TBR_TBL_WRITE                284
#define TBR_TBU_WRITE                285

#define SPR_PVR                      287

#define SPR_IBAT0U                   528
#define SPR_IBAT0L                   529
#define SPR_IBAT1U                   530
#define SPR_IBAT1L                   531
#define SPR_IBAT2U                   532
#define SPR_IBAT2L                   533
#define SPR_IBAT3U                   534
#define SPR_IBAT3L                   535

#define SPR_DBAT0U                   536
#define SPR_DBAT0L                   537
#define SPR_DBAT1U                   538
#define SPR_DBAT1L                   539
#define SPR_DBAT2U                   540
#define SPR_DBAT2L                   541
#define SPR_DBAT3U                   542
#define SPR_DBAT3L                   543

#define SPR_IBAT4U                   560
#define SPR_IBAT4L                   561
#define SPR_IBAT5U                   562
#define SPR_IBAT5L                   563
#define SPR_IBAT6U                   564
#define SPR_IBAT6L                   565
#define SPR_IBAT7U                   566
#define SPR_IBAT7L                   567

#define SPR_DBAT4U                   568
#define SPR_DBAT4L                   569
#define SPR_DBAT5U                   570
#define SPR_DBAT5L                   571
#define SPR_DBAT6U                   572
#define SPR_DBAT6L                   573
#define SPR_DBAT7U                   574
#define SPR_DBAT7L                   575

#define SPR_GQR0                     912
#define SPR_GQR1                     913
#define SPR_GQR2                     914
#define SPR_GQR3                     915
#define SPR_GQR4                     916
#define SPR_GQR5                     917
#define SPR_GQR6                     918
#define SPR_GQR7                     919

#define SPR_HID2                     920
#define SPR_WPAR                     921
#define SPR_DMAU                     922
#define SPR_DMAL                     923

#define SPR_UMMCR0                   936
#define SPR_UPMC1                    937
#define SPR_UPMC2                    938
#define SPR_USIA                     939
#define SPR_UMMCR1                   940
#define SPR_UPMC3                    941
#define SPR_UPMC4                    942
#define SPR_USDA                     943

#define SPR_MMCR0                    952
#define SPR_PMC1                     953
#define SPR_PMC2                     954
#define SPR_SIA                      955
#define SPR_MMCR1                    956
#define SPR_PMC3                     957
#define SPR_PMC4                     958
#define SPR_SDA                      959

#define SPR_HID0                     1008
#define SPR_HID1                     1009
#define SPR_IABR                     1010
#define SPR_HID4                     1011
#define SPR_TDCL                     1012
#define SPR_DABR                     1013

#define SPR_L2CR                     1017
#define SPR_TDCH                     1018
#define SPR_ICTC                     1019

#define SPR_THRM1                    1020
#define SPR_THRM2                    1021
#define SPR_THRM3                    1022

#define SPR_FPECR         1022

//MSR Register bits
#define COND_EQ 2
#define MSR_SE            0x0400      /* SE bit of MSR (bit 21) */
#define MSR_EE            0x8000      /* EE bit of MSR (bit 16) */
#define MSR_RI            0x0002      /* RI bit of MSR (bit 30) */
#define MSR_DR            0x0010      /* DR bit of MSR (bit 27) */
#define MSR_IR            0x0020      /* IR bit of MSR (bit 26) */
#define MSR_FP            0x2000      /* FP bit of MSR (bit 18) */
#define MSR_VEC           0x02000000  /* VEC bit of MSR (bit 6) */ /* AltiVec support */

#endif
/* end "PowerPC_EABI_Support/MetroTRK/Processor/ppc/Export/ppc_reg.h" */

typedef struct Extended1_PPC_6xx_7xx{
    Extended1Type SR[16];
    Extended1Type TBL;
    Extended1Type TBU;
    Extended1Type HID0;
    Extended1Type HID1;
    Extended1Type MSR;
    Extended1Type PVR;
    Extended1Type HID2;
    Extended1Type IBAT0U;
    Extended1Type IBAT0L;
    Extended1Type IBAT1U;
    Extended1Type IBAT1L;
    Extended1Type IBAT2U;
    Extended1Type IBAT2L;
    Extended1Type IBAT3U;
    Extended1Type IBAT3L;
    Extended1Type DBAT0U;
    Extended1Type DBAT0L;
    Extended1Type DBAT1U;
    Extended1Type DBAT1L;
    Extended1Type DBAT2U;
    Extended1Type DBAT2L;
    Extended1Type DBAT3U;
    Extended1Type IBAT4U;

    Extended1Type DCMP;
    Extended1Type HASH1;
    Extended1Type HASH2;
    Extended1Type IMISS;
    Extended1Type ICMP;
    Extended1Type RPA;
    Extended1Type SDR1;
    Extended1Type DAR;
    Extended1Type DSISR;
    Extended1Type SPRG0;
    Extended1Type SPRG1;
    Extended1Type SPRG2;
    Extended1Type SPRG3;
    Extended1Type DEC;
    Extended1Type IABR;
    Extended1Type EAR;

    //7455 extension only
    Extended1Type DABR;
    Extended1Type LDSTCR;
    Extended1Type MSSSR0;
    Extended1Type UPMC5;
    Extended1Type UPMC6;
    Extended1Type PMC5;
    Extended1Type PMC6;
    Extended1Type TLBMISS;
    Extended1Type IBAT7L;
    Extended1Type PTELO;
    Extended1Type L3PM;
    Extended1Type ICTC;
    Extended1Type L3CR;
    Extended1Type SRR0;
    Extended1Type DBAT5U;
    Extended1Type UMMCR0;
    Extended1Type IBAT5L;
    Extended1Type IBAT6U;
    Extended1Type IBAT6L;
    Extended1Type IBAT7U;
    Extended1Type PTEHI;
    Extended1Type DBAT4U;
    Extended1Type DBAT5L;
    Extended1Type IBAT4L;
    Extended1Type DBAT4L;
    Extended1Type DBAT6U;
    Extended1Type DBAT6L;
    Extended1Type DBAT7U;
    Extended1Type DBAT7L;
    
    Extended1Type exceptionID;
    Extended1Type GQR;

    Extended1Type DBAT3L;
    Extended1Type DMISS;

    //7455 extension only
    Extended1Type L2CR2;
    Extended1Type MMCR0;
    Extended1Type MMCR1;
    Extended1Type THRM1;
    Extended1Type THRM2;
    Extended1Type THRM3;
    Extended1Type ICCR;
    Extended1Type L2CR;
    ui8 pad[4];
} Extended1_PPC_6xx_7xx;

#define DS_EXTENDED1_MIN_REGISTER_6xx_7xx    0
#define DS_EXTENDED1_MAX_REGISTER_6xx_7xx    (sizeof(Extended1_PPC_6xx_7xx) / sizeof(Extended1Type) - 1)

typedef struct Extended2_PPC_6xx_7xx{
    ui32 PSR[32][2];
} Extended2_PPC_6xx_7xx;

typedef struct ProcessorState_PPC_6xx_7xx{
    Default_PPC Default;
    Float_PPC Float;
    Extended1_PPC_6xx_7xx Extended1;
    Extended2_PPC_6xx_7xx Extended2;
#if TRK_TRANSPORT_INT_DRIVEN
    DefaultType transport_handler_saved_ra;
#endif
} ProcessorState_PPC_6xx_7xx;


typedef ProcessorState_PPC_6xx_7xx ProcessorState_PPC;

#endif
/* end "PowerPC_EABI_Support/MetroTRK/Processor/ppc/Export/m7xx_m603e_reg.h" */
/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MetroTRK/Processor/ppc/Board/dolphin/target.h" line 25 "PowerPC_EABI_Support/MetroTRK/Processor/ppc/Export/ppc_except.h" */
#ifndef METROTRK_PPC_EXCEPT
#define METROTRK_PPC_EXCEPT

#define PPC_SYSTEMRESET              0x0100
#define PPC_MACHINECHECK             0x0200
#define PPC_DATAACCESSERROR          0x0300
#define PPC_INSTACCESSERROR          0x0400
#define PPC_EXTERNALINTERRUPT        0x0500
#define PPC_ALIGNMENTERROR           0x0600
#define PPC_PROGRAMERROR             0x0700
#define PPC_FPUNAVAILABLE            0x0800
#define PPC_DECREMENTERINTERRUPT     0x0900
#define PPC_RESERVED_0A00            0x0A00
#define PPC_RESERVED_0B00            0x0B00
#define PPC_SYSTEMCALL               0x0C00
#define PPC_TRACE                    0x0D00
#define PPC_FPASSIST                 0x0E00

#define PPC_PERFORMANCE_MONITOR      0x0F00    /* PPC 740 and 750 */
#define PPC_ALTIVEC_UNAVAILABLE      0x0F20    /* PPC 7400 (Max processor) */

#define PPC5xx_8xx_SOFTWAREEMULATION 0x1000    /* PPC 505, 821, and 860 */

#define PPC8xx_INSTTLBMISS           0x1100    /* PPC 821 and 860 */
#define PPC8xx_DATATLBMISS           0x1200    /* PPC 821 and 860 */
#define PPC8xx_INSTTLBERROR          0x1300    /* PPC 821, 860, and 555 */
#define PPC8xx_DATATLBERROR          0x1400    /* PPC 821, 860, and 555 */
#define PPC_DENORM_DETECT_JAVA_MODE  0x1600    /* PPC 7400 (Max processor) */

#define PPC603E_INSTR_TLB_MISS       0x1000    /* PPC 603e */
#define PPC603E_DATA_LOAD_TLB_MISS   0x1100    /* PPC 603e */
#define PPC603E_DATA_STORE_TLB_MISS  0x1200    /* PPC 603e */

#define PPC7xx_603E_INSTR_ADDR_BREAK 0x1300    /* PPC 603e, 740, 750 */

#define PPC7xx_603E_SYS_MANAGE       0x1400    /* PPC 603e, 740, 750 */

#define PPC_RESERVED_1500            0x1500
#define PPC_RESERVED_1600            0x1600
#define PPC_THERMAL_MANAGE           0x1700    /* PPC 740 and 750 */
#define PPC_RESERVED_1800            0x1800
#define PPC_RESERVED_1900            0x1900
#define PPC_RESERVED_1A00            0x1A00
#define PPC_RESERVED_1B00            0x1B00
#define PPC_DATABREAKPOINT           0x1C00    /* PPC 821, 860, and 505 */
#define PPC_INSTBREAKPOINT           0x1D00    /* PPC 821, 860, and 505 */
#define PPC_PERIPHERALBREAKPOINT     0x1E00    /* PPC 821, 860, and 505 */
#define PPC_NMDEVELOPMENTPORT        0x1F00    /* PPC 821, 860, and 505 */

#endif
/* end "PowerPC_EABI_Support/MetroTRK/Processor/ppc/Export/ppc_except.h" */

#endif
/* end "PowerPC_EABI_Support/MetroTRK/Processor/ppc/Board/dolphin/target.h" */

#ifdef __cplusplus
extern "C"{
#endif

typedef struct TRKState_PPC {
    DefaultType GPR[32]; //0x0
    DefaultType LR; //0x80
    DefaultType CTR; //0x84
    DefaultType XER; //0x88
    Extended1Type MSR; //0x8c
    Extended1Type DAR; //0x90
    Extended1Type DSISR; //0x94
    bool stopped; //0x98
    bool inputActivated; //0x9c
    ui8* inputPendingPtr; //0xA0
} TRKState_PPC;

extern TRKState_PPC gTRKState;

typedef struct ProcessorRestoreFlags_PPC {
    ui8 TBR;
    ui8 DEC;
    ui8 linker_padding[0x9 - 0x2];
} ProcessorRestoreFlags_PPC;

extern ProcessorRestoreFlags_PPC gTRKRestoreFlags;
extern ProcessorState_PPC gTRKCPUState;

ui32 __TRK_get_MSR();
void __TRK_set_MSR(register ui32 val);
ui32 __TRK_get_PVR();
ui32 __TRK_get_IBAT0U();
ui32 __TRK_get_IBAT0L();
ui32 __TRK_get_IBAT1U();
ui32 __TRK_get_IBAT1L();
ui32 __TRK_get_IBAT2U();
ui32 __TRK_get_IBAT2L();
ui32 __TRK_get_IBAT3U();
ui32 __TRK_get_IBAT3L();
ui32 __TRK_get_DBAT0U();
ui32 __TRK_get_DBAT0L();
ui32 __TRK_get_DBAT1U();
ui32 __TRK_get_DBAT1L();
ui32 __TRK_get_DBAT2U();
ui32 __TRK_get_DBAT2L();
ui32 __TRK_get_DBAT3U();
ui32 __TRK_get_DBAT3L();

DSError TRKPPCAccessSPR(void* srcDestPtr, ui32 spr, bool read);
DSError TRKPPCAccessPairedSingleRegister(void* srcDestPtr, ui32 psr, bool read);
DSError TRKPPCAccessFPRegister(void* srcDestPtr, ui32 fpr, bool read);
DSError TRKPPCAccessSpecialReg(void* srcDestPtr, ui32* instructionData, bool read);
void TRKPostInterruptEvent();
ui32 ConvertAddress(ui32);

#ifdef __cplusplus
}
#endif

#endif
/* end "PowerPC_EABI_Support/MetroTRK/Processor/ppc/Generic/ppc_targimpl.h" */
/* "libs/PowerPC_EABI_Support/src/MetroTRK/dolphin_trk.c" line 12 "PowerPC_EABI_Support/MetroTRK/main_TRK.h" */
#ifndef METROTRK_MAIN_TRK_H
#define METROTRK_MAIN_TRK_H

/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MetroTRK/main_TRK.h" line 3 "PowerPC_EABI_Support/MetroTRK/dstypes.h" */
/* end "PowerPC_EABI_Support/MetroTRK/dstypes.h" */

#ifdef __cplusplus
extern "C"{
#endif

int TRK_main();


#ifdef __cplusplus
}
#endif
#endif
/* end "PowerPC_EABI_Support/MetroTRK/main_TRK.h" */
/* "libs/PowerPC_EABI_Support/src/MetroTRK/dolphin_trk.c" line 13 "PowerPC_EABI_Support/MetroTRK/rvl_mem.h" */
#ifndef __RVL_MEM_H__
#define __RVL_MEM_H__


#define BOOTINFO            0x80000000
#define MEM2_CACHED         0x90000000
#define DB_EXCEPTION_MASK   0x44


#endif // __RVL_MEM_H__
/* end "PowerPC_EABI_Support/MetroTRK/rvl_mem.h" */

#define EXCEPTION_SIZE  0x100
#define NUM_EXCEPTIONS  15

static ui32 TRK_ISR_OFFSETS[NUM_EXCEPTIONS] = {
    PPC_SYSTEMRESET,
    PPC_MACHINECHECK,
    PPC_DATAACCESSERROR,
    PPC_INSTACCESSERROR,
    PPC_EXTERNALINTERRUPT,
    PPC_ALIGNMENTERROR,
    PPC_PROGRAMERROR,
    PPC_FPUNAVAILABLE,
    PPC_DECREMENTERINTERRUPT,
    PPC_SYSTEMCALL,
    PPC_TRACE,
    PPC_PERFORMANCE_MONITOR,
    PPC7xx_603E_INSTR_ADDR_BREAK,
    PPC7xx_603E_SYS_MANAGE,
    PPC_THERMAL_MANAGE
};

enum { HARDWARE_BBA = 2 }; // from dolphin_trk_glue.h (see include note above)

static ui32* lc_base;

/*
 * Wii boot-entry vectors (policy exception, PLAN.md §17.6):
 * InitMetroTRK / InitMetroTRK_BBA are entered by the Wii boot code with a
 * non-standard ABI — no valid stack frame, no return address, hardware ID in
 * r5 — and their retail bodies hand-roll the GPR save/restore (stmw/lmw),
 * MSR/SRR1 and IABR/DABR SPR setup, and a fixed debug-stack switch. MWCC's
 * mandatory frame prologue makes a C reconstruction impossible. The matching
 * build therefore uses whole-function `asm void` + `nofralloc` transcribed
 * from retail for exactly these two boot vectors. Non-MWCC / PC builds select
 * the readable C fallback below (TRK is dead code on PC).
 */
#if defined(NONMATCHING) || defined(COMPAT_ANY) || !defined(__MWERKS__)

static void InitMetroTRK_Common(ui32 hwId) {
    if (InitMetroTRKCommTable(hwId) == 1) {
        return;
    }

    TRK_main();
}

void InitMetroTRK(void) {
    InitMetroTRK_Common(0);
}

void InitMetroTRK_BBA(void) {
    InitMetroTRK_Common(HARDWARE_BBA);
}

#else /* MWCC matching build — boot-entry vectors */

// r5: hardware id (non-standard boot ABI)
asm void InitMetroTRK(){
    nofralloc
    subi r1, r1, 4
    stw r3, 0(r1)
    lis r3, gTRKCPUState@h
    ori r3, r3, gTRKCPUState@l
    stmw r0, 0(r3)                  // Save all GPRs
    lwz r4, 0(r1)
    addi r1, r1, 4
    stw r1, 0x4(r3)                 // GPR[1] = entry stack pointer
    stw r4, 0xc(r3)                 // GPR[3] = original r3
    mflr r4
    stw r4, 0x84(r3)                // LR
    stw r4, 0x80(r3)                // PC
    mfcr r4
    stw r4, 0x88(r3)                // CR
    // Clear external interrupts (MSR_EE = 0), preserve original MSR in SRR1.
    mfmsr r4
    ori r3, r4, MSR_EE
    xori r3, r3, MSR_EE
    mtmsr r3
    mtsrr1 r4
    bl TRKSaveExtended1Block
    lis r3, gTRKCPUState@h
    ori r3, r3, gTRKCPUState@l
    lmw r0, 0(r3)                   // Restore all GPRs
    // Reset IABR and DABR, switch to the debug stack, init comm table.
    li r0, 0
    mtiabr r0
    mtdabr r0
    lis r1, 0x8067
    ori r1, r1, 0xd560
    mr r3, r5
    bl InitMetroTRKCommTable
    cmpwi r3, 1
    bne initCommTableSuccess
    // BUG (retail): reads the return value of InitMetroTRKCommTable as a
    // TRKCPUState pointer, returning to a garbage code address.
    lwz r4, 0x84(r3)
    mtlr r4
    lmw r0, 0(r3)
    blr
initCommTableSuccess:
    b TRK_main
    blr
}

asm void InitMetroTRK_BBA(){
    nofralloc
    subi r1, r1, 4
    stw r3, 0(r1)
    lis r3, gTRKCPUState@h
    ori r3, r3, gTRKCPUState@l
    stmw r0, 0(r3)                  // Save all GPRs
    lwz r4, 0(r1)
    addi r1, r1, 4
    stw r1, 0x4(r3)                 // GPR[1] = entry stack pointer
    stw r4, 0xc(r3)                 // GPR[3] = original r3
    mflr r4
    stw r4, 0x84(r3)                // LR
    stw r4, 0x80(r3)                // PC
    mfcr r4
    stw r4, 0x88(r3)                // CR
    // Turn on external interrupts (MSR_EE = 1), preserve original MSR in SRR1.
    mfmsr r4
    ori r3, r4, MSR_EE
    mtmsr r3
    mtsrr1 r4
    bl TRKSaveExtended1Block
    lis r3, gTRKCPUState@h
    ori r3, r3, gTRKCPUState@l
    lmw r0, 0(r3)                   // Restore all GPRs
    // Reset IABR and DABR, switch to the debug stack, init comm table as BBA.
    li r0, 0
    mtiabr r0
    mtdabr r0
    lis r1, 0x8067
    ori r1, r1, 0xd560
    li r3, 2 // HARDWARE_BBA
    bl InitMetroTRKCommTable
    cmpwi r3, 1
    bne initCommTableSuccess
    // BUG (retail): see InitMetroTRK.
    lwz r4, 0x84(r3)
    mtlr r4
    lmw r0, 0(r3)
    blr
initCommTableSuccess:
    b TRK_main
    blr
}

#endif /* NONMATCHING / COMPAT_ANY / !__MWERKS__ */

void EnableMetroTRKInterrupts(){
    EnableEXI2Interrupts();
}

void* TRKTargetTranslate(ui32* addr) {
    if (addr >= lc_base && addr < &lc_base[0x1000]) {
        if (gTRKCPUState.Extended1.DBAT2L & 3) return addr;
    }
    if ((ui32)addr < 0x3000000) {
        return (void*)(((ui32)addr & 0x3FFFFFFF) | BOOTINFO);
    }
    if ((ui32)addr >= 0x10000000 && 0x1C000000 > (ui32)addr) {
        return (void*)(((ui32)addr & 0x3FFFFFFF) | MEM2_CACHED);
    }
    return addr;
}

void __TRK_copy_vectors(){
    ui32* data_ptr;
    ui32* isrOffsetPtr;
    int i;
    ui32 data;

    if((ui32)lc_base <= DB_EXCEPTION_MASK && (ui32)&lc_base[0x1000] > DB_EXCEPTION_MASK && gTRKCPUState.Extended1.DBAT2L & 0x3){
        data_ptr = (ui32*)DB_EXCEPTION_MASK;
    }else{
        data_ptr = (ui32*)(BOOTINFO + DB_EXCEPTION_MASK);
    }

    isrOffsetPtr = TRK_ISR_OFFSETS;
    i = 0;
    data = *data_ptr;

    do{
        if((data & (1 << i)) != 0 && i != 4){
            {
                ui32 offset;
                void* destPtr;
                offset = *isrOffsetPtr;
                destPtr = (void*)TRKTargetTranslate((ui32*)offset);
                TRK_memcpy(destPtr, (void*)(gTRKInterruptVectorTable + offset), EXCEPTION_SIZE);
                TRK_flush_cache(destPtr, EXCEPTION_SIZE);
            }
        }

        i++;
        isrOffsetPtr++;
    }while(i <= 14);
}

DSError TRKInitializeTarget(){
    gTRKState.stopped = true;
    gTRKState.MSR = __TRK_get_MSR();
    lc_base = (ui32*)0xE0000000;
    return kNoError;
}

void __TRKreset() {
    OSResetSystem(0, 0, 0);
}
