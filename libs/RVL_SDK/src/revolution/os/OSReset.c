#include <revolution/DVD.h>
#include <revolution/ESP.h>
#include <revolution/OS.h>
#include <revolution/PAD.h>
#include <revolution/SC.h>
#include <revolution/VI.h>

#include <string.h>

OSShutdownFunctionQueue ShutdownFunctionQueue;
volatile BOOL __OSIsReturnToIdle;
u32 bootThisDol;

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

static inline void HotResetPanic(void) {
    if (__OSInNandBoot || __OSInReboot) {
        __OSInitSTM();
    }
    __OSHotReset();

    OSPanic("OSReset.c", 1034, "__OSHotReset(): Falied to reset system.\n");
}

static inline void HotResetPanicMenu(void) {
    HotResetPanic();

    OSPanic("OSReset.c", 1010, "__OSReturnToMenu(): Falied to boot system menu.\n");
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
    if (__OSInNandBoot || __OSInReboot) {
        __OSInitSTM();
    }
    __OSHotReset();
    OSPanic("OSReset.c", 1034, "__OSHotReset(): Falied to reset system.\n");
}

void __OSReturnToMenu(u8 menuMode) {
    /* Three OSStateFlags: retail frame -0x90 at 0x58/0x38/0x18.
     * Soft-cap ~95%: early strBase lands in r31 (retail r30); disc/ticket in r30
     * (retail r31). Decl-order / union / CSE-hoist experiments did not flip it. */
    OSStateFlags stateFlags;
    OSStateFlags stateFlagsEsp;
    OSStateFlags stateFlagsAlloc;
    u8 disc;
    void* ticketView;

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
        HotResetPanicMenu();
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
        HotResetPanicMenu();
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
    HotResetPanic();
}

void OSReturnToMenu(void) {
    __OSReturnToMenu(0);

    OSPanic("OSReset.c", 895, "OSReturnToMenu(): Falied to boot system menu.\n");
}

/* Public SDK reset paths; the game build dead-strips these functions but
 * MWCC keeps their literals in this TU's string pool. */
// name-guess: name from the panic string; no retail symbol/body survives dead-strip, line 1010 guessed
void OSReturnToDataManager(void) {
    OSPanic("OSReset.c", 1010,
            "OSReturnToDataManager(): Falied to boot system menu.\n");
}

// name-guess: invented owner for the 7 page-name strings (Calendar..Update); no caller/symbol, switch shape and page order guessed
void __OSReportMenuPage(u32 page) {
    switch (page) {
    case 0:
        OSPanic("OSReset.c", 0, "Calendar/Calendar_index.html");
        break;
    case 1:
        OSPanic("OSReset.c", 0, "Display/Display_index.html");
        break;
    case 2:
        OSPanic("OSReset.c", 0, "Sound/Sound_index.html");
        break;
    case 3:
        OSPanic("OSReset.c", 0,
                "Parental_Control/Parental_Control_index.html");
        break;
    case 4:
        OSPanic("OSReset.c", 0, "Internet/Internet_index.html");
        break;
    case 5:
        OSPanic("OSReset.c", 0, "WiiConnect24/Wiiconnect24_index.html");
        break;
    case 6:
        OSPanic("OSReset.c", 0, "Update/Update_index.html");
        break;
    }
}

// name-guess: name from the panic string; u32 setting/line 0 inferred from "%d", no retail body survives dead-strip
void OSReturnToSetting(u32 setting) {
    OSPanic("OSReset.c", 0,
            "OSReturnToSetting(): You can't specify %d.  \n", setting);
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

    HotResetPanicMenu();
}

void __OSHotResetForError(void) {
    if (__OSInNandBoot || __OSInReboot) {
        __OSInitSTM();
    }
    __OSHotReset();
    OSPanic("OSReset.c", 1034, "__OSHotReset(): Falied to reset system.\n");
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

    OSPanic("OSReset.c", 1185,
            "OSResetSystem() is obsoleted. It doesn't work any longer.\n");
}

// name-guess: name from the obsolete-API string; void signature/line 1185 guessed, no retail body survives dead-strip
void OSSetBootDol(void) {
    OSPanic("OSReset.c", 1185,
            "OSSetBootDol() is obsoleted. It doesn't work any longer.\n");
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
