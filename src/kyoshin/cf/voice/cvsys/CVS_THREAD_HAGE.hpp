#pragma once

#include <types.h>
#include "kyoshin/cf/voice/cvsys/CVS_THREAD.hpp"
#include "kyoshin/cf/voice/CCharVoice.hpp"
#include "kyoshin/cf/object/CActorParam.hpp"

// Forward declarations for the C-linkage imports below.
class CVS_THREAD_HAGE;

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" {
    int func_802A3E88(CVS_THREAD* self);
    void func_802A3BEC(CVS_THREAD* self, CCharVoice* voicePtr);
    int func_802A3C44(CVS_THREAD* self, CCharVoice* voicePtr, int voiceId);
    int func_802A77E8(CVoiceHandle* handle);
    int func_802A7850(int iter);
    int func_802A7B90(CVoiceHandle* handle1, CVoiceHandle* handle2);
    u8* func_802A34E4(int size);
    void __ct__cf_CVS_THREAD();

    // Runtime rethrow (NMWException.h): declared noreturn so MWCC elides the
    // __end__catch epilogue of a catch-all handler that ends with `bl __throw`
    // (retail catch-all handlers end at the rethrow).
    __declspec(noreturn) void __throw(char* throwtype, void* location,
                                      void* dtor);
}

// Voice-idle check: cf::CActorParam slot 138 at vtable offset 0x2BC (retail
// CActorParam_isBattleLocked__Q22cf11CActorParamFv, Fv arity). Voice
// handles are CActorParam-derived (CfObjectPc lineage), so call the real
// slot through cf::CActorParam instead of a phantom view.

// Init data tables for slot states (3 u32s each).
extern "C" u32 lbl_eu_80539BA8[3];
extern "C" u32 lbl_eu_80539BB4[3];
extern "C" u32 lbl_eu_80539BC0[3];

// Retail virtual table for CVS_THREAD_HAGE (0x1C = 28 bytes = 7 entries).
extern "C" u32 lbl_eu_80539BCC[7];

/**
 * CVS_THREAD_HAGE - voice thread variant for "Hage" (bald/bare) audio.
 *
 * Inherits from CVS_THREAD and returns a smaller thread-local buffer
 * (240 bytes) compared to the BUF variant (260 bytes).
 */

// Init-state triple view over lbl_eu_80539BA8 (word 0 is stored into the
// object's pointer-sized state0 field, hence the pointer type).
struct HAGEInitData {
    u32* w0;   // 0x00
    u32 w1;    // 0x04
    u32 w2;    // 0x08
};

// Raw layout of the CVS_THREAD_HAGE object exposing the implicit vtable
// pointer at 0x1C (owned by the CVS_THREAD base) so the factory can
// override it with the HAGE vtable, plus the two voice slots.
struct CVS_THREAD_HAGE_raw {
    u32* state0;                // 0x00: init-state word 1
    u32 state1;                 // 0x04: init-state word 2
    u32 state2;                 // 0x08: init-state word 3
    u32 field_0x0C;             // 0x0C
    u32 field_0x10;             // 0x10
    u32 field_0x14;             // 0x14
    u32 field_0x18;             // 0x18
    const u32* vtable;  // 0x1C: subclass vtable (retail label, u32 words)
    CVoiceHandle* slot1;        // 0x20: voice handle pointer (slot 1)
    CVoiceHandle* slot2;        // 0x24: voice handle pointer (slot 2)
};

class CVS_THREAD_HAGE : public CVS_THREAD {
public:
    // Size of the thread-local voice buffer in bytes.
    static const int BUFFER_SIZE = 0xF0;

    CVoiceHandle* field_0x20;  // 0x20: voice handle pointer (slot 1)
    CVoiceHandle* field_0x24;  // 0x24: voice handle pointer (slot 2)

    int blank1() override;
};

// CVoiceHandle is defined once in the shared base header CVS_THREAD.hpp.
