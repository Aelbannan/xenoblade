/* "libs/PowerPC_EABI_Support/src/MetroTRK/custconn/CircleBuffer.c" line 0 "PowerPC_EABI_Support/MetroTRK/custconn/CircleBuffer.h" */
#ifndef TRK_CIRCLE_BUFFER_H
#define TRK_CIRCLE_BUFFER_H

/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MetroTRK/custconn/CircleBuffer.h" line 3 "PowerPC_EABI_Support/MetroTRK/dstypes.h" */
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

typedef struct CircleBuffer {
    ui8* unk0;
    ui8* unk4;
    ui8* unk8;
    ui32 unkC;
    ui32 mBytesToRead;
    ui32 mBytesToWrite;
    unsigned int mSection;
    ui32 unk1C;
} CircleBuffer;


ui32 CBGetBytesAvailableForRead(CircleBuffer* cb);
ui32 CBGetBytesAvailableForWrite(CircleBuffer* cb);
void CircleBufferInitialize(CircleBuffer* cb, ui8* buf, ui32 size);
void CircleBufferTerminate(CircleBuffer* cb);
int CircleBufferWriteBytes(CircleBuffer* cb, ui8* buf, ui32 size);
int CircleBufferReadBytes(CircleBuffer* cb, ui8* buf, ui32 size);

#endif
/* end "PowerPC_EABI_Support/MetroTRK/custconn/CircleBuffer.h" */
/* "libs/PowerPC_EABI_Support/src/MetroTRK/custconn/CircleBuffer.c" line 1 "PowerPC_EABI_Support/MetroTRK/custconn/MWCriticalSection_gc.h" */
#ifndef MW_CRITICAL_SECTION_GC
#define MW_CRITICAL_SECTION_GC

//MWCriticalSection_gc
void MWInitializeCriticalSection(unsigned int* section);
void MWEnterCriticalSection(unsigned int* section);
void MWExitCriticalSection(unsigned int* section);
void MWTerminateCriticalSection(unsigned int* section);

#endif
/* end "PowerPC_EABI_Support/MetroTRK/custconn/MWCriticalSection_gc.h" */
/* "libs/PowerPC_EABI_Support/src/MetroTRK/custconn/CircleBuffer.c" line 2 "PowerPC_EABI_Support/MetroTRK/trk.h" */
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

ui32 CBGetBytesAvailableForRead(CircleBuffer* cb){
    return cb->mBytesToRead;
}

//unused
ui32 CBGetBytesAvailableForWrite(CircleBuffer* cb){
    return cb->mBytesToWrite;
}

void CircleBufferInitialize(CircleBuffer* cb, ui8* buf, ui32 size){
    cb->unk8 = buf;
    cb->unkC = size;
    cb->unk0 = buf;
    cb->unk4 = buf;
    cb->mBytesToRead = 0;
    cb->mBytesToWrite = size;
    MWInitializeCriticalSection(&(cb->mSection));
}

//unused
void CircleBufferTerminate(CircleBuffer* cb){
    MWTerminateCriticalSection(&(cb->mSection));
    memset(cb,0,sizeof(CircleBuffer) - 4);
}

int CircleBufferWriteBytes(CircleBuffer* cb, ui8* buf, ui32 size){
    ui32 r29;
    
    if(size > cb->mBytesToWrite) return -1;

    MWEnterCriticalSection(&(cb->mSection));

    r29 = cb->unkC - (cb->unk4 - cb->unk8);

    if(r29 >= size){
        memcpy(cb->unk4, buf, size);
        cb->unk4 += size;
    }else{
        memcpy(cb->unk4, buf, r29);
        memcpy(cb->unk8, buf + r29, size - r29);
        cb->unk4 = cb->unk8 + size - r29;
    }

    if(cb->unkC == cb->unk4 - cb->unk8){
        cb->unk4 = cb->unk8;
    }

    cb->mBytesToWrite -= size;
    cb->mBytesToRead += size;

    MWExitCriticalSection(&(cb->mSection));

    return 0;
}

int CircleBufferReadBytes(CircleBuffer* cb, ui8* buf, ui32 size){
    ui32 r29;

    if(size > cb->mBytesToRead) return -1;

    MWEnterCriticalSection(&(cb->mSection));

    r29 = cb->unkC - (cb->unk0 - cb->unk8);

    if(size < r29){
        memcpy(buf, cb->unk0, size);
        cb->unk0 += size;
    }else{
        memcpy(buf, cb->unk0, r29);
        memcpy(buf + r29, cb->unk8, size - r29);
        cb->unk0 = cb->unk8 + size - r29;
    }

    if(cb->unkC == cb->unk0 - cb->unk8){
        cb->unk0 = cb->unk8;
    }

    cb->mBytesToWrite += size;
    cb->mBytesToRead -= size;

    MWExitCriticalSection(&(cb->mSection));
    
    return 0;
}
