#include <revolution/NWC24.h>
#include <revolution/OS.h>
#include <revolution/IPC.h>
#include <revolution/SC.h>


static OSShutdownFunctionInfo ShutdownFuncInfo;

static u32 nwc24TimeCommonResult[8] ALIGN(32);
static u32 nwc24TimeCommonBuffer[8] ALIGN(32);

static s32 nwc24ShtRetryRest;
static s32 nwc24ShtFd = -1;
static BOOL NWC24iIsRequestPending;

static BOOL NWC24Shutdown_(BOOL final, u32 event);
static s32 CallbackAsyncIpc(s32 result, void* arg);
NWC24Err NWC24iRequestShutdown(u32 param_1, NWC24Err* resultOut);
static s32 NWC24iOpenResourceManager_(const char* funcName, const char* path, s32* resultPtr, IPCOpenMode mode);
static s32 NWC24iIoctlResourceManager_(const char* funcName, s32 fd, s32 type, void* in, s32 inSize, void* out, s32 outSize);
static s32 NWC24iIoctlResourceManagerAsync_(const char* funcName, s32 fd, s32 type, void* in, s32 inSize, void* out, s32 outSize, void* callbackArg);
static s32 NWC24iCloseResourceManager_(const char* funcName, s32 fd);
static s32 NWC24iCloseResourceManagerAsync_(const char* funcName, s32 fd, void* callbackArg);
static s32 CheckCallingStatus(const char* funcName);

#pragma push
#pragma auto_inline off

NWC24Err NWC24iPrepareShutdown() DECOMP_DONT_INLINE {
    s32 result;
    
    result = 0;
    ShutdownFuncInfo.func = NWC24Shutdown_;
    ShutdownFuncInfo.prio = 0x6e;
    OSRegisterShutdownFunction(&ShutdownFuncInfo);

    if (nwc24ShtFd < 0) {
        result = NWC24iOpenResourceManager_(__FUNCTION__,"/dev/net/kd/request",&nwc24ShtFd,1);
    }
    nwc24ShtRetryRest = 5;

    if (result == 0) {
        result = 1;
    }
    return result;
}

s32 NWC24iSetRtcCounter_(u32 rtc, u32 param_2) DECOMP_DONT_INLINE;

NWC24Err NWC24iSynchronizeRtcCounter(BOOL val) DECOMP_DONT_INLINE {
    s32 result;
    s32 rtc;
    
    while (TRUE) {
        u32 status = SCCheckStatus();
        if (status == 2) {
            result = -1;
            goto check_result;
        }
        if (status == 0) {
            break;
        }
    }
    
    {
        u32 bias = SCGetCounterBias();
        s64 time = OSGetTime();
        rtc = OS_TICKS_TO_SEC(time) - bias;
    }
    result = 0;
    
check_result:
    if (result != 0) {
        return result;
    }
    return NWC24iSetRtcCounter_(rtc, val != FALSE);
}

s32 NWC24SuspendScheduler() DECOMP_DONT_INLINE {
    s32 result;
    s32 fd;
    static s32 susResult[8] ALIGN(32);

    result = CheckCallingStatus(__FUNCTION__);
    if (result < 0) {
        return result;
    }

    fd = IOS_Open("/dev/net/kd/request", IPC_OPEN_NONE);
    if (fd < 0) {
        result = (fd == -6) ? -29 : -42;
    } else {
        result = 0;
    }

    if (result >= 0) {
        if (IOS_Ioctl(fd, 1, NULL, 0, susResult, sizeof(susResult)) < 0) {
            result = -42;
        } else {
            result = 0;
        }
        if (result >= 0) {
            result = susResult[0];
        }

        {
            s32 closeResult;
            if (IOS_Close(fd) < 0) {
                closeResult = -42;
            } else {
                closeResult = 0;
            }
            if (closeResult < 0) {
                result = closeResult;
            }
        }
    }

    return result;
}

#pragma pop

#pragma dont_inline on
NWC24Err NWC24iRequestShutdown(u32 param_1, NWC24Err* resultOut) {
    static s32 shtBuffer[8] ALIGN(32);
    static s32 shtResult[8] ALIGN(32);
    
    shtBuffer[0] = param_1;
    
    if (IOS_IoctlAsync(nwc24ShtFd, 0x28, shtBuffer, 0x20, shtResult, 0x20, CallbackAsyncIpc, resultOut) < 0) {
        return -42;
    }
    
    NWC24iIsRequestPending = TRUE;
    return 0;
}
#pragma dont_inline off

static BOOL NWC24iIsAsyncRequestPending_();

