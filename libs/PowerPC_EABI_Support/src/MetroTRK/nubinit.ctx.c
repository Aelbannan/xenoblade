/* "libs/PowerPC_EABI_Support/src/MetroTRK/nubinit.c" line 0 "PowerPC_EABI_Support/MetroTRK/nubinit.h" */
#ifndef TRK_NUBINIT
#define TRK_NUBINIT

/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MetroTRK/nubinit.h" line 3 "PowerPC_EABI_Support/MetroTRK/dstypes.h" */
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
/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MetroTRK/nubinit.h" line 4 "PowerPC_EABI_Support/MetroTRK/trk.h" */
#ifndef _METROTRK_TRK_H
#define _METROTRK_TRK_H

/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MetroTRK/trk.h" line 3 "PowerPC_EABI_Support/MetroTRK/msgcmd.h" */
#ifndef _TRK_MSGCMD_H
#define _TRK_MSGCMD_H

/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MetroTRK/msgcmd.h" line 3 "PowerPC_EABI_Support/MetroTRK/dstypes.h" */
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

#ifdef __cplusplus
extern "C"{
#endif

extern bool gTRKBigEndian;

DSError TRK_InitializeNub(void);
DSError TRK_TerminateNub(void);
void TRK_NubWelcome(void);

#ifdef __cplusplus
}
#endif
#endif
/* end "PowerPC_EABI_Support/MetroTRK/nubinit.h" */
/* "libs/PowerPC_EABI_Support/src/MetroTRK/nubinit.c" line 1 "PowerPC_EABI_Support/MetroTRK/serpoll.h" */
#ifndef TRK_SERPOLL
#define TRK_SERPOLL

/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MetroTRK/serpoll.h" line 3 "PowerPC_EABI_Support/MetroTRK/dstypes.h" */
/* end "PowerPC_EABI_Support/MetroTRK/dstypes.h" */
/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MetroTRK/serpoll.h" line 4 "PowerPC_EABI_Support/MetroTRK/trk.h" */
/* end "PowerPC_EABI_Support/MetroTRK/trk.h" */

#ifdef __cplusplus
extern "C"{
#endif

extern void* gTRKInputPendingPtr;

void TRKGetInput();
void TRKProcessInput(int);
DSError TRKInitializeSerialHandler();
DSError TRKTerminateSerialHandler();

#ifdef __cplusplus
}
#endif
#endif
/* end "PowerPC_EABI_Support/MetroTRK/serpoll.h" */
/* "libs/PowerPC_EABI_Support/src/MetroTRK/nubinit.c" line 2 "PowerPC_EABI_Support/MetroTRK/targimpl.h" */
#ifndef TRK_TARGIMPL
#define TRK_TARGIMPL

/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MetroTRK/targimpl.h" line 3 "PowerPC_EABI_Support/MetroTRK/dstypes.h" */
/* end "PowerPC_EABI_Support/MetroTRK/dstypes.h" */
/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MetroTRK/targimpl.h" line 4 "PowerPC_EABI_Support/MetroTRK/trk.h" */
/* end "PowerPC_EABI_Support/MetroTRK/trk.h" */
/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MetroTRK/targimpl.h" line 5 "PowerPC_EABI_Support/MetroTRK/nubevent.h" */
#ifndef TRK_NUBEVENT
#define TRK_NUBEVENT

/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MetroTRK/nubevent.h" line 3 "PowerPC_EABI_Support/MetroTRK/dstypes.h" */
/* end "PowerPC_EABI_Support/MetroTRK/dstypes.h" */
/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MetroTRK/nubevent.h" line 4 "PowerPC_EABI_Support/MetroTRK/trk.h" */
/* end "PowerPC_EABI_Support/MetroTRK/trk.h" */
/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MetroTRK/nubevent.h" line 5 "PowerPC_EABI_Support/MetroTRK/msgbuf.h" */
#ifndef TRK_MSGBUF
#define TRK_MSGBUF

/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MetroTRK/msgbuf.h" line 3 "PowerPC_EABI_Support/MetroTRK/dstypes.h" */
/* end "PowerPC_EABI_Support/MetroTRK/dstypes.h" */
/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MetroTRK/msgbuf.h" line 4 "PowerPC_EABI_Support/MetroTRK/trk.h" */
/* end "PowerPC_EABI_Support/MetroTRK/trk.h" */
/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MetroTRK/msgbuf.h" line 5 "stddef.h" */
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


typedef int MessageBufferID;

enum
{
    kInvalidMessageBufferId = -1
};

#define kMessageBufferSize 0x800 + 0x80
#define NUM_BUFFERS 3

typedef struct MessageBuffer {
    bool fInUse; //0x0
    ui32 fLength; //0x4
    ui32 fPosition; //0x8
    ui8 fData[kMessageBufferSize]; //0xC
} MessageBuffer;


DSError TRK_InitializeMessageBuffers();
DSError TRK_GetFreeBuffer(int*, MessageBuffer**);
MessageBuffer* TRKGetBuffer(int);
void TRK_ReleaseBuffer(int);
void TRKResetBuffer(MessageBuffer*, bool);
DSError TRK_SetBufferPosition(MessageBuffer*, ui32);
DSError TRK_AppendBuffer(MessageBuffer*, const void*, size_t);
DSError TRK_ReadBuffer(MessageBuffer*, void*, size_t);

