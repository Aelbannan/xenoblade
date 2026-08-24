#pragma once

#include "kyoshin/cf/voice/cvsys/CVS_THREAD.hpp"

// CVS_THREAD_FAINT: voice thread state machine for the "faint" status effect.
// Allocates 0xF0 (240) bytes.  Overrides vmethod[2] (blank1) to return that size.
// Two voice-handle slots at 0x20/0x24, mirroring the DOWN/BATTLE_END variants.
class CVS_THREAD_FAINT : public CVS_THREAD {
public:
    // Size of the thread-local buffer in bytes.
    static const int BUFFER_SIZE = 0xF0;

    // Voice handle slots, mirroring the DOWN/BATTLE_END variants.
    CVoiceHandle* slot1;  // 0x20: owner-1 voice handle
    CVoiceHandle* slot2;  // 0x24: owner-2 voice handle

    int blank1() override;
};

// Init-state triples and this subclass's vtable (imported data symbols).
extern u32 lbl_eu_80539B58[3];
extern u32 lbl_eu_80539B64[3];
extern u32 lbl_eu_80539B70[3];
extern u32 lbl_eu_80539B7C[7];

struct CVS_THREAD_FAINT_VTable;

// Raw layout of the CVS_THREAD_FAINT object exposing the implicit vtable
// pointer at 0x1C (owned by the CVS_THREAD base) so the factory can
// override it with the FAINT vtable, plus the two voice slots.
struct CVS_THREAD_FAINT_raw {
    u32* state0;                // 0x00: init-state word 1
    u32 state1;                 // 0x04: init-state word 2
    u32 state2;                 // 0x08: init-state word 3
    u32 field_0x0C;             // 0x0C
    u32 field_0x10;             // 0x10
    u32 field_0x14;             // 0x14
    u32 field_0x18;             // 0x18
    const CVS_THREAD_FAINT_VTable* vtable;  // 0x1C
    CVoiceHandle* slot1;        // 0x20
    CVoiceHandle* slot2;        // 0x24
};