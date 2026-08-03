#include <revolution/AX.h>
#include <revolution/OS.h>

// Retail .data is 0x48 bytes: the version string (0x45 incl. NUL) followed by 3
// zero pad bytes so the next unit's .data starts 8-aligned.
static char s_AXVersionStr[0x48] =
    "<< RVL_SDK - AX \trelease build: Feb 27 2009 10:01:36 (0x4302_145) >>";

// Retail .sdata slice is 8 bytes: the version string pointer (reloc) followed by
// 4 zero pad bytes (gap_09_80662F8C_sdata) aligning the next unit's .sdata.
struct AXVersionDesc {
    const char* str; // +0x0: pointer to version string
    u32 pad;         // +0x4: zero pad
} __AXVersion = { s_AXVersionStr, 0 };

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
