// CVS_THREAD_REVIVE: Voice thread variant for revival/healing audio.
// Seven functions: factory ctor, play/init, completion callback, voice
// removal, and buffer-size getter.

#include "kyoshin/cf/voice/cvsys/CVS_THREAD_REVIVE.hpp"
#include "kyoshin/harness_catalog.hpp"
#include "monolib/math/Random.hpp"

// us-802aae00 (__ct__802A86CC)
// Factory/constructor for CVS_THREAD_REVIVE. Takes two owner objects (with a
// field at offset 0x3F00 that must have bit 1 set), allocates a voice-handle
// buffer (0xE6 bytes, discarded) and the object itself (0x28 bytes), calls
// the base constructor, sets vtable/owner fields, and copies init data from
// lbl_eu_80539C98.
extern "C" CVS_THREAD_REVIVE* __ct__802A86CC(CVoiceHandle* owner1, CVoiceHandle* owner2) {
    // Both owners must have their 0x3F00 field's bit 1 set
    if (!(owner1->field_0x3F00 & 2)) return NULL;
    if (!(owner2->field_0x3F00 & 2)) return NULL;

    // Allocate voice-handle buffer (0xE6 bytes, discarded)
    if (CCharVoiceMan_DispatchById(0xE6, 1) == NULL) return NULL;

    // Allocate the actual CVS_THREAD_REVIVE object
    CVS_THREAD_REVIVE* self = (CVS_THREAD_REVIVE*)CCharVoiceMan_AllocVoiceArena(0x28);
    if (self == NULL) return NULL;

    // Retail emits a redundant self null re-check (the `beq`) guarding the
    // constructor try-block; mirror it so the guard survives. The catch
    // rethrows via the runtime __throw(0,0,0) so MWCC elides the
    // __end__catch epilogue (retail's catch-all ends at `bl __throw`).
    CVS_THREAD_REVIVE_raw* raw = (CVS_THREAD_REVIVE_raw*)self;
    if (self != NULL) {
        try {
            // Base constructor (self in r3)
            __ct__cf_CVS_THREAD(self);

            // Override the vtable at 0x1C with the REVIVE vtable, then slots.
            raw->vtable = lbl_eu_80539CBC;
            self->field_0x20 = owner1;
            self->field_0x24 = owner2;
        } catch (...) {
            __throw(0, 0, 0);
        }
    }

    // Copy the init-state triple as the base's ptmf member (VoiceCb is
    // 12 bytes; DOWN precedent: member-typed access colors the pair
    // r0/r4 with the single r5 base, matching retail).
    ((CVS_THREAD*)self)->unk0 = ((CVS_THREAD*)lbl_eu_80539C98)->unk0;

    return self;
}

// us-802aaf08 (ReviveVxPlaySlot1)
// Advance/play function. Copies init data from lbl_eu_80539CA4, then if both
// voice slots are populated and slot 1 is inactive, plays a state-based voice
// (slot-2 iterator + 0x204) or a fallback voice (0x21E) depending on iterator
// validity, slot conflict, and a random gate.
void ReviveVxPlaySlot1(CVS_THREAD_REVIVE* self) {
    // Copy init data using pointer increment to force lwzu pattern
    u32 v0;
    const u32* p = lbl_eu_80539CA4;
    v0 = *p++;
    CVoiceHandle* handle20 = self->field_0x20;
    CVS_THREAD_REVIVE_raw* raw = (CVS_THREAD_REVIVE_raw*)self;
    raw->state1 = *p++;
    raw->state0 = (u32*)v0;
    raw->state2 = *p;

    // Both voice slots must be populated
    if (handle20 == NULL) return;
    if (self->field_0x24 == NULL) return;

    // Check if slot 1 voice is still active (virtual dispatch -> r12 chain)
    if (((CVoiceHandleVTV*)handle20)->isActive() != 0) return;

    // Get voice iterator from slot 2 handle
    int iter = HpVoice_MapSubState(self->field_0x24);

    if (HpVoice_ValidIter(iter) != 0 &&
        HpVoice_CheckPair26(self->field_0x20, self->field_0x24) == 0 &&
        ml::math::mtRand(2) != 0) {
        // Valid state -- play the state-specific voice (iter + 0x204)
        CVoiceHandle* tmpHandle = self->field_0x20;
        CCharVoice* voicePtr = (CCharVoice*)tmpHandle;
        if (tmpHandle != NULL) {
            voicePtr = &tmpHandle->voice;
        }
        if (TryRequestVoice(self, voicePtr, iter + 0x204) == 0) {
            self->PlayThreadVoice();
        }
    } else {
        // Fallback voice 0x21E
        CVoiceHandle* tmpHandle = self->field_0x20;
        CCharVoice* voicePtr = (CCharVoice*)tmpHandle;
        if (tmpHandle != NULL) {
            voicePtr = &tmpHandle->voice;
        }
        if (TryRequestVoice(self, voicePtr, 0x21E) == 0) {
            self->PlayThreadVoice();
        }
    }
}

