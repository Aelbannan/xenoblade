#pragma once

#include "kyoshin/cf/voice/cvsys/CVS_THREAD.hpp"

/**
 * CVS_THREAD_SUDDEN - voice thread for "sudden" audio events.
 *
 * Overrides blank1() to return 1, indicating a minimal/flag-only buffer
 * rather than a full voice processing allocation.
 *
 * Layout: 7 CVS_THREAD base u32s (0x00-0x18) + vtable at 0x1C + a single
 * voice-handle slot at 0x20 (the factory allocates 0x24 bytes).
 */
class CVS_THREAD_SUDDEN : public CVS_THREAD {
public:
    CVoiceHandle* voiceHandle;  // 0x20: voice handle pointer slot
    int blank1() override;
};

// C-linkage imports used by this TU's thread helpers / factory. Kept here
// (not in the .cpp) because they are genuine retail C/ABI symbols imported
// from another voice module.
extern "C" {
    int  func_802A3E88(CVS_THREAD* self);
    void func_802A3BEC(CVS_THREAD* self, CCharVoice* voicePtr);
    int  func_802A3C44(CVS_THREAD* self, CCharVoice* voicePtr, int voiceId);
    void func_802A3E28(CVS_THREAD* self);
    CVoiceHandle* func_802A330C(int size, int align);
    u8*  func_802A34E4(int size);

    // Base-subobject constructor (implemented in CVS_THREAD.cpp).
    CVS_THREAD* __ct__cf_CVS_THREAD(CVS_THREAD* object);

    // Runtime rethrow (NMWException.h): declared noreturn so MWCC elides the
    // __end__catch epilogue of a catch-all handler that ends with `bl __throw`
    // (retail catch-all handlers end at the rethrow). All three parameters
    // are opaque pointers in the runtime ABI.
    __declspec(noreturn) void __throw(char* throwtype, char* location,
                                      char* dtor);
}

// Raw view used by the factory to reach the base state words (0x00-0x08),
// the subclass vtable (0x1C) and the null voice-handle slot (0x20). The
// compiler-managed vptr overlaps the base's 7 leading words, so the manual
// retail vtable store and init-triple copy need this byte-exact view.
struct CVS_THREAD_SUDDEN_raw {
    u32* state0;    // 0x00: base state word 1 (pointer)
    u32 state4;     // 0x04: base state word 2
    u32 state8;     // 0x08: base state word 3
    u8 _0C[0x10];   // 0x0C-0x1B
    u32 vtable;     // 0x1C
    CVoiceHandle* voiceHandle;  // 0x20
};

// Globals: slot-state init table and this subclass's vtable (0x1C bytes).
extern u32 lbl_eu_80539CE8[3];
extern u32 lbl_eu_80539CF4[7];