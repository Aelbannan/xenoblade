#pragma once

#include <types.h>
#include "kyoshin/cf/voice/cvsys/CVS_THREAD.hpp"
#include "kyoshin/cf/voice/CCharVoice.hpp"

// Forward declarations for the C-linkage imports below.
struct CVoiceHandle;
class CVS_THREAD_HAGE;

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" {
    int func_802A3E88(CVS_THREAD* self);
    void func_802A3BEC(CVS_THREAD* self, CCharVoice* voicePtr);
    int func_802A3C44(CVS_THREAD* self, CCharVoice* voicePtr, int voiceId);
    int func_802A77E8(CVoiceHandle* handle);
    int func_802A7850(int iter);
    int func_802A7B90(CVoiceHandle* handle1, CVoiceHandle* handle2);
    CVS_THREAD_HAGE* func_802A330C(int size, int align);
    u8* func_802A34E4(int size);
    void __ct__cf_CVS_THREAD(CVS_THREAD_HAGE* self);
}

// Init data tables for slot states (3 u32s each).
extern "C" u32 lbl_eu_80539BA8[3];
extern "C" u32 lbl_eu_80539BB4[3];
extern "C" u32 lbl_eu_80539BC0[3];

// Vtable for CVS_THREAD_HAGE (0x1C = 28 bytes = 7 entries).
extern "C" u32 lbl_eu_80539BCC[7];

/**
 * CVS_THREAD_HAGE - voice thread variant for "Hage" (bald/bare) audio.
 *
 * Inherits from CVS_THREAD and returns a smaller thread-local buffer
 * (240 bytes) compared to the BUF variant (260 bytes).
 */
class CVS_THREAD_HAGE : public CVS_THREAD {
public:
    // Size of the thread-local voice buffer in bytes.
    static const int BUFFER_SIZE = 0xF0;

    CVoiceHandle* field_0x20;  // 0x20: voice handle pointer (slot 1)
    CVoiceHandle* field_0x24;  // 0x24: voice handle pointer (slot 2)

    int blank1() override;
};

// Voice-handle type. The actual CCharVoice is embedded at offset 0x3E9C
// within the handle allocation (0x3E9C bytes of handle data + CCharVoice).
struct CVoiceHandle {
    u32* vtable;                         // 0x00: vtable pointer
    u8 _pad[0x3E9C - sizeof(u32*)];       // 0x04-0x3E9B: handle data
    CCharVoice voice;                    // 0x3E9C: the actual voice object
};