// us-802ab038 (ReviveVxPlaySlot2)
// Advance/play function for voice slot 2 (field_0x24). Copies init data from
// lbl_eu_80539CB0, checks the slot-2 voice state (vtable method at 0x308);
// if >= 3 plays 0x711, otherwise a random voice (0x714 when mtRand==0, else
// 0x710).
void ReviveVxPlaySlot2(CVS_THREAD_REVIVE* self) {
    if (IsThreadVoiceActive(self) != 0) {
        return;
    }

    // Copy init data using DOWN-proven statement order so MWCC keeps the
    // source pointer and first word in the same registers as retail.
    u32 v0;
    u32* src = lbl_eu_80539CB0;
    v0 = *src++;
    CVoiceHandle* handle24 = self->field_0x24;
    CVS_THREAD_REVIVE_raw* raw = (CVS_THREAD_REVIVE_raw*)self;
    raw->state1 = *src++;
    raw->state0 = (u32*)v0;
    raw->state2 = *src;

    if (handle24 != NULL) {
        // State check at vtable 0x308 via virtual dispatch (r12 chain).
        if (((CVoiceHandleVTV*)handle24)->state() >= 3) {
            // High state -- play 0x711
            CVoiceHandle* tmpHandle = self->field_0x24;
            CCharVoice* voicePtr = (CCharVoice*)tmpHandle;
            if (tmpHandle != NULL) {
                voicePtr = &tmpHandle->voice;
            }
            if (TryRequestVoice(self, voicePtr, 0x711) == 0) {
                self->PlayThreadVoice();
            }
        } else {
            // Low state -- play weighted random voice
            int voiceId = ml::math::mtRand(2) != 0 ? 0x710 : 0x714;
            CVoiceHandle* tmpHandle = self->field_0x24;
            CCharVoice* voicePtr = (CCharVoice*)tmpHandle;
            if (tmpHandle != NULL) {
                voicePtr = &tmpHandle->voice;
            }
            if (TryRequestVoice(self, voicePtr, voiceId) == 0) {
                self->PlayThreadVoice();
            }
        }
    }
}

// us-802ab134 (ReviveVxOnComplete)
// Completion callback: if no active voice, call the playback-start virtual.
void ReviveVxOnComplete(CVS_THREAD_REVIVE* self) {
    if (IsThreadVoiceActive(self) == 0) {
        self->PlayThreadVoice();
    }
}

// us-802ab17c (ReviveVxRemoveVoice)
// Remove a voice from the slots by matching its embedded CCharVoice pointer.
void ReviveVxRemoveVoice(CVS_THREAD_REVIVE* self, CCharVoice* voicePtr) {
    DetachThreadVoice(self, voicePtr);

    // Slot 0x20: load handle, bias if non-null, compare with voicePtr
    CVoiceHandle* handle = self->field_0x20;
    CCharVoice* biased = (CCharVoice*)handle;
    if (handle != NULL) {
        biased = &handle->voice;
    }
    if (biased == voicePtr) {
        self->field_0x20 = NULL;
    }

    // Slot 0x24: same pattern
    handle = self->field_0x24;
    biased = (CCharVoice*)handle;
    if (handle != NULL) {
        biased = &handle->voice;
    }
    if (biased == voicePtr) {
        self->field_0x24 = NULL;
    }
}

// Buffer-size getter for the blank1 vtable slot (retail
// blank1__17CVS_THREAD_REVIVEFv, 0x8 bytes: li r3,0xE6; blr). Defined with
// C linkage under the exact retail spelling (CVS_THREAD_HP precedent:
// HpVoice_GetBufSize); the blank1() member decl in the header still
// satisfies the class interface, and since no virtual is defined in this TU
// MWCC emits no vtable here (the thread vtable is the retail lbl_eu_80539CBC
// data). Returns 0xE6 (230).
extern "C" int blank1__17CVS_THREAD_REVIVEFv() {
    return CVS_THREAD_REVIVE::BUFFER_SIZE;
}

