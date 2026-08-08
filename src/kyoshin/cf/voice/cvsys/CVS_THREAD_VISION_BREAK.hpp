#pragma once

#include "kyoshin/cf/voice/cvsys/CVS_THREAD.hpp"
#include "kyoshin/cf/voice/CCharVoice.hpp"

/**
 * CVS_THREAD_VISION_BREAK - voice thread for vision-break status effect.
 *
 * Overrides blank1() to return 0x0F (15), indicating a small buffer
 * allocation for this thread type. Object size: 0x28 (40 bytes).
 * Fields at 0x20/0x24 store CVoiceHandle pointer and a phase/state counter.
 */
class CVS_THREAD_VISION_BREAK : public CVS_THREAD {
public:
    // Size of the thread-local buffer in bytes.
    static const int BUFFER_SIZE = 0x0F;

    CVoiceHandle* field_0x20;   // 0x20: voice handle pointer
    s32 field_0x24;             // 0x24: phase counter / state selector

    int func_802A95FC() { return BUFFER_SIZE; }
};


// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)

// Init-data triples (3 u32s each: {field_0, field_4, callback}).
extern "C" u32 lbl_eu_80539D70[3]; // Initial state triple
extern "C" u32 lbl_eu_80539D7C[3]; // Reset state triple (for func_802A93FC)
extern "C" u32 lbl_eu_80539D88[];  // CVS_THREAD_VISION_BREAK vtable

// Voice-ID tables indexed by mtRand(3)*2 (halfword entries).
extern "C" s16 lbl_eu_80662D40[4]; // Voice IDs for field_0x24 >= 3 phase
extern "C" s16 lbl_eu_80662D48[4]; // Voice IDs for field_0x24 < 3 phase

// Functions from sibling TUs (not yet in headers).
extern "C" void* func_8016FE34(void* r3);
extern "C" void func_800BE924(void* voice);

// Base CVS_THREAD constructor (C-linkage).
extern "C" void __ct__cf_CVS_THREAD(void* self);