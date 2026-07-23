#include <revolution/DVD.h>
#include <revolution/ESP.h>
#include <revolution/OS.h>
#include <revolution/PAD.h>
#include <revolution/SC.h>
#include <revolution/VI.h>

#include <string.h>

static OSShutdownFunctionQueue ShutdownFunctionQueue;
static u32 bootThisDol;
volatile BOOL __OSIsReturnToIdle;

static void KillThreads(void);
void __OSHotResetForError(void);

extern BOOL __OSInNandBoot;
extern void __OSReboot(u32 resetCode, u32 bootDol);
extern void __OSRelaunchTitle(u32 resetCode);
extern BOOL OSPlayTimeIsLimited(void);
extern s32 __OSGetPlayTime(void* ticketView, u32* out1, s32* out2);
extern void __OSWriteExpiredFlagIfSet(void);

void OSRegisterShutdownFunction(OSShutdownFunctionInfo* info) {
    OSShutdownFunctionInfo* it;
    OSShutdownFunctionInfo* prev;
    OSShutdownFunctionInfo* tail;

    for (it = ShutdownFunctionQueue.head; it != NULL && it->prio <= info->prio;
         it = it->next) {
        ;
    }

    if (it == NULL) {

        tail = ShutdownFunctionQueue.tail;
        if (tail == NULL) {
            ShutdownFunctionQueue.head = info;
        } else {
            ShutdownFunctionQueue.tail->next = info;
        }

        info->prev = tail;
        info->next = NULL;

        it = ShutdownFunctionQueue.head;
        ShutdownFunctionQueue.tail = info;
    } else {
        info->next = it;

        prev = it->prev;
        it->prev = info;
        info->prev = prev;

        if (prev == NULL) {
            ShutdownFunctionQueue.head = info;
        } else {
            prev->next = info;
        }
    }
}

BOOL __OSCallShutdownFunctions(u32 pass, u32 event) {
    OSShutdownFunctionInfo* iter;
    BOOL failure;
    u32 prio;

    prio = 0;
    failure = FALSE;

    for (iter = ShutdownFunctionQueue.head; iter != NULL; iter = iter->next) {
        if (failure && prio != iter->prio) {
            break;
        }

        failure |= !iter->func(pass, event);
        prio = iter->prio;
    }

    failure |= !__OSSyncSram();

    return !failure;
}

void __OSShutdownDevices(u32 event) {
    BOOL padIntr;
    BOOL keepEnable;

    switch (event) {
    case OS_SD_EVENT_FATAL:
    case OS_SD_EVENT_RESTART:
    case OS_SD_EVENT_RETURN_TO_MENU:
    case OS_SD_EVENT_LAUNCH_APP:
        keepEnable = FALSE;
        break;
    case OS_SD_EVENT_1:
    case OS_SD_EVENT_SHUTDOWN:
    case OS_SD_EVENT_3:
    default:
        keepEnable = TRUE;
        break;
    }

    __OSStopAudioSystem();

    if (!keepEnable) {
        padIntr = __PADDisableRecalibration(TRUE);
    }

    while (!__OSCallShutdownFunctions(FALSE, event)) {
        ;
    }

    while (!__OSSyncSram()) {
        ;
    }

    OSDisableInterrupts();
    __OSCallShutdownFunctions(TRUE, event);
    LCDisable();

    if (!keepEnable) {
        __PADDisableRecalibration(padIntr);
    }

    KillThreads();
}

static u8 GetDiscState(u8 last) {
    u32 flags;

    if (__DVDGetCoverStatus() != DVD_COVER_CLOSED) {
        return 3;
    }

    if (last == 1 && __OSGetRTCFlags(&flags) && flags == 0) {
        return 1;
    }

    return 2;
}

