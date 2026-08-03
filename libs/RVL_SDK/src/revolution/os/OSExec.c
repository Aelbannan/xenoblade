#include <revolution/ESP.h>
#include <revolution/IPC.h>
#include <revolution/OS.h>
#include <revolution/os/OSArena.h>

#include <stdio.h>
#include <string.h>

#define MENU_TITLE_ID 0x0000000100000002
#define TICKET_VIEW_SIZE 0xD8

BOOL __OSInReboot;

#pragma dont_inline on
void Run(void (*func)(void)) {
    ICFlashInvalidate();
    __sync();
    __isync();
    func();
}
#pragma dont_inline off

void __OSGetExecParams(OSExecParams* out) {
    if ((void*)OS_DOL_EXEC_PARAMS >= (void*)0x80000000) {
        memcpy(out, OS_DOL_EXEC_PARAMS, sizeof(OSExecParams));
    } else {
        out->WORD_0x0 = 0;
    }
}

//unused
void __OSSetExecParams(){
}

/**
 * Hex-encodes a UTF-16LE string (up to the first zero byte) into dst.
 * Returns 1 on success, 0 when src is NULL or contains an invalid digit.
 */
/**
 * Hex-encodes a UTF-16LE string (up to the first zero byte) into dst.
 * Returns TRUE on success, FALSE when srcName is NULL or contains an
 * invalid digit.
 */
BOOL Utf16ToArg(char* dstArg, u16* srcName) {
    char* srcPtr;
    char* dstPtr;
    u8 i;
    u8 mask;
    u8 shift;

    if (srcName != 0) {
        srcPtr = (char*)srcName;
        dstPtr = dstArg;

        while (*srcPtr || *(srcPtr + 1)) {
            for (i = 0; i < 4; i++) {
                mask = (u8)((i & 0x1) ? 0xF : 0xF0);
                shift = (u8)((i & 0x1) ? 0 : 4);

                if (0 <= ((*srcPtr & mask) >> shift) &&
                    ((*srcPtr & mask) >> shift) < 0xA) {
                    *dstPtr = (char)(((*srcPtr & mask) >> shift) + 0x30);
                } else if (0xA <= ((*srcPtr & mask) >> shift) &&
                           ((*srcPtr & mask) >> shift) < 0x10) {
                    *dstPtr = (char)(((*srcPtr & mask) >> shift) + 0x57);
                } else {
                    return FALSE;
                }

                dstPtr++;

                if (i & 1) {
                    srcPtr++;
                }
            }
        }

        *dstPtr++ = 0;
        return TRUE;
    }

    return FALSE;
}

void __OSLaunchMenu(void) {
    s32 result;
    u32 count = 1;
    void* pviews;

    OSSetArenaLo((void*)0x81280000);
    OSSetArenaHi((void*)0x812f0000);

    result = ESP_InitLib();
    if (result != 0) {
        return;
    }

    result = ESP_GetTicketViews(MENU_TITLE_ID, NULL, &count);
    if (count != 1 || result != 0) {
        return;
    }

    pviews = OSAllocFromMEM1ArenaLo((count * TICKET_VIEW_SIZE + 0x1F) & ~0x1Fu,
                                    0x20);
    result = ESP_GetTicketViews(MENU_TITLE_ID, pviews, &count);
    if (result != 0) {
        return;
    }

    result = ESP_LaunchTitle(MENU_TITLE_ID, pviews);
    if (result != 0) {
        return;
    }

    while (1)
        ;
}

/**
 * Packs argc/argv into the 0x2000-byte boot-args area at dst:
 * strings are copied back-to-front from the top of the area, and the
 * argv pointer table is written just below them.  *(dst+8) receives
 * the offset of the pointer-table count word.
 */
int PackArgs(void* addr, s32 argc, char* argv[]) {
    s32 numArgs;
    char* bootInfo2;
    char* ptr;
    char** list;
    u32 i;

    bootInfo2 = (char*)addr;
    memset(bootInfo2, 0, 0x2000);

    if (argc == 0) {
        *(u32*)&bootInfo2[8] = 0;
    } else {
        numArgs = argc;
        ptr = bootInfo2 + 0x2000;
        while (--argc >= 0) {
            ptr -= (strlen(argv[argc]) + 1);
            strcpy(ptr, argv[argc]);
            argv[argc] = (char*)(ptr - bootInfo2);
        }

        ptr = bootInfo2 + ((ptr - bootInfo2) & ~3);
        ptr -= 4 * (numArgs + 1);
        list = (char**)ptr;

        for (i = 0; i < numArgs + 1; i++) {
            list[i] = argv[i];
        }

        ptr -= 4;
        *(s32*)ptr = numArgs;
        *(u32*)&bootInfo2[8] = (u32)(ptr - bootInfo2);
    }

    return TRUE;
}