static BOOL NWC24Shutdown_(BOOL final, u32 event){
    static BOOL shuttingdown = FALSE;
    static NWC24Err result = NWC24_OK;

    int iVar1;
    
    if (final != FALSE) {
        return TRUE;
    }
    if (shuttingdown) {
        iVar1 = NWC24iIsAsyncRequestPending_();

        if (iVar1 != 0) {
            return FALSE;
        }
        if (result >= 0) {
            return TRUE;
        }
        if (nwc24ShtRetryRest > 0) {
            shuttingdown = FALSE;
            nwc24ShtRetryRest--;
        } else {
            OSReport("NWC24Shutdown_: Give up!\n");
            return TRUE;
        }
    } else {
        iVar1 = NWC24iRequestShutdown(event, &result);
        if (iVar1 >= 0) {
            shuttingdown = TRUE;
        }
    }

    return FALSE;
}

//unused
s32 NWC24iSetRtcCounter_(u32 rtc, u32 param_2) DECOMP_DONT_INLINE {
    s32 result;
    s32 fd;

    result = CheckCallingStatus(__FUNCTION__);
    if (result < 0) {
        return result;
    }

    fd = IOS_Open("/dev/net/kd/time", IPC_OPEN_NONE);
    if (fd < 0) {
        result = (fd == -6) ? -29 : -42;
    } else {
        result = 0;
    }

    if (result >= 0) {
        nwc24TimeCommonBuffer[0] = rtc;
        nwc24TimeCommonBuffer[1] = param_2;

        if (IOS_Ioctl(fd, 0x17, nwc24TimeCommonBuffer, 0x20, nwc24TimeCommonResult, 0x20) < 0) {
            result = -42;
        } else {
            result = 0;
        }
        if (result >= 0) {
            result = nwc24TimeCommonResult[0];
        }

        {
            s32 closeResult;
            if (IOS_Close(fd) < 0) {
                closeResult = -42;
            } else {
                closeResult = 0;
            }
            if (result >= 0) {
                result = closeResult;
            }
        }
    }

    return result;
}

static s32 NWC24iOpenResourceManager_(const char* funcName, const char* path, s32* resultPtr, IPCOpenMode mode){
    if(resultPtr == NULL){
        return -3;
    }
    
    *resultPtr = IOS_Open(path, mode);
    
    if (*resultPtr < 0) {
        if (*resultPtr == -6) return -29;
        else return -42;
    }else {
        return 0;
    }
}

static s32 NWC24iCloseResourceManager_(const char* funcName, s32 fd){
    if (IOS_Close(fd) < 0) return -42;
    else return 0;
}

static s32 CallbackAsyncIpc(s32 result, void* arg);

//unused
static s32 NWC24iCloseResourceManagerAsync_(const char* funcName, s32 fd, void* callbackArg){
    if (IOS_CloseAsync(fd, CallbackAsyncIpc, callbackArg) < 0) return -42;
    else {
        NWC24iIsRequestPending = TRUE;
        return 0;
    }
}

static s32 NWC24iIoctlResourceManager_(const char* funcName, s32 fd, s32 type, void* in, s32 inSize, void* out, s32 outSize){
    if (IOS_Ioctl(fd, type, in, inSize, out, outSize) < 0) return -42;
    else return 0;
}

static s32 NWC24iIoctlResourceManagerAsync_(const char* funcName, s32 fd, s32 type, void* in, s32 inSize, void* out, s32 outSize, void* callbackArg){
    if (IOS_IoctlAsync(fd, type, in, inSize, out, outSize, CallbackAsyncIpc, callbackArg) < 0) return -42;
    else {
        NWC24iIsRequestPending = TRUE;
        return 0;
    }
}

static BOOL NWC24iIsAsyncRequestPending_(){
    return NWC24iIsRequestPending;
}

static s32 CallbackAsyncIpc(s32 result, void* arg){
    if (arg != NULL) {
        *(s32*)arg = result;
    }
    NWC24iIsRequestPending = FALSE;
    return 0;
}

static s32 CheckCallingStatus(const char* funcName){
    if (OSGetCurrentThread() == NULL) return -1;
    else return 0;
}

NWC24Err NWC24iPrepareShutdown(void) DECOMP_DONT_INLINE;
s32 NWC24SuspendScheduler(void) DECOMP_DONT_INLINE;
NWC24Err NWC24iSynchronizeRtcCounter(BOOL val) DECOMP_DONT_INLINE;

void __OSInitNet(void) {
    s32 error;
    OSIOSRev rev;

    __OSGetIOSRev(&rev);

    if (rev.idLo <= 4 || rev.idLo == 9) {
        return;
    }

    error = NWC24iPrepareShutdown();
    if (error != 0) {
        if (error < 0) {
            OSReport("Failed to register network shutdown function. %d\n", error);
        }
        error = NWC24SuspendScheduler();
        if (error < 0) {
            OSReport("Failed to suspend the WiiConnect24 scheduler. %d\n", error);
        }
    }

    if (!__OSInIPL) {
        error = NWC24iSynchronizeRtcCounter(FALSE);
        if (error != 0) {
            OSReport("Failed to synchronize time with network resource managers. %d\n", error);
        }
    }
}