void OSShutdownSystem(void) {
    SCIdleModeInfo idleMode;
    OSStateFlags stateFlags;
    OSIOSRev iosRev;

    memset(&idleMode, 0, sizeof(SCIdleModeInfo));
    SCInit();
    while (SCCheckStatus() == SC_STATUS_BUSY) {
        ;
    }
    SCGetIdleMode(&idleMode);

    __OSStopPlayRecord();
    __OSUnRegisterStateEvent();
    __DVDPrepareReset();
    __OSReadStateFlags(&stateFlags);

    stateFlags.discState = GetDiscState(stateFlags.discState);
    if (idleMode.wc24 == TRUE) {
        stateFlags.BYTE_0x5 = 5;
    } else {
        stateFlags.BYTE_0x5 = 1;
    }

    __OSClearRTCFlags();
    __OSWriteStateFlags(&stateFlags);
    __OSGetIOSRev(&iosRev);

    if (idleMode.wc24 == TRUE) {
        __OSIsReturnToIdle = TRUE;
        OSDisableScheduler();
        __OSShutdownDevices(OS_SD_EVENT_RETURN_TO_MENU);
        OSEnableScheduler();
        __OSLaunchMenu();
    } else {
        OSDisableScheduler();
        __OSShutdownDevices(OS_SD_EVENT_SHUTDOWN);
        __OSShutdownToSBY();
    }
}

static inline void HotResetPanic(void) {
    if (__OSInNandBoot || __OSInReboot) {
        __OSInitSTM();
    }
    __OSHotReset();

    // clang-format off
#line 1034
    OS_ERROR("__OSHotReset(): Falied to reset system.\n");
    // clang-format on
}

static inline void HotResetPanicMenu(void) {
    HotResetPanic();

    // clang-format off
#line 1010
    OS_ERROR("__OSReturnToMenu(): Falied to boot system menu.\n");
    // clang-format on
}

void OSRestart(u32 resetCode) {
    u8 type = OSGetAppType();

    __OSStopPlayRecord();
    __OSUnRegisterStateEvent();

    if (type == 0x81) {
        OSDisableScheduler();
        __OSShutdownDevices(OS_SD_EVENT_RESTART);
        OSEnableScheduler();
        __OSRelaunchTitle(resetCode);
    } else if (type == 0x80) {
        OSDisableScheduler();
        __OSShutdownDevices(OS_SD_EVENT_RESTART);
        OSEnableScheduler();
        __OSReboot(resetCode, bootThisDol);
    }

    OSDisableScheduler();
    __OSShutdownDevices(OS_SD_EVENT_1);
    HotResetPanic();
}

void __OSReturnToMenu(u8 menuMode) {
    OSStateFlags stateFlags;
    void* ticketView;
    u32 unk;
    s32 playTime;

    __OSStopPlayRecord();
    __OSUnRegisterStateEvent();
    __DVDPrepareReset();
    __OSReadStateFlags(&stateFlags);

    stateFlags.discState = GetDiscState(stateFlags.discState);
    stateFlags.BYTE_0x5 = 3;
    stateFlags.BYTE_0x7 = menuMode;
    __OSClearRTCFlags();
    __OSWriteStateFlags(&stateFlags);

    OSSetArenaLo((void*)0x81280000);
    OSSetArenaHi((void*)0x812F0000);

    if (ESP_InitLib() != 0) {
        __OSReadStateFlags(&stateFlags);
        stateFlags.discState = 2;
        stateFlags.BYTE_0x5 = 3;
        __OSClearRTCFlags();
        __OSWriteStateFlags(&stateFlags);
        __OSLaunchMenu();
        OSDisableScheduler();
        __VISetRGBModeImm();
        HotResetPanicMenu();
    }

    ticketView = OSAllocFromMEM1ArenaLo(0xE0, 0x20);
    if (ticketView == NULL) {
        __OSReadStateFlags(&stateFlags);
        stateFlags.discState = 2;
        stateFlags.BYTE_0x5 = 3;
        __OSClearRTCFlags();
        __OSWriteStateFlags(&stateFlags);
        __OSLaunchMenu();
        OSDisableScheduler();
        __VISetRGBModeImm();
        HotResetPanicMenu();
    }

    memset(ticketView, 0, 0xE0);
    if (ESP_DiGetTicketView(NULL, ticketView) == 0) {
        if (OSPlayTimeIsLimited()) {
            unk = 0;
            playTime = -1;
            __OSGetPlayTime(ticketView, &unk, &playTime);
            if (playTime == 0) {
                __OSWriteExpiredFlagIfSet();
            }
        }
    }

    OSDisableScheduler();
    __OSShutdownDevices(OS_SD_EVENT_RETURN_TO_MENU);
    OSEnableScheduler();
    __OSLaunchMenu();
    OSDisableScheduler();
    __VISetRGBModeImm();
    HotResetPanic();
}

