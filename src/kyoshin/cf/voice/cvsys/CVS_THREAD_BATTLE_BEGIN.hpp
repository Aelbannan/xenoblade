#pragma once

#include "kyoshin/cf/voice/cvsys/CVS_THREAD.hpp"

/**
 * CVS_THREAD_BATTLE_BEGIN - voice thread for battle-start audio events.
 *
 * Inherits from CVS_THREAD and uses a minimal thread-local buffer
 * of 30 (0x1E) bytes for battle-start voice lines.
 *
 * Layout: 0x24 is a single dedicated voice-handle slot, 0x28 a small array of
 * handle slots, 0x36 is the active index/counter, 0x38 the slot count/limit,
 * and 0x3A a direction flag.
 */
class CVS_THREAD_BATTLE_BEGIN : public CVS_THREAD {
public:
    // Size of the thread-local voice buffer in bytes.
    static const int BUFFER_SIZE = 0x1E;

    int blank1() override;

    u32            field_0x20;           // 0x20
    CVoiceHandle*  field_0x24;           // 0x24: single voice handle
    CVoiceHandle*  field_0x28[3];        // 0x28-0x33: voice-handle slot array
    s16            field_0x34;           // 0x34: sub-state / direction
    s16            field_0x36;           // 0x36: active-slot index / counter
    s16            field_0x38;           // 0x38: slot count / limit
    u8             field_0x3a;           // 0x3a: direction flag
};

// C-linkage imports used by this TU's thread helpers.  Kept here (not in the
// .cpp) because they are genuine retail C/ABI symbols from sibling modules.
extern "C" {
    int func_802A3E88(CVS_THREAD* self);
    void func_802A3BEC(CVS_THREAD* self, CCharVoice* voicePtr);
    int func_802A3D54(CCharVoice* voicePtr, int voiceId, int arg);
    int func_802A7870(void* arr, int capacity, int unused);
    int func_802AED0C(void);
    CVoiceHandle* func_802A330C(int size, int align);
    u8* func_802A34E4(int size);
    int func_802AF388(CVoiceHandle* a);
    void __ct__cf_CVS_THREAD(CVS_THREAD_BATTLE_BEGIN* self);
}

// Voice-playback state helper shared by the voice thread TUs (genuine
// retail C-ABI symbol; kept extern "C" so the name is unmangled).
extern "C" int func_802A77E8(CVoiceHandle* handle);

// Offset layout over CVoiceHandle for the fields this TU's helpers access
// (the manager's large handle block: state flags at 0x3374 and a half-word
// sub-state at 0x3F28).
struct CVSHandleFieldView {
    u8 _00[0x3374];
    u32 field_0x3374;          // 0x3374: handle state flags
    u8 _p[0x3F28 - 0x3378];
    u16 field_0x3F28;          // 0x3F28: half-word sub-state
};

// Globals: init-data triples for this subclass, plus its vtable and the
// post-construction state triple.
extern u32 lbl_eu_8053AC94[3];
extern u32 lbl_eu_8053ACAC[3];
extern u32 lbl_eu_8053AC88[3];
extern u32 lbl_eu_8053ACB8[7];

// Overlay over the virtual-dispatch slot (0x1C) so the manual factory
// constructor can install the subclass vtable.
struct CVSThreadVtableView {
    u8 _00[0x1C];
    u32* vtable;            // 0x1C
};