// CVS_THREAD_DOWN: Voice thread for "Down" (knocked-down) status effect.
// Five functions: completion callback, voice removal, slot-1 play, slot-2 play, constructor.


#include "kyoshin/cf/voice/cvsys/CVS_THREAD_DOWN.hpp"
#include "kyoshin/harness_catalog.hpp"
#include "monolib/math/Random.hpp"

// ── Target 1: us-802a8540 (func_802A5E0C) ──────────────────────────────────
// Completion callback: if no active voice, call the playback-start virtual.
void func_802A5E0C(CVS_THREAD_DOWN* self) {
    if (func_802A3E88(self) == 0) {
        self->func_802A3B50();
    }
}

// ── Target 2: us-802a8588 (func_802A5E54) ──────────────────────────────────
// Remove a voice from the slots by matching its embedded CCharVoice pointer.
void func_802A5E54(CVS_THREAD_DOWN* self, CCharVoice* voicePtr) {
    func_802A3BEC(self, voicePtr);

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

// ── Target 3: us-802a83c4 (func_802A5C90) ──────────────────────────────────
// Advance/play function for voice slot 1 (field_0x20).
// Copies init data from lbl_eu_80539A74 to fields 0x00-0x08, checks if
// the voice is still active (vtable method at offset 0x2BC), and if
// inactive, plays a random voice ID (mtRand(2) + 0x70C).
void func_802A5C90(CVS_THREAD_DOWN* self) {
    u32 v0;
    u32* src = lbl_eu_80539A74;
    v0 = *src++;
    CVoiceHandle* handle20 = self->field_0x20;
    self->unk4 = *src++;
    self->unk0 = (u32*)v0;
    self->unk8 = *src;

    if (handle20 != NULL) {
        // Call vtable method at offset 0x2BC (is-active check) as a real
        // virtual so MWCC emits the r12 dispatch retail shows.
        if (((CVS_THREAD_DOWN_Vtbl*)handle20)->isVoiceActive() == 0) {
            // Voice is not active -- try to play a random voice
            handle20 = self->field_0x20;
            CCharVoice* voicePtr = (CCharVoice*)handle20;
            if (handle20 != NULL) {
                voicePtr = &handle20->voice;
            }
            if (func_802A3C44(self, voicePtr, ml::math::mtRand(2) + 0x70C) == 0) {
                self->func_802A3B50();
            }
        }
    }
}

// ── Target 4: us-802a8480 (func_802A5D4C) ──────────────────────────────────
// Advance/play function for voice slot 2 (field_0x24).
// Copies init data from lbl_eu_80539A80 to fields 0x00-0x08, checks if
// the voice is still active, and if inactive, plays a voice ID chosen by
// mtRand(2): 0x713 when rand==0, otherwise 0x70E.
void func_802A5D4C(CVS_THREAD_DOWN* self) {
    if (func_802A3E88(self) != 0) {
        return;
    }

    u32 v0;
    u32* src = lbl_eu_80539A80;
    v0 = *src++;
    CVoiceHandle* handle24 = self->field_0x24;
    self->unk4 = *src++;
    self->unk0 = (u32*)v0;
    self->unk8 = *src;

    if (handle24 != NULL) {
        // Call vtable method at offset 0x2BC (is-active check) as a real
        // virtual so MWCC emits the r12 dispatch retail shows.
        if (((CVS_THREAD_DOWN_Vtbl*)handle24)->isVoiceActive() == 0) {
            // Voice is not active -- play a weighted random voice.
            // Retail computes the random voice ID first, then reloads the
            // slot handle so it is NOT live across the mtRand call.
            int voiceId = (ml::math::mtRand(2) != 0) ? 0x70E : 0x713;
            handle24 = self->field_0x24;
            CCharVoice* voicePtr = (CCharVoice*)handle24;
            if (handle24 != NULL) {
                voicePtr = &handle24->voice;
            }
            if (func_802A3C44(self, voicePtr, voiceId) == 0) {
                self->func_802A3B50();
            }
        }
    }
}

// ── Target 5: us-802a82bc (__ct__802A5B88) ──────────────────────────────────
// Factory/constructor for CVS_THREAD_DOWN. Takes two owner CVoiceHandles
// (each must have the factory-active flag bit 1 set at 0x3F00), allocates a
// throwaway handle buffer (0xF0 bytes) and the object itself (0x28 bytes),
// runs the base constructor, overrides the vtable and owner slots, then
// copies the init-state triple from lbl_eu_80539A68.
CVS_THREAD_DOWN* __ct__802A5B88(CVoiceHandle* owner1, CVoiceHandle* owner2) {
    // Both owners must have their 0x3F00 field's bit 1 set.
    if (!(owner1->field_0x3F00 & 2)) return NULL;
    if (!(owner2->field_0x3F00 & 2)) return NULL;

    // Allocate the (discarded) handle buffer, then the thread object itself.
    if (func_802A330C(0xF0, 1) == NULL) return NULL;

    CVS_THREAD_DOWN* self = (CVS_THREAD_DOWN*)func_802A34E4(0x28);
    if (self == NULL) return NULL;

    // Base-construct the object. The redundant `self != NULL` guard
    // reproduces retail's `beq` re-check of CR0 guarding the EH region; the
    // local try/catch reproduces retail's CATCHBLOCK extab action. The catch
    // rethrows via the runtime __throw(0,0,0) (retail `li r3,0; li r4,0;
    // li r5,0; bl __throw`) so MWCC elides the __end__catch epilogue.
    if (self != NULL) {
        try {
            __ct__cf_CVS_THREAD(self);

            // Override the vtable at 0x1C with the DOWN vtable, then the slots.
            ((CVS_THREAD_DOWN_raw*)self)->vtable = (void*)lbl_eu_80539A8C;
            self->field_0x20 = owner1;
            self->field_0x24 = owner2;
        } catch (...) {
            __throw(0, 0, 0);
        }
    }

    // Copy the init-state triple. The address is forced through an integer
    // cast so the full base (lis+addi) is materialized once before any load.
    u32 v0;
    u32 v1;
    u32* src = (u32*)(u32)lbl_eu_80539A68;
    v1 = src[1];
    v0 = src[0];
    self->unk0 = (u32*)v0;
    self->unk4 = v1;
    self->unk8 = src[2];

    return self;
}