#pragma once

#include "kyoshin/cf/voice/cvsys/CVS_THREAD.hpp"

// CVS_THREAD_FAINT: voice thread state machine for the "faint" status effect.
// Allocates 0xF0 (240) bytes.  Overrides vmethod[2] (blank1) to return that size.
// Two voice-handle slots at 0x20/0x24, mirroring the DOWN/BATTLE_END variants.
class CVS_THREAD_FAINT : public CVS_THREAD {
public:
    // Size of the thread-local buffer in bytes.
    static const int BUFFER_SIZE = 0xF0;

    CVoiceHandle* field_0x20;  // 0x20: voice handle pointer (slot 1)
    CVoiceHandle* field_0x24;  // 0x24: voice handle pointer (slot 2)

    int blank1() override;
};

// Init-state triples and this subclass's vtable (imported data symbols).
extern u32 lbl_eu_80539B58[3];
extern u32 lbl_eu_80539B64[3];
extern u32 lbl_eu_80539B70[3];
extern u32 lbl_eu_80539B7C[7];