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

void REXInit();

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
    
    {
        s32 status;
        if (OSGetCurrentThread() == NULL) {
            status = -1;
        } else {
            status = 0;
        }
        if (status < 0) {
            return status;
        }
    }
    
    fd = IOS_Open("/dev/net/kd/request", IPC_OPEN_NONE);
    if (fd < 0) {
        if (fd == -6) {
            result = -29;
        } else {
            result = -42;
        }
    } else {
        result = 0;
    }
    if (result >= 0) {
        if (IOS_Ioctl(fd, 1, 0, 0, susResult, 0x20) < 0) {
            result = -42;
        } else {
            result = 0;
        }
        if (result >= 0) {
            result = susResult[0];
        }
        {
            s32 closeErr;
            if (IOS_Close(fd) < 0) {
                closeErr = -42;
            } else {
                closeErr = 0;
            }
            if (closeErr < 0) {
                result = closeErr;
            }
        }
    }
    return result;
}

#pragma pop

#pragma push
#pragma auto_inline off

//unused
DECL_WEAK s32 NWC24ResumeScheduler(){
    int iVar1;
    int iVar2;
    s32 local_18;
    static s32 susResult[8] ALIGN(32);
    
    iVar1 = CheckCallingStatus(__FUNCTION__);
    if ((-1 < iVar1)){
        iVar1 = NWC24iOpenResourceManager_(__FUNCTION__,"/dev/net/kd/request", &local_18,0);
        if(-1 < iVar1) {
        iVar1 = NWC24iIoctlResourceManager_(__FUNCTION__,local_18, 3, 0, 0, susResult, sizeof(susResult));
        if (-1 < iVar1) {
            iVar1 = susResult[0];
        }
        iVar2 = NWC24iCloseResourceManager_(__FUNCTION__,local_18);
        if (iVar2 < 0) {
            iVar1 = iVar2;
        }
        }
    }
    return iVar1;
}

NWC24Err NWC24iRequestShutdown(u32 param_1, NWC24Err* resultOut) DECOMP_DONT_INLINE {
    static s32 shtBuffer[8] ALIGN(32);
    static s32 shtResult[8] ALIGN(32);
    
    shtBuffer[0] = param_1;
    
    if (IOS_IoctlAsync(nwc24ShtFd, 0x28, shtBuffer, 0x20, shtResult, 0x20, CallbackAsyncIpc, resultOut) < 0) {
        return -42;
    }
    
    NWC24iIsRequestPending = TRUE;
    return 0;
}

static BOOL NWC24iIsAsyncRequestPending_();

static BOOL NWC24Shutdown_(BOOL final, u32 event) DECOMP_DONT_INLINE {
    static BOOL shuttingdown;
    static NWC24Err result;
    
    if (final) {
        return TRUE;
    }
    if (shuttingdown) {
        if (NWC24iIsRequestPending) {
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
        if (NWC24iRequestShutdown(event, &result) >= 0) {
            shuttingdown = TRUE;
        }
    }
    return FALSE;
}

s32 NWC24iSetRtcCounter_(u32 rtc, u32 param_2) DECOMP_DONT_INLINE {
    s32 result;
    s32 fd;
    
    {
        s32 status;
        if (OSGetCurrentThread() == NULL) {
            status = -1;
        } else {
            status = 0;
        }
        if (status < 0) {
            return status;
        }
    }
    
    fd = IOS_Open("/dev/net/kd/time", IPC_OPEN_NONE);
    if (fd < 0) {
        if (fd == -6) {
            result = -29;
        } else {
            result = -42;
        }
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
            s32 closeErr;
            if (IOS_Close(fd) < 0) {
                closeErr = -42;
            } else {
                closeErr = 0;
            }
            if (result >= 0) {
                result = closeErr;
            }
        }
    }
    return result;
}

#pragma pop

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

//unused
void __OSSyncTimeWithNetRM(){
    NWC24iSynchronizeRtcCounter(FALSE);
}

void REXInit(){
}
