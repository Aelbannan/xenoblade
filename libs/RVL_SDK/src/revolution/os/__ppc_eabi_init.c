#include <revolution/BASE.h>
#include <revolution/OS.h>

#pragma section ".ctors$00"
DECL_SECTION(".ctors$00") extern funcptr_t _ctors[];
#pragma section ".dtors$00"
DECL_SECTION(".dtors$00") extern funcptr_t _dtors[];

asm void __init_hardware(void) {
    // clang-format off
    nofralloc

    // Enable floating-point
    mfmsr r0
    ori r0, r0, MSR_FP
    mtmsr r0

    // Backup LR
    mflr r31

    bl __OSPSInit
    bl __OSFPRInit
    bl __OSCacheInit

    // Restore LR
    mtlr r31
    blr
    // clang-format on
}

asm void __flush_cache(register void* dst, register size_t n) {
    // clang-format off
    nofralloc

    lis r5, 0xFFFFFFF1@h
    ori r5, r5, 0xFFFFFFF1@l
    and r5, r5, dst

    subf r3, r5, r3
    add n, n, r3

_loop:
    dcbst 0, r5
    sync
    icbi 0, r5
    addic r5, r5, 8
    subic. n, n, 8
    bge _loop

    isync
    blr
    // clang-format on
}

// Must not inline __init_cpp into __init_user (retail keeps a 0x20 trampoline).
#pragma push
#pragma optimization_level 4
#pragma scheduling on
#pragma peephole on
#pragma global_optimizer on
#pragma dont_inline on
void __init_cpp(void) {
    funcptr_t* ctor;

    for (ctor = _ctors; *ctor != NULL; ctor++) {
        (*ctor)();
    }
}
#pragma dont_inline off

// Retail inlines the dtor walk + PPCHalt into exit (no separate
// __fini_cpp / _ExitProcess symbols in this unit's .text).
void exit(void) {
    funcptr_t* dtor;

    for (dtor = _dtors; *dtor != NULL; dtor++) {
        (*dtor)();
    }
    PPCHalt();
}
#pragma pop

// __init_user (0x20 retail) is a full-frame `bl __init_cpp` trampoline.  The
// tail-call fold that collapses `{ __init_cpp(); }` into a bare `b` is keyed to
// the GLOBAL -opt level (>= -O2); per-function pragmas cannot undo it while the
// unit's global level is -O4,p.  The unit is therefore compiled at -O1,p (see
// configure.py) and the two sibling functions are re-raised to -O4 with the
// scheduling/peephole/global-optimizer bundle above, giving ALL THREE functions
// byte-identical retail codegen (verified 2026-08, Wii/1.1).
void __init_user(void) {
    __init_cpp();
}
