#pragma once

#include "kyoshin/cf/voice/cvsys/CVS_THREAD.hpp"

namespace cf {

/**
 * CVS_THREAD_CHAIN - voice thread chain node.
 *
 * Inherits from CVS_THREAD and adds a link pointer (field_0x20) for chaining
 * multiple voice threads together (e.g. multi-part battle voices).
 * Object size: 0x24 (36 bytes).
 */
class CVS_THREAD_CHAIN : public CVS_THREAD {
public:
    CVoiceHandle* field_0x20;  // 0x20: voice handle slot

    int func_802A5A0C() { return 1; }
};

} // namespace cf

// C-linkage imports used by this TU's thread helpers / factory. Kept here
// (not in the .cpp) because they are genuine retail C/ABI symbols imported
// from sibling voice modules.
extern "C" {
    int           func_802A3E88(CVS_THREAD* self);
    void          func_802A3BEC(CVS_THREAD* self, CCharVoice* voicePtr);
    int           func_802A3C44(CVS_THREAD* self, CCharVoice* voicePtr, int voiceId);
    void          func_802A3E28(CVS_THREAD* self);
    int           func_802A3D54(CCharVoice* voicePtr, int voiceId, int arg);
    CVoiceHandle* func_802A330C(int size, int align);
    u8*           func_802A34E4(int size);
    int           func_800BE8F4(CCharVoice* voicePtr);
    void          __ct__cf_CVS_THREAD();
}

// Globals: init-state triple and this subclass's vtable.
extern u32 lbl_eu_80539A30[3];
extern u32 lbl_eu_80539A3C[7];