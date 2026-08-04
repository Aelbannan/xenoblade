// Auto-scaffolded catalog TU for RVL_SDK/src/revolution/os/OSLaunch
// Replace stubs with high-level C/C++ during decomp.

#include <types.h>
#include <revolution/os/OSArena.h>
#include <revolution/os/OSReset.h>
#include <revolution/os/OSStateFlags.h>
#include <revolution/os/OS.h>
#include <revolution/esp/esp.h>
#include <string.h>

// Forward declarations for functions not in headers
extern BOOL OSPlayTimeIsLimited(void);
extern s32 __OSGetPlayTime(void* ticketView, u32* out1, s32* out2);
extern void __OSWriteExpiredFlag(void);
extern BOOL __OSCreateNandbootInfo(void);
extern BOOL __OSWriteNandbootInfo(s32* arg0);

// NANDBootInfo structure (stored at /shared2/sys/NANDBOOTINFO, 0x1020 bytes)
typedef struct NANDBootInfo {
    s32 checksum;        // 0x00
    u8 UNK_0x04[6];      // 0x04
    u8 appType;          // 0x0A
    u8 field_0xB;        // 0x0B
    u32 launchFlags;     // 0x0C
    u8 UNK_0x10[8];      // 0x10
    ESTitleId titleId;          // 0x18
    u8 reserved[0x1000]; // 0x20
} NANDBootInfo; // total 0x1020

// 0x2000 MEM1 buffer: NANDBootInfo at 0xFE0 (0xFE0 + 0x1020 = 0x2000).
typedef struct OSLaunchBuf {
    u8  _pad[0xFE0];
    NANDBootInfo bootInfo;   // 0xFE0
} OSLaunchBuf;

void __OSRelaunchTitle(u32 flags) {
    ESTicketView* ticketView;
    NANDBootInfo* bootInfo;
    ESTitleId titleId __attribute__((aligned(32)));
    void* buffer;
    s32 result;
    u32 count = 1;
    u32 playTimeUsed;
    s32 playTimeRemaining;
    OSStateFlags state __attribute__((aligned(32)));
    OSSetArenaLo((void*)0x81280000);
    OSSetArenaHi((void*)0x812F0000);

    if (ESP_InitLib() != 0)
        __OSReturnToMenuForError();

    if (ESP_GetTitleId(&titleId) != 0)
        __OSReturnToMenuForError();

    ticketView = (ESTicketView*)OSAllocFromMEM1ArenaLo(0xE0, 0x20);
    if (ticketView == (ESTicketView*)NULL)
        __OSReturnToMenuForError();

    memset(ticketView, 0, 0xE0);

    result = ESP_DiGetTicketView(NULL, ticketView);

    if (result == -0x3F9) {
        if (ESP_GetTicketViews(titleId, NULL, &count) != 0)
            __OSReturnToMenuForError();

        buffer = OSAllocFromMEM1ArenaLo((count * 0xD8 + 0x1F) & ~0x1F, 0x20);
        if (buffer == (void*)NULL)
            __OSReturnToMenuForError();

        if (ESP_GetTicketViews(titleId, (ESTicketView*)buffer, &count) != 0)
            __OSReturnToMenuForError();

        memcpy(ticketView, buffer, 0xD8);

    } else if (result != 0) {
        __OSReturnToMenuForError();

    } else {
        if (OSPlayTimeIsLimited()) {
            playTimeUsed = 0;
            playTimeRemaining = -1;
            __OSGetPlayTime(ticketView, &playTimeUsed, &playTimeRemaining);

            if (playTimeRemaining == 0) {
                __OSWriteExpiredFlag();
                __OSReturnToMenuForError();
            }
        }
    }

    buffer = OSAllocFromMEM1ArenaLo(0x2000, 0x40);
    bootInfo = &((OSLaunchBuf*)buffer)->bootInfo;

    memset(buffer, 0, 0x2000);

    bootInfo->titleId = titleId;
    bootInfo->appType = OSGetAppType();
    bootInfo->field_0xB = 1;
    bootInfo->launchFlags = flags | 0x80000000;

    __OSCreateNandbootInfo();
    __OSWriteNandbootInfo((s32*)bootInfo);

    __OSReadStateFlags(&state);
    (&state)->BYTE_0x5 = 3;
    __OSWriteStateFlags(&state);

    if (ESP_LaunchTitle(titleId, ticketView) != 0)
        __OSReturnToMenuForError();

    while (1)
        ;
}
