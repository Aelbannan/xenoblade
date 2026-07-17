#include <revolution/BASE.h>
#include <revolution/DB.h>
#include <revolution/OS.h>

BOOL DBVerbose;
OSDebugInterface* __DBInterface;

void DBInit(void) {
    __DBInterface =
        (OSDebugInterface*)OSPhysicalToCached(OS_PHYS_DEBUG_INTERFACE);
    __DBInterface->exceptionHook = OSCachedToPhysical(__DBExceptionDestination);
    DBVerbose = TRUE;
}

//unused
void DBIsDebuggerPresent(){
}

void __DBExceptionDestinationAux(void) {
    const void* physCtx = (void*)OS_PHYS_CURRENT_CONTEXT_PHYS;
    OSContext* ctx = (OSContext*)OSPhysicalToCached(*(u32*)physCtx);
    OSReport("DBExceptionDestination\n");
    OSDumpContext(ctx);
    PPCHalt();
}

void __DBExceptionDestination(void) {
    __asm__ volatile (
        "mfmsr 3\n\t"
        "ori 3, 3, 0x30\n\t"
        "mtmsr 3\n\t"
        "b __DBExceptionDestinationAux"
    );
}

BOOL __DBIsExceptionMarked(u8 exc) {
    return __DBInterface->exceptionMask & (1 << exc);
}

//unused
void __DBMarkException(){
}

//unused
void __DBSetPresent(){
}

void DBPrintf(const char* fmt, ...) {
    (void)fmt;
}
