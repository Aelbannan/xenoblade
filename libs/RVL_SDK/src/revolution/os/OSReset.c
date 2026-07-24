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

/*
 * Retail .data pools (sized arrays, not bare OS_ERROR literals — those leave
 * size-0 relocs and soft-cap ~98%). Layout targets: file 0xC, hot 0x60,
 * menu+orphans 0x180, obsolete 0x7C. MWCC may 8-align after the 0xC file
 * object (unit data soft-cap); function match uses reloc *sizes*. Big callers
 * use strBase=&OSReset_file with +0xC/+0x38; thin wrappers / OSRestart use
 * distinct objects so MWCC emits two lis pairs.
 */
#pragma force_active on
char OSReset_file[0xC] = "OSReset.c\0\0";
char OSReset_hotResetPool[0x60] =
    "__OSHotReset(): Falied to reset system.\n\0\0\0\0"
    "__OSReturnToMenu(): Falied to boot system menu.\n\0\0\0";
char OSReset_menuPool[0x180] =
    "OSReturnToMenu(): Falied to boot system menu.\n\0\0"
    "OSReturnToDataManager(): Falied to boot system menu.\n\0\0\0"
    "Calendar/Calendar_index.html\0\0\0\0"
    "Display/Display_index.html\0\0"
    "Sound/Sound_index.html\0\0"
    "Parental_Control/Parental_Control_index.html\0\0\0\0"
    "Internet/Internet_index.html\0\0\0\0"
    "WiiConnect24/Wiiconnect24_index.html\0\0\0\0"
    "Update/Update_index.html\0\0\0\0"
    "OSReturnToSetting(): You can't specify %d.  \n\0\0";
char OSReset_obsoletePool[0x7C] =
    "OSResetSystem() is obsoleted. It doesn't work any longer.\n\0\0"
    "OSSetBootDol() is obsoleted. It doesn't work any longer.\n\0\0\0\0\0\0";
#pragma force_active off

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
    // Retail: (event-5)<=1 → {RETURN_TO_MENU,LAUNCH_APP}, else FATAL.
    // RESTART (4) keeps recalibration enabled (unlike ogws switch grouping).
    case OS_SD_EVENT_FATAL:
    case OS_SD_EVENT_RETURN_TO_MENU:
    case OS_SD_EVENT_LAUNCH_APP:
        keepEnable = FALSE;
        break;
    case OS_SD_EVENT_1:
    case OS_SD_EVENT_SHUTDOWN:
    case OS_SD_EVENT_3:
    case OS_SD_EVENT_RESTART:
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

static inline void HotResetPanic(char* strBase) {
    if (__OSInNandBoot || __OSInReboot) {
        __OSInitSTM();
    }
    __OSHotReset();

    OSPanic(strBase, 1034, strBase + 0xC);
}

static inline void HotResetPanicMenu(char* strBase) {
    HotResetPanic(strBase);

    OSPanic(strBase, 1010, strBase + 0x38);
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
    /* Separate objects → retail's two lis (file + hot), not strBase+0xC. */
    if (__OSInNandBoot || __OSInReboot) {
        __OSInitSTM();
    }
    __OSHotReset();
    OSPanic(OSReset_file, 1034, OSReset_hotResetPool);
}

void __OSReturnToMenu(u8 menuMode) {
    /* Three OSStateFlags: retail frame -0x90 at 0x58/0x38/0x18.
     * Soft-cap ~95%: early strBase lands in r31 (retail r30); disc/ticket in r30
     * (retail r31). Decl-order / union / CSE-hoist experiments did not flip it. */
    OSStateFlags stateFlags;
    OSStateFlags stateFlagsEsp;
    OSStateFlags stateFlagsAlloc;
    void* ticketView;
    u8 disc;
    char* strBase = OSReset_file;

    __OSStopPlayRecord();
    __OSUnRegisterStateEvent();
    __DVDPrepareReset();
    __OSReadStateFlags(&stateFlags);

    disc = stateFlags.discState;
    stateFlags.discState = GetDiscState(disc);
    disc = 3;
    stateFlags.BYTE_0x5 = disc;
    stateFlags.BYTE_0x7 = menuMode;
    __OSClearRTCFlags();
    __OSWriteStateFlags(&stateFlags);

    OSSetArenaLo((void*)0x81280000);
    OSSetArenaHi((void*)0x812F0000);

    if (ESP_InitLib() != 0) {
        __OSReadStateFlags(&stateFlagsEsp);
        stateFlagsEsp.discState = 2;
        stateFlagsEsp.BYTE_0x5 = disc;
        __OSClearRTCFlags();
        __OSWriteStateFlags(&stateFlagsEsp);
        __OSLaunchMenu();
        OSDisableScheduler();
        __VISetRGBModeImm();
        HotResetPanicMenu(strBase);
    }

    ticketView = OSAllocFromMEM1ArenaLo(0xE0, 0x20);
    if (ticketView == NULL) {
        __OSReadStateFlags(&stateFlagsAlloc);
        stateFlagsAlloc.discState = 2;
        stateFlagsAlloc.BYTE_0x5 = disc;
        __OSClearRTCFlags();
        __OSWriteStateFlags(&stateFlagsAlloc);
        __OSLaunchMenu();
        OSDisableScheduler();
        __VISetRGBModeImm();
        HotResetPanicMenu(strBase);
    }

    memset(ticketView, 0, 0xE0);
    if (ESP_DiGetTicketView(NULL, ticketView) == 0) {
        if (OSPlayTimeIsLimited()) {
            u32 unk;
            s32 playTime;

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
    HotResetPanic(strBase);
}

void OSReturnToMenu(void) {
    __OSReturnToMenu(0);

    OSPanic(OSReset_file, 895, OSReset_menuPool);
}

void __OSReturnToMenuForError(void) {
    OSStateFlags stateFlags;
    char* strBase = OSReset_file;

    __OSReadStateFlags(&stateFlags);
    stateFlags.discState = 2;
    stateFlags.BYTE_0x5 = 3;
    __OSClearRTCFlags();
    __OSWriteStateFlags(&stateFlags);
    __OSLaunchMenu();
    OSDisableScheduler();
    __VISetRGBModeImm();

    HotResetPanicMenu(strBase);
}

void __OSHotResetForError(void) {
    if (__OSInNandBoot || __OSInReboot) {
        __OSInitSTM();
    }
    __OSHotReset();
    OSPanic(OSReset_file, 1034, OSReset_hotResetPool);
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

    OSPanic(OSReset_file, 1185, OSReset_obsoletePool);
}

static void KillThreads(void) {
    OSThread* iter;
    OSThread* next;

    for (iter = OS_THREAD_QUEUE.head; iter != NULL; iter = next) {
        next = iter->nextActive;

        switch (iter->state) {
        case OS_THREAD_STATE_READY:
        case OS_THREAD_STATE_SLEEPING:
            OSCancelThread(iter);
            break;
        }
    }
}
