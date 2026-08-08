#pragma once

#include "kyoshin/cf/voice/cvsys/CVS_THREAD.hpp"
#include "kyoshin/cf/voice/CCharVoice.hpp"

// CVoiceHandle is defined once in the shared base header CVS_THREAD.hpp.

/**
 * CVS_THREAD_VISION_TELL - voice thread for vision-tell audio events.
 *
 * Overrides blank1() to return 0x32 (50), indicating a small buffer
 * allocation for this thread type. Object size: 0x28 (40 bytes).
 * Fields at 0x20/0x24 store CVoiceHandle pointers for two voice slots.
 */
class CVS_THREAD_VISION_TELL : public CVS_THREAD {
public:
    // Size of the thread-local voice buffer in bytes.
    static const int BUFFER_SIZE = 0x32;

    CVoiceHandle* field_0x20;  // 0x20: voice handle pointer (slot 1)
    CVoiceHandle* field_0x24;  // 0x24: voice handle pointer (slot 2)

    int blank1() override;
};

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" u32 lbl_eu_80539DB0[3];  // Init data tables for slot states (3 u32s each)
extern "C" u32 lbl_eu_80539DBC[3];
extern "C" u32 lbl_eu_80539DC8[3];

// Random voice ID tables (short arrays in .sdata).
extern "C" short lbl_eu_80662D58[4];
extern "C" short lbl_eu_80662D60[4];

// Vtable for CVS_THREAD_VISION_TELL (0x1C = 28 bytes = 7 entries).
extern "C" u32 lbl_eu_80539DD4[7];