// Retail section reconstruction (measured in
// .scratch/dlabels/a2/CVS_THREAD_REVIVE.txt): the .rodata typeinfo name
// (sized to absorb the retail pad), three .data init-state triples
// ({0, -1, callback}), the 7-word thread vtable ({typeinfo, 0, play,
// remove, blank1, alwaysTrue, nullDispatch}), the trailing typeinfo-base
// word plus padding, and the .sdata typeinfo object ({name, trailing}).
// Global-scope data symbols: MWCC does not mangle them.
//
// NOTE (frozen-rule residual): this TU carries a UNIT_RULES
// extern_data_sections=(".data", ".rodata", ".sdata") entry that
// zeroes these sections in the link-time reloc.o. The retail split object
// genuinely carries them (0x50/0x18/0x8, reconstructed byte-identical
// below), so that rule entry is stale and should be removed; until then
// the link keeps resolving these labels as UNDEF. lbl_eu_80662CC0 lives in
// the CCharVoiceMan range and is referenced, not defined, here.
extern "C" void PlayThreadVoice();
extern "C" bool CCharVoiceMan_AlwaysTrue();
extern "C" int CCharVoiceMan_NullVoiceDispatch(void* self);
extern u32 lbl_eu_80662CC0[2];
extern u32 lbl_eu_80662D28[2];

extern const char lbl_eu_805107E8[0x18] = "cf::CVS_THREAD_REVIVE";

u32 lbl_eu_80539C98[3] = {
    0,
    0xFFFFFFFF,
    (u32)ReviveVxPlaySlot1,
};

u32 lbl_eu_80539CA4[3] = {
    0,
    0xFFFFFFFF,
    (u32)ReviveVxPlaySlot2,
};

u32 lbl_eu_80539CB0[3] = {
    0,
    0xFFFFFFFF,
    (u32)ReviveVxOnComplete,
};

u32 lbl_eu_80539CBC[7] = {
    (u32)lbl_eu_80662D28,
    0,
    (u32)PlayThreadVoice,
    (u32)ReviveVxRemoveVoice,
    (u32)blank1__17CVS_THREAD_REVIVEFv,
    (u32)CCharVoiceMan_AlwaysTrue,
    (u32)CCharVoiceMan_NullVoiceDispatch,
};

u32 lbl_eu_80539CD8[4] = {
    (u32)lbl_eu_80662CC0,
    0,
    0,
    0,
};

__declspec(section ".sdata") extern u32 lbl_eu_80662D28[2] = {
    (u32)lbl_eu_805107E8,
    (u32)lbl_eu_80539CD8,
};

// us-802ab1fc (ReviveVxInitVoice6E)
// Voice-ID init helper: if the handle is active-gated, allocate a 0x6E-byte
// handle buffer, then play voice (arg + 0x1005) via RequestVoicePlayback.
extern "C" int ReviveVxInitVoice6E(CVoiceHandle* self, int arg) {
    // Handle must have its 0x3F00 field's bit 1 set
    if (!(self->field_0x3F00 & 2)) return 0;

    // Handle must not be active (virtual dispatch -> r12 chain)
    if (((CVoiceHandleVTV*)self)->isActive() != 0) return 0;

    // Allocate voice-handle buffer (0x6E bytes)
    if (CCharVoiceMan_DispatchById(0x6E, 1) == NULL) return 0;

    CCharVoice* voicePtr = (CCharVoice*)self;
    if (self != NULL) {
        voicePtr = &self->voice;
    }
    RequestVoicePlayback(voicePtr, arg + 0x1005, 0x6E);
    return 0;
}

// us-802ab2a0 (ReviveVxInitVoice3C)
// Voice-ID init helper: if the handle is active-gated, allocate a 0x3C-byte
// handle buffer, then play voice 0x100E via RequestVoicePlayback.
extern "C" int ReviveVxInitVoice3C(CVoiceHandle* self) {
    // Handle must have its 0x3F00 field's bit 1 set
    if (!(self->field_0x3F00 & 2)) return 0;

    // Handle must not be active (virtual dispatch -> r12 chain)
    if (((CVoiceHandleVTV*)self)->isActive() != 0) return 0;

    // Allocate voice-handle buffer (0x3C bytes)
    if (CCharVoiceMan_DispatchById(0x3C, 1) == NULL) return 0;

    CCharVoice* voicePtr = (CCharVoice*)self;
    if (self != NULL) {
        voicePtr = &self->voice;
    }
    RequestVoicePlayback(voicePtr, 0x100E, 0x3C);
    return 0;
}