/**
 * PackInstallerArgs: like PackArgs, but the odd/even index selects
 * UTF-16 (wcslen + Utf16ToArg) vs UTF-8 (strlen + strcpy) packing.
 */
int PackInstallerArgs(void* addr, s32 argc, char* argv[]) {
    s32 numArgs;
    char* bootInfo2;
    char* ptr;
    char** list;
    u32 i;

    bootInfo2 = (char*)addr;
    memset(bootInfo2, 0, 0x2000);

    if (argc == 0) {
        *(u32*)&bootInfo2[8] = 0;
    } else {
        numArgs = argc;
        ptr = bootInfo2 + 0x2000;

        while (--argc >= 0) {
            if (argc < 2 || (argc % 2)) {
                char* val = argv[argc];
                ptr -= (strlen(argv[argc]) + 1);
                strcpy(ptr, val);
                argv[argc] = (char*)(ptr - bootInfo2);
            } else {
                u16* val = (u16*)argv[argc];
                ptr -= (wcslen(val) * 4 + 1);
                Utf16ToArg(ptr, val);
                argv[argc] = (char*)(ptr - bootInfo2);
            }
        }

        ptr = bootInfo2 + ((ptr - bootInfo2) & ~3);
        ptr -= 4 * (numArgs + 1);

        list = (char**)ptr;

        for (i = 0; i < numArgs + 1; i++) {
            list[i] = argv[i];
        }

        ptr -= 4;
        *(s32*)ptr = numArgs;

        *(u32*)&bootInfo2[8] = (u32)(ptr - bootInfo2);
    }

    return TRUE;
}



/* Externs for the firmware-launch / disc-boot path (dvd_broadway.h / OS.h). */
typedef void (*DVDLowCallback)(u32 intType);
extern BOOL DVDLowInit(void);
extern BOOL DVDLowReadDiskID(DVDDiskID *out, DVDLowCallback callback);
extern BOOL DVDLowClosePartition(DVDLowCallback callback);
extern BOOL DVDLowUnencryptedRead(void *dst, u32 size, u32 offset,
                                  DVDLowCallback callback);
extern BOOL DVDLowFinalize(void);
extern BOOL DVDLowOpenPartitionWithTmdAndTicketView(const u32 offset,
                                                    const ESTicketView *ticket,
                                                    const u32 ticketViewSize,
                                                    const ESTitleMeta *tmd,
                                                    const u32 tmdSize,
                                                    const u8 *certs,
                                                    DVDLowCallback callback);
extern BOOL DVDLowOpenPartition(const u32 offset, const ESTicket *ticket,
                                const u32 certsSize, const u8 *certs,
                                ESTitleMeta *tmd, DVDLowCallback callback);
extern u32 DVDLowIntType;
extern void callback(u32 type);
extern DVDDiskID id;

extern void __OSGetPlayTime(ESTicketView *ticket, u32 *pLimit, u32 *pUsed);
extern void __OSWriteExpiredFlag(void);
extern BOOL OSPlayTimeIsLimited(void);
extern void __OSReturnToMenuForError(void);
extern void __OSHotResetForError(void);
extern void __OSInitMemoryProtection(void);
extern void __OSInitIPCBuffer(void);
extern void IPCReInit(void);
extern s32 IPCCltReInit(void);

/* Static error/status strings (retail pool). */
static const char sFailStr[] = "\nOSExec(): Failed to exec %d in %d\n";
static const char sNoGameStr[] =
    "\nOSExec(): The specified game doesn't exist in the disc\n";

void __OSLaunchNextFirmware(void) {
    u8 *bootInfo;
    u8 *gameTmd;
    ESTitleMeta *discTmd;
    u8 *ticketView;
    s32 err;
    u32 count;
    u32 tmdSize;
    u32 limit;
    u32 used;
    u32 *p;
    u8 *p2;
    u32 i;
    u64 titleId;
    ESTicketView *views;
    u32 codeStart;
    u32 codeSize;

    count = 1;
    tmdSize = 0;
    err = -1;

    bootInfo = OSAllocFromMEM1ArenaLo(0x20, 0x20);
    gameTmd = OSAllocFromMEM1ArenaLo(0x800, 0x20);
    discTmd = OSAllocFromMEM1ArenaLo(0x4A00, 0x40);
    ticketView = OSAllocFromMEM1ArenaLo(0xE0, 0x20);

    if ((u32)__OSNextPartitionType == *(u32 *)0x80003194 &&
        *(u32 *)0x80003198 != 0) {
        err = ESP_InitLib();
        if (err == 0) {
            err = ESP_DiGetTicketView(NULL, (ESTicketView *)ticketView);
        }
        if (err == 0) {
            err = ESP_DiGetTmd(NULL, &tmdSize);
        }
        if (err == 0) {
            err = ESP_DiGetTmd(discTmd, &tmdSize);
        }
        ESP_CloseLib();

        if (OSPlayTimeIsLimited()) {
            limit = 0;
            used = (u32)-1;
            __OSGetPlayTime((ESTicketView *)ticketView, &limit, &used);
            if (used == 0) {
                __OSWriteExpiredFlag();
                __OSReturnToMenuForError();
            }
        }
    }

    if (err == 0) {
        p = (u32 *)gameTmd;
        *(u32 *)(gameTmd + 4) = *(u32 *)0x80003194;
        *(u32 *)(gameTmd + 0) = *(u32 *)0x80003198;
    } else {
        DVDLowIntType = 0;
        DVDLowClosePartition(callback);
        while (DVDLowIntType == 0) {
            ;
        }
        if (DVDLowIntType != 1) {
            OSReport(sFailStr, DVDLowIntType, 0x398);
            __OSReturnToMenuForError();
        }

        DVDLowIntType = 0;
        DVDLowUnencryptedRead(bootInfo, 0x20, 0x10000, callback);
        while (DVDLowIntType == 0) {
            ;
        }
        if (DVDLowIntType != 1) {
            OSReport(sFailStr, DVDLowIntType, 0x3A2);
            __OSReturnToMenuForError();
        }

        DVDLowIntType = 0;
        DVDLowUnencryptedRead(gameTmd, 0x800, *(u32 *)(bootInfo + 4),
                              callback);
        while (DVDLowIntType == 0) {
            ;
        }
        if (DVDLowIntType != 1) {
            OSReport(sFailStr, DVDLowIntType, 0x3AE);
            __OSReturnToMenuForError();
        }

        p = NULL;
        p2 = gameTmd;
        for (i = 0; i < *(u32 *)bootInfo; i++, p2 += 8) {
            if (*(u32 *)(p2 + 4) == (u32)__OSNextPartitionType) {
                p = (u32 *)p2;
            }
        }

        if (p == NULL) {
            p2 = gameTmd + 0x20;
            for (i = 0; i < *(u32 *)(bootInfo + 8); i++, p2 += 8) {
                if (*(u32 *)(p2 + 4) == (u32)__OSNextPartitionType) {
                    p = (u32 *)p2;
                }
            }

            if (p == NULL) {
                OSReport(sNoGameStr);
                __OSReturnToMenuForError();
            }
        }

        *(u32 *)0x80003194 = *(u32 *)((u8 *)p + 4);
        *(u32 *)0x80003198 = *(u32 *)((u8 *)p + 0);

        DVDLowIntType = 0;
        if (*(u8 *)0x80003187 == 0x80) {
            DVDLowOpenPartitionWithTmdAndTicketView(
                *(u32 *)((u8 *)p + 0), (ESTicketView *)ticketView, tmdSize,
                discTmd, 0, NULL, callback);
        } else {
            DVDLowOpenPartition(*(u32 *)((u8 *)p + 0), NULL, 0, NULL,
                                discTmd, callback);
        }
        while (DVDLowIntType == 0) {
            ;
        }
        if (DVDLowIntType != 1) {
            OSReport(sFailStr, DVDLowIntType, 0x3EB);
            __OSReturnToMenuForError();
        }

        DVDLowIntType = 0;
        DVDLowClosePartition(callback);
        while (DVDLowIntType == 0) {
            ;
        }
        if (DVDLowIntType != 1) {
            OSReport(sFailStr, DVDLowIntType, 0x3F5);
            __OSReturnToMenuForError();
        }
    }

    titleId = *(u64 *)((u8 *)discTmd + 0x184);
    err = ESP_InitLib();
    if (err != 0) {
        OSReport(sFailStr, err, 0x400);
        __OSHotResetForError();
    }

    err = ESP_GetTicketViews(titleId, NULL, &count);
    if (count != 1 || err != 0) {
        OSReport(sFailStr, err, 0x409);
        __OSHotResetForError();
    }

    views = OSAllocFromMEM1ArenaLo((count * 0xD8 + 0x1F) & ~0x1Fu, 0x20);
    err = ESP_GetTicketViews(titleId, views, &count);
    if (err != 0) {
        OSReport(sFailStr, err, 0x412);
        __OSHotResetForError();
    }

    DVDLowFinalize();

    codeStart = *(u32 *)0x8000311C;
    codeSize = *(u32 *)0x80003120;
    DCStoreRange((void *)0x80003100, 0x100);

    err = ESP_LaunchTitle(titleId, views);
    if (err != 0) {
        OSReport(sFailStr, err, 0x422);
        __OSHotResetForError();
    }

    ESP_CloseLib();

    DCInvalidateRange((void *)0x80003100, 0x100);

    if (codeStart < *(u32 *)0x8000311C) {
        *(u32 *)0x80003120 = *(u32 *)0x80003120 - (*(u32 *)0x8000311C - codeStart);
        *(u32 *)0x80003128 = *(u32 *)0x80003128 - (*(u32 *)0x8000311C - codeStart);
        *(u32 *)0x80003130 = *(u32 *)0x80003130 - (*(u32 *)0x8000311C - codeStart);
        *(u32 *)0x80003134 = *(u32 *)0x80003134 - (*(u32 *)0x8000311C - codeStart);
        *(u32 *)0x8000311C = codeStart;
    }

    if (codeSize < *(u32 *)0x80003120) {
        __OSInitMemoryProtection();
    }

    __OSInitIPCBuffer();
    IPCReInit();
    IPCCltReInit();
    DVDLowInit();

    DVDLowIntType = 0;
    DVDLowReadDiskID(&id, callback);
    while (DVDLowIntType == 0) {
        ;
    }
    if (DVDLowIntType != 1) {
        OSReport(sFailStr, DVDLowIntType, 0x44E);
        __OSReturnToMenuForError();
    }

    DVDLowIntType = 0;
    if (*(u8 *)0x80003187 == 0x80) {
        DVDLowOpenPartitionWithTmdAndTicketView(
            *(u32 *)((u8 *)p + 0), (ESTicketView *)ticketView, tmdSize,
            discTmd, 0, NULL, callback);
    } else {
        DVDLowOpenPartition(*(u32 *)((u8 *)p + 0), NULL, 0, NULL, discTmd,
                            callback);
    }
    while (DVDLowIntType == 0) {
        ;
    }
    if (DVDLowIntType != 1) {
        OSReport(sFailStr, DVDLowIntType, 0x465);
        __OSReturnToMenuForError();
    }
}

/* .sdata:0x80663240 "%016llx" — retail snprintf format, referenced via
 * sda21 from __OSBootDolSimple. */
extern const char lbl_80665B70[8];

/* .data:0x8054E8B0 "2004/02/01" — retail apploader date string. */
extern const char lbl_80551F90[0x10];

/* Missing from revolution/dvd.h. */
extern void DVDSetAutoInvalidation(BOOL invalidate);

/* Defined at the end of this TU (retail .fn Callback, local). */
extern void Callback(void);
extern volatile int Prepared;

/*
 * Reads the apploader/DOL start sector from the disc and caches it.
 * 0x800030F4 holds the apploader start offset; the 0x40-byte header read
 * carries the size at +0x38. The cached value is 0x910 when the disc has
 * no apploader region. Inlined into __OSBootDolSimple (retail has no
 * standalone symbol; the cache lives in .sbss as
 * "@LOCAL@GetApploaderPosition__Fv@apploaderPosition").
 */
#pragma push
#pragma inline_max_size(10000)
#pragma inline_max_total_size(10000)
static __inline s32 GetApploaderPosition(DVDCommandBlock *cb) {
    static s32 apploaderPosition;

    if (apploaderPosition != 0) {
        return apploaderPosition;
    }
    if (*(s32 *)0x800030F4 != 0) {
        u8 *buf;
        u32 pos;

        buf = (u8 *)OSAllocFromMEM1ArenaLo(0x40, 0x20);
        pos = (u32)(*(s32 *)0x800030F4 >> 2);
        DVDReadAbsAsyncPrio(cb, buf, 0x40, pos, 0, 0);
        while (DVDGetCommandBlockStatus(cb) != 0) {
            if (DVDGetCommandBlockStatus(cb) > 2 ||
                DVDGetCommandBlockStatus(cb) < 0) {
                __OSReturnToMenuForError();
            }
        }
        apploaderPosition = (*(s32 *)0x800030F4 + *(s32 *)(buf + 0x38)) >> 2;
    } else {
        apploaderPosition = 0x910;
    }
    return apploaderPosition;
}
#pragma pop

/*
 * Boots the game DOL: prepares the boot-args block, resets the DVD layer,
 * then either runs the disc apploader (which streams the DOL) or loads the
 * DOL directly at 0x81330000. param1 == -1 selects the apploader path
 * (normal disc boot); otherwise the DOL is read raw at
 * apploaderPosition + ((cb[5] + 0x20) >> 2).
 */
void __OSBootDolSimple(s32 param1, u32 param2, u32 regionStart, u32 regionEnd,
                       s32 param5, s32 argc, char *argv[]) {
    u32 entry;
    void *p2;
    void *p3;
    s32 *p;
    s32 *cb;
    s32 apploaderPosition;
    void *args;
    DVDCommandBlock cb8[8] ALIGN(32);
    ESTitleId titleId ALIGN(32);
    u32 c;
    u32 b;
    u32 a;
    u32 v2;
    u32 v1;
    u32 v0;
    s32 cmpRes;

    OSDisableInterrupts();
    if (__OSInReboot) {
        __OSNextPartitionType = (void *)*(u32 *)0x80003194;
    }
    __OSRestoreCodeExecOnMEM1(0xBA2CF);

    p = (s32 *)OSAllocFromMEM1ArenaLo(0x1C, 1);
    p[0] = 1;
    p[1] = (s32)param2;
    p[3] = (s32)regionStart;
    p[4] = (s32)regionEnd;
    p[5] = param5;

    if (param5 == 0) {
        args = OSAllocFromMEM1ArenaLo(0x2000, 1);
        p[6] = (s32)args;
        if ((u32)__OSNextPartitionType == 2 && !__OSInReboot) {
            PackInstallerArgs((void *)p[6], argc, argv);
        } else {
            PackArgs((void *)p[6], argc, argv);
        }
    }

    DVDInit();
    DVDSetAutoInvalidation(TRUE);
    DVDResume();
    Prepared = 0;
    __DVDPrepareResetAsync((DVDCommandCallback)Callback);
    __OSMaskInterrupts(-0x10);
    __OSUnmaskInterrupts(0x10);
    OSEnableInterrupts();
    while (Prepared != 1) {
        ;
    }
    __OSLaunchNextFirmware();

    if (param2 == 0xA0000000 && !__OSInReboot) {
        if (ESP_InitLib() == 0 && ESP_GetTitleId(&titleId) == 0 &&
            ESP_CloseLib() == 0) {
            snprintf((char *)p[6] + (u32)argv[1], 0x11, lbl_80665B70,
                     titleId);
        }
    }

    cb = (s32 *)OSAllocFromMEM1ArenaLo(0x20, 0x20);
    apploaderPosition = GetApploaderPosition(&cb8[4]);
    DVDReadAbsAsyncPrio(&cb8[5], cb, 0x20, (u32)apploaderPosition, 0, 0);
    while (DVDGetCommandBlockStatus(&cb8[5]) != 0) {
        if (DVDGetCommandBlockStatus(&cb8[5]) > 2 ||
            DVDGetCommandBlockStatus(&cb8[5]) < 0) {
            __OSReturnToMenuForError();
        }
    }

    apploaderPosition = GetApploaderPosition(&cb8[6]);
    DVDReadAbsAsyncPrio(&cb8[7], (void *)0x81200000,
                        (cb[5] + 0x1F) & ~0x1F, (u32)apploaderPosition + 8,
                        0, 0);
    while (DVDGetCommandBlockStatus(&cb8[7]) != 0) {
        if (DVDGetCommandBlockStatus(&cb8[7]) > 2 ||
            DVDGetCommandBlockStatus(&cb8[7]) < 0) {
            __OSReturnToMenuForError();
        }
    }
    ICInvalidateRange((void *)0x81200000, (cb[5] + 0x1F) & ~0x1F);

    if (strncmp((char *)cb, lbl_80551F90, 0xA) > 0) {
        cmpRes = 1;
    } else {
        cmpRes = 0;
    }
    if (cmpRes) {
        /* Disc apploader path: run the apploader, then stream the DOL
         * through its callbacks. */
        if ((u32)param1 == 0xFFFFFFFF) {
            apploaderPosition = GetApploaderPosition(&cb8[3]);
            param1 = apploaderPosition + ((u32)(cb[5] + 0x20) >> 2);
        }
        p[2] = param1;
        ((void (*)(u32 *, u32 *, u32 *))cb[4])(&v0, &v1, &v2);

        p2 = OSAllocFromMEM1ArenaLo(0x1C, 1);
        memcpy(p2, p, 0x1C);
        *(u32 *)0x800030F0 = (u32)p2;
        ((void (*)(s32))v0)((s32)&OSReport);
        OSSetArenaLo(p2);

        while (((s32(*)(u32 *, u32 *, u32 *))v1)(&a, &b, &c) != 0) {
            DVDReadAbsAsyncPrio(&cb8[2], (void *)a, b,
                                c >> __DVDLayoutFormat, 0, 0);
            while (DVDGetCommandBlockStatus(&cb8[2]) != 0) {
                if (DVDGetCommandBlockStatus(&cb8[2]) > 2 ||
                    DVDGetCommandBlockStatus(&cb8[2]) < 0) {
                    __OSReturnToMenuForError();
                }
            }
        }
        entry = ((u32(*)())v2)();
        *(u32 *)0x80003180 = *(u32 *)0x80000000;
        *(u8 *)0x80003184 = 0x80;
        p3 = OSAllocFromMEM1ArenaLo(0x1C, 1);
        memcpy(p3, p, 0x1C);
        *(u32 *)0x800030F0 = (u32)p3;
        *(u32 *)0xCC003024 = 7;
        OSDisableInterrupts();
        Run((void (*)(void))entry);
    } else {
        /* Direct DOL boot at 0x81330000. */
        *(u32 *)0x812FDFF0 = regionStart;
        *(u32 *)0x812FDFEC = regionEnd;
        *(u8 *)0x800030E2 = 1;
        apploaderPosition = GetApploaderPosition(&cb8[1]);
        DVDReadAbsAsyncPrio(&cb8[0], (void *)0x81330000,
                            (cb[6] + 0x1F) & ~0x1F,
                            (u32)apploaderPosition + ((u32)(cb[5] + 0x20) >> 2),
                            0, 0);
        while (DVDGetCommandBlockStatus(&cb8[0]) != 0) {
            if (DVDGetCommandBlockStatus(&cb8[0]) > 2 ||
                DVDGetCommandBlockStatus(&cb8[0]) < 0) {
                __OSReturnToMenuForError();
            }
        }
        ICInvalidateRange((void *)0x81330000, (cb[6] + 0x1F) & ~0x1F);
        OSDisableInterrupts();
        ICFlashInvalidate();
        Run((void (*)(void))0x81330000);
    }
}

/* .sdata:0x80663248 "%d" — retail sprintf format string, referenced via
 * sda21 from __OSBootDol (MWCC_REFERENCE §1h: fixed-size extern keeps the
 * retail pool label instead of a TU-local @N pool symbol). */
extern const char lbl_80665B78[8];

void __OSBootDol(u32 doloffset, u32 restartCode, u32* argv) {
    char doloffInString[20];
    s32 argvlen;
    char** argvToPass;
    s32 i;
    void* saveStart;
    void* saveEnd;

    OSGetSaveRegion(&saveStart, &saveEnd);
    sprintf(doloffInString, lbl_80665B78, doloffset);

    argvlen = 0;
    if (argv) {
        while (argv[argvlen]) {
            argvlen++;
        }
    }

    argvlen += 1;
    argvToPass = OSAllocFromMEM1ArenaLo((argvlen + 1) * sizeof(char*), 1);
    argvToPass[0] = doloffInString;

    for (i = 1; i < argvlen; i++) {
        argvToPass[i] = (char*)argv[i - 1];
    }

    __OSBootDolSimple(0xffffffff, restartCode, (u32)saveStart, (u32)saveEnd,
                      FALSE, argvlen, argvToPass);
}


extern volatile int Prepared;
void Callback() {
    Prepared = 1;
}
extern unsigned long DVDLowIntType;
void callback(unsigned long type) {
    DVDLowIntType = type;
}