void OSReturnToMenu(void) {
    __OSReturnToMenu(0);

    // clang-format off
#line 895
    OS_ERROR("OSReturnToMenu(): Falied to boot system menu.\n");
    // clang-format on
}

void __OSReturnToMenuForError(void) {
    OSStateFlags stateFlags;

    __OSReadStateFlags(&stateFlags);
    stateFlags.discState = 2;
    stateFlags.BYTE_0x5 = 3;
    __OSClearRTCFlags();
    __OSWriteStateFlags(&stateFlags);
    __OSLaunchMenu();
    OSDisableScheduler();
    __VISetRGBModeImm();

    if (__OSInNandBoot || __OSInReboot) {
        __OSInitSTM();
    }
    __OSHotReset();

    // clang-format off
#line 1034
    OS_ERROR("__OSHotReset(): Falied to reset system.\n");
#line 1010
    OS_ERROR("__OSReturnToMenu(): Falied to boot system menu.\n");
    // clang-format on
}

void __OSHotResetForError(void) {
    if (__OSInNandBoot || __OSInReboot) {
        __OSInitSTM();
    }

    __OSHotReset();

    // clang-format off
#line 1034
    OS_ERROR("__OSHotReset(): Falied to reset system.\n");
    // clang-format on
}

u32 OSGetResetCode(void) {
    if (__OSRebootParams.WORD_0x0 != 0) {
        return __OSRebootParams.WORD_0x4 | 0x80000000;
    }

    return PI_HW_REGS[PI_RESET] >> 3;
}

void OSResetSystem(BOOL reset, u32 resetCode, BOOL forceMenu) {
#pragma unused(reset)
#pragma unused(resetCode)
#pragma unused(forceMenu)

    // clang-format off
#line 1185
    OS_ERROR("OSResetSystem() is obsoleted. It doesn't work any longer.\n");
    // clang-format on
}

/* Orphan literals retained in retail .data (functions removed from this TU). */
#pragma force_active on
static const char OSReset_orphan_data[] =
    "OSReturnToDataManager(): Falied to boot system menu.\n"
    "Calendar/Calendar_index.html\0\0\0"
    "Display/Display_index.html\0\0"
    "Sound/Sound_index.html\0\0"
    "Parental_Control/Parental_Control_index.html\0\0\0"
    "Internet/Internet_index.html\0\0\0"
    "WiiConnect24/Wiiconnect24_index.html\0\0\0"
    "Update/Update_index.html\0\0\0"
    "OSReturnToSetting(): You can't specify %d.  \n\0\0\0"
    "OSSetBootDol() is obsoleted. It doesn't work any longer.\n";
#pragma force_active off

static void KillThreads(void) {
    OSThread* iter;
    OSThread* next;

    for (iter = OS_THREAD_QUEUE.head; iter != NULL; iter = next) {
        next = iter->nextActive;

        switch (iter->state) {
        case OS_THREAD_STATE_SLEEPING:
        case OS_THREAD_STATE_READY:
            OSCancelThread(iter);
            break;
        }
    }
}
