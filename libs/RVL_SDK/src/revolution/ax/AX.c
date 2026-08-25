#include <revolution/AX.h>
#include <revolution/OS.h>

// Retail .data slice is the version string alone (0x45 incl. NUL); the split
// ends there and the next unit's .data supplies its own alignment.
static char s_AXVersionStr[0x45] =
    "<< RVL_SDK - AX \trelease build: Feb 27 2009 10:01:36 (0x4302_145) >>";

// Retail .sdata slice is just the version-string pointer (4 bytes).
struct AXVersionDesc {
    const char* str; // +0x0: pointer to version string
} __AXVersion = { s_AXVersionStr };

static BOOL __init = FALSE;

void AXInit(void) {
    if (!__init) {
        OSRegisterVersion(__AXVersion.str);

        __AXAllocInit();
        __AXVPBInit();
        __AXSPBInit();
        __AXAuxInit();
        __AXClInit();
        __AXOutInit(0);

        __init = TRUE;
    }
}

//unused in Xenoblade retail: AXInitEx, AXInitSpecifyMem, AXInitExSpecifyMem

void AXQuit(void) {
    if (__init) {
        __AXOutQuit();
        __AXAllocQuit();
        __AXVPBQuit();
        __AXSPBQuit();
        __AXAuxQuit();
        __AXClQuit();

        __init = FALSE;
    }
}

BOOL AXIsInit(void) {
    return __init;
}
