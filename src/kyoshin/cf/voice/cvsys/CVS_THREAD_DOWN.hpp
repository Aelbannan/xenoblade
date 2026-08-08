#pragma once

#include <types.h>

#include "kyoshin/cf/voice/cvsys/CVS_THREAD.hpp"
#include "kyoshin/cf/voice/CCharVoice.hpp"

/**
 * CVS_THREAD_DOWN - voice thread variant for "Down" (knocked-down) audio.
 *
 * Inherits from CVS_THREAD and returns a thread-local buffer.
 * Total object size: 0x28 (40 bytes).
 * Fields at 0x20/0x24 store CVoiceHandle pointers to the two voice slots.
 */
class CVS_THREAD_DOWN : public CVS_THREAD {
public:
    CVoiceHandle* field_0x20;  // 0x20: voice handle pointer (slot 1)
    CVoiceHandle* field_0x24;  // 0x24: voice handle pointer (slot 2)
};

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" {
    int func_802A3E88(CVS_THREAD* self);
    void func_802A3BEC(CVS_THREAD* self, CCharVoice* voicePtr);
    int func_802A3C44(CVS_THREAD* self, CCharVoice* voicePtr, int voiceId);
    CVS_THREAD_DOWN* func_802A330C(int size, int align);
    u8* func_802A34E4(int size);
}

extern "C" void __ct__cf_CVS_THREAD();

extern "C" u32 lbl_eu_80539A68[3];
extern "C" u32 lbl_eu_80539A74[3];
extern "C" u32 lbl_eu_80539A80[3];
extern "C" u32 lbl_eu_80539A8C[7];