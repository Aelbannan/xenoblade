#include <revolution/OS.h>

static u32 SaveEnd;
static u32 SaveStart;

void __OSReboot(u32 restartCode, u32 dolOffset){
    BOOL enabled;
    u32 argv[2];

    enabled = OSDisableInterrupts();
    OSSetArenaLo((void*)0x81280000);
    OSSetArenaHi((void*)0x812F0000);
    argv[0] = 0;
    __OSNextPartitionType = *(void**)OSPhysicalToCached(OS_PHYS_BOOT_PARTITION_TYPE);
    __OSBootDol(dolOffset, restartCode | 0x80000000, argv);
}

void OSGetSaveRegion(u32* saveStartPtr, u32* saveEndPtr){
    *saveStartPtr = SaveStart;
    *saveEndPtr = SaveEnd;
}