inline DSError TRKAppendBuffer1_ui8(MessageBuffer* buffer, const ui8 data) {
    if (buffer->fPosition >= kMessageBufferSize) {
        return kMessageBufferOverflow;
    }

    buffer->fData[buffer->fPosition++] = data;
    buffer->fLength++;
    return kNoError;
}

DSError TRKAppendBuffer1_ui16(MessageBuffer* buffer, const ui16 data);
DSError TRKAppendBuffer1_ui32(MessageBuffer* buffer, const ui32 data);
DSError TRKAppendBuffer1_ui64(MessageBuffer* buffer, const ui64 data);
DSError TRKAppendBuffer1_ui128(MessageBuffer* buffer, const ui128 data);

DSError TRKAppendBuffer_ui8(MessageBuffer* buffer, const ui8* data, int count);
DSError TRKAppendBuffer_ui16(MessageBuffer* buffer, const ui16* data, int count);
DSError TRKAppendBuffer_ui32(MessageBuffer* buffer, const ui32* data, int count);
DSError TRKAppendBuffer_ui64(MessageBuffer* buffer, const ui64* data, int count);
DSError TRKAppendBuffer_ui128(MessageBuffer* buffer, const ui128* data, int count);

DSError TRKReadBuffer1_ui8(MessageBuffer* buffer, ui8* data);
DSError TRKReadBuffer1_ui16(MessageBuffer* buffer, ui16* data);
DSError TRKReadBuffer1_ui32(MessageBuffer* buffer, ui32* data);
DSError TRKReadBuffer1_ui64(MessageBuffer* buffer, ui64* data);
DSError TRKReadBuffer1_ui128(MessageBuffer* buffer, ui128 data);

DSError TRKReadBuffer_ui8(MessageBuffer* buffer, ui8* data, int count);
DSError TRKReadBuffer_ui16(MessageBuffer* buffer, ui16* data, int count);
DSError TRKReadBuffer_ui32(MessageBuffer* buffer, ui32* data, int count);
DSError TRKReadBuffer_ui64(MessageBuffer* buffer, ui64* data, int count);
DSError TRKReadBuffer_ui128(MessageBuffer* buffer, ui128* data, int count);


#endif
/* end "PowerPC_EABI_Support/MetroTRK/msgbuf.h" */

#ifdef __cplusplus
extern "C"{
#endif

typedef unsigned long NubEventID;

typedef enum NubEventType {
    kNullEvent,
    kShutdownEvent,
    kRequestEvent,
    kBreakpointEvent,
    kExceptionEvent,
    kSupportEvent
} NubEventType;

typedef struct NubEvent {
    NubEventType fType;
    NubEventID fID;
    MessageBufferID fMessageBufferID;
} NubEvent;

DSError TRKInitializeEventQueue();
bool TRKGetNextEvent(NubEvent*);
DSError TRKPostEvent(NubEvent*);
void TRKConstructEvent(NubEvent*, int);
void TRKDestructEvent(NubEvent*);

#ifdef __cplusplus
}
#endif
#endif
/* end "PowerPC_EABI_Support/MetroTRK/nubevent.h" */
/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MetroTRK/targimpl.h" line 6 "PowerPC_EABI_Support/MetroTRK/msgbuf.h" */
/* end "PowerPC_EABI_Support/MetroTRK/msgbuf.h" */
/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MetroTRK/targimpl.h" line 7 "PowerPC_EABI_Support/MetroTRK/Processor/ppc/Board/dolphin/target.h" */
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

typedef enum ValidMemoryOptions {
    kValidMemoryReadable,
    kValidMemoryWriteable
} ValidMemoryOptions;

typedef enum MemoryAccessOptions {
    kUserMemory,
    kDebuggerMemory
} MemoryAccessOptions;

/*
enum
{
    kInvalidMemory = 0x0700,
    kInvalidRegister,
    kCWDSException,
    kUnsupportedError,
    kInvalidProcessId,
    kInvalidThreadId,
    kOsError
};
*/

typedef struct DSVersions {
    ui8 kernelMajor;
    ui8 kernelMinor;
    ui8 protocolMajor;
    ui8 protocolMinor;
} DSVersions;

typedef struct DSCPUType {
    ui8 cpuMajor;
    ui8 cpuMinor;
    ui8 bigEndian;
    ui8 defaultTypeSize;
    ui8 fpTypeSize;
    ui8 extended1TypeSize;
    ui8 extended2TypeSize;
} DSCPUType;

void TRKTargetSetStopped(bool);
void TRKTargetSetInputPendingPtr(void*);

DSError TRKTargetAccessMemory(void*,void*,size_t*,MemoryAccessOptions,bool);
DSError TRKTargetAccessDefault(ui32,ui32,MessageBuffer*,size_t*,bool);
DSError TRKTargetAccessFP(ui32,ui32,MessageBuffer*,size_t*,bool);
DSError TRKTargetAccessExtended1(ui32,ui32,MessageBuffer*,size_t*,bool);
DSError TRKTargetAccessExtended2(ui32,ui32,MessageBuffer*,size_t*,bool);

DSError TRKTargetVersions(DSVersions*);
DSError TRKTargetSupportMask(DSSupportMask*);
//DSError TRKTargetCPUType(DSCPUType*);
DSError TRKTargetCheckException();
DSError TRKInitializeTarget();
DSError TRKTargetContinue();

void TRKSwapAndGo();

DSError TRKTargetInterrupt(NubEvent*);

DSError TRKTargetAddStopInfo(MessageBuffer*);
void TRKTargetAddExceptionInfo(MessageBuffer*);

DSError TRKTargetSingleStep(ui8,bool);
DSError TRKTargetStepOutOfRange(ui32,ui32,bool);

ui32 TRKTargetGetPC();

DSError TRKTargetSupportRequest();
DSError TRKTargetFlushCache(ui8,void*,void*);

bool TRKTargetStopped();
void TRKTargetSetStopped(bool);
DSError TRKTargetStop();

void *TRKTargetTranslate(ui32*);

void TRK_InterruptHandler(ui16);

#if TRK_TRANSPORT_INT_DRIVEN
void TRKTargetSetInputPendingPtr(void*);
#endif


/*
** Define aliases for the functions which have both OS and non-OS
** variants.  Each alias references the variant which is appropriate
** for the current protocol level.
*/
#if DS_PROTOCOL < DS_PROTOCOL_RTOS

    #define XTargetAccessMemory       TRKTargetAccessMemory
    #define XTargetAccessDefault      TRKTargetAccessDefault
    #define XTargetAccessFP           TRKTargetAccessFP
    #define XTargetAccessExtended1    TRKTargetAccessExtended1
    #define XTargetAccessExtended2    TRKTargetAccessExtended2
    #define XTargetContinue           TRKTargetContinue
    #define XTargetSingleStep         TRKTargetSingleStep
    #define XTargetStepOutOfRange     TRKTargetStepOutOfRange
    #define XTargetGetPC              TRKTargetGetPC
    #define XTargetStopped            TRKTargetStopped
    #define XTargetStop               TRKTargetStop
    #define XTargetAddStopInfo        TRKTargetAddStopInfo
    #define XTargetAddExceptionInfo   TRKTargetAddExceptionInfo

#else /* #if DS_PROTOCOL < DS_PROTOCOL_RTOS */

    #define XTargetAccessMemory       OsTargetAccessMemory
    #define XTargetAccessDefault      OsTargetAccessDefault
    #define XTargetAccessFP           OsTargetAccessFP
    #define XTargetAccessExtended1    OsTargetAccessExtended1
    #define XTargetAccessExtended2    OsTargetAccessExtended2
    #define XTargetContinue           OsTargetContinue
    #define XTargetSingleStep         OsTargetSingleStep
    #define XTargetStepOutOfRange     OsTargetStepOutOfRange
    #define XTargetGetPC              OsTargetGetPC
    #define XTargetStopped            OsTargetStopped
    #define XTargetStop               OsTargetStop
    #define XTargetAddStopInfo        OsTargetAddStopInfo
    #define XTargetAddExceptionInfo   OsTargetAddExceptionInfo

#endif /* #if DS_PROTOCOL < DS_PROTOCOL_RTOS */

#endif
/* end "PowerPC_EABI_Support/MetroTRK/targimpl.h" */


bool gTRKBigEndian;

DSError TRK_InitializeNub(void)
{
    DSError result;
    DSError resultTemp;

    result = TRK_InitializeEndian();

    if (result == kNoError) {
        result = TRKInitializeEventQueue();
    }
    if (result == kNoError) {
        result = TRK_InitializeMessageBuffers();
    }
    InitializeProgramEndTrap();
    if (result == kNoError) {
        result = TRKInitializeSerialHandler();
    }
    if (result == kNoError) {
        result = TRKInitializeTarget();
    }
    if (result == kNoError) {

        resultTemp = TRK_InitializeIntDrivenUART(1, 0, &gTRKInputPendingPtr);
        TRKTargetSetInputPendingPtr(gTRKInputPendingPtr);
        if (resultTemp != kNoError) {
            result = resultTemp;
        }
    }
    return result;
}

DSError TRK_TerminateNub(void)
{
    TRKTerminateSerialHandler();
    return kNoError;
}

void TRK_NubWelcome(void)
{
    TRK_board_display("MetroTRK for Revolution v0.4");
    return;
}

bool TRK_InitializeEndian(void)
{
    ui8 bendian[4];
    bool result = false;
    gTRKBigEndian = true;

    bendian[0] = 0x12;
    bendian[1] = 0x34;
    bendian[2] = 0x56;
    bendian[3] = 0x78;

    if (*(ui32*)bendian == 0x12345678) {
        gTRKBigEndian = true;
    } else if (*(ui32*)bendian == 0x78563412) {
        gTRKBigEndian = false;
    } else {
        result = true;
    }
    return result;
}
