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
    CVoiceHandle* field_0x20;  // 0x20: voice handle pointer slot
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
}

extern "C" void __ct__cf_CVS_THREAD();

// Globals: slot-state init table and this subclass's vtable.
extern u32 lbl_eu_80539CE8[3];
extern u32 lbl_eu_80539CF4[3];