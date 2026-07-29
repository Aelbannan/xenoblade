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