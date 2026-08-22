// CVS_THREAD_REVIVE: Voice thread variant for revival/healing audio.
// Seven functions: factory ctor, play/init, completion callback, voice
// removal, and buffer-size getter.

#include "kyoshin/cf/voice/cvsys/CVS_THREAD_REVIVE.hpp"
#include "kyoshin/harness_catalog.hpp"
#include "monolib/math/Random.hpp"

// ── Target 6: us-802aae00 (__ct__802A86CC) ────────────────────────────────
// Factory/constructor for CVS_THREAD_REVIVE. Takes two owner objects (with a
// field at offset 0x3F00 that must have bit 1 set), allocates a voice-handle
// buffer (0xE6 bytes, discarded) and the object itself (0x28 bytes), calls
// the base constructor, sets vtable/owner fields, and copies init data from
// lbl_eu_80539C98.
CVS_THREAD_REVIVE* __ct__802A86CC(CVoiceHandle* owner1, CVoiceHandle* owner2) {
    // Both owners must have their 0x3F00 field's bit 1 set
    if (!(owner1->field_0x3F00 & 2)) return NULL;
    if (!(owner2->field_0x3F00 & 2)) return NULL;

    // Allocate voice-handle buffer (0xE6 bytes, discarded)
    if (func_802A330C(0xE6, 1) == NULL) return NULL;

    // Allocate the actual CVS_THREAD_REVIVE object
    CVS_THREAD_REVIVE* self = (CVS_THREAD_REVIVE*)func_802A34E4(0x28);
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

    // Copy the init-state triple. The address is forced through an integer
    // cast so the full base (lis+addi) is materialized once before any load.
    u32 v1;
    u32 v0;
    u32* src = (u32*)(u32)lbl_eu_80539C98;
    v0 = src[0];
    v1 = src[1];
    raw->state0 = (u32*)v0;
    raw->state1 = v1;
    raw->state2 = src[2];

    return self;
}

// ── Target 7: us-802aaf08 (func_802A87D4) ─────────────────────────────────
// Advance/play function. Copies init data from lbl_eu_80539CA4, then if both
// voice slots are populated and slot 1 is inactive, plays a state-based voice
// (slot-2 iterator + 0x204) or a fallback voice (0x21E) depending on iterator
// validity, slot conflict, and a random gate.
void func_802A87D4(CVS_THREAD_REVIVE* self) {
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
    int iter = func_802A77E8(self->field_0x24);

    if (func_802A7850(iter) != 0 &&
        func_802A7B90(self->field_0x20, self->field_0x24) == 0 &&
        ml::math::mtRand(2) != 0) {
        // Valid state -- play the state-specific voice (iter + 0x204)
        CVoiceHandle* tmpHandle = self->field_0x20;
        CCharVoice* voicePtr = (CCharVoice*)tmpHandle;
        if (tmpHandle != NULL) {
            voicePtr = &tmpHandle->voice;
        }
        if (func_802A3C44(self, voicePtr, iter + 0x204) == 0) {
            self->func_802A3B50();
        }
    } else {
        // Fallback voice 0x21E
        CVoiceHandle* tmpHandle = self->field_0x20;
        CCharVoice* voicePtr = (CCharVoice*)tmpHandle;
        if (tmpHandle != NULL) {
            voicePtr = &tmpHandle->voice;
        }
        if (func_802A3C44(self, voicePtr, 0x21E) == 0) {
            self->func_802A3B50();
        }
    }
}

// ── Target 5: us-802ab038 (func_802A8904) ─────────────────────────────────
// Advance/play function for voice slot 2 (field_0x24). Copies init data from
// lbl_eu_80539CB0, checks the slot-2 voice state (vtable method at 0x308);
// if >= 3 plays 0x711, otherwise a random voice (0x714 when mtRand==0, else
// 0x710).
void func_802A8904(CVS_THREAD_REVIVE* self) {
    if (func_802A3E88(self) != 0) {
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
            if (func_802A3C44(self, voicePtr, 0x711) == 0) {
                self->func_802A3B50();
            }
        } else {
            // Low state -- play weighted random voice
            int voiceId = ml::math::mtRand(2) != 0 ? 0x710 : 0x714;
            CVoiceHandle* tmpHandle = self->field_0x24;
            CCharVoice* voicePtr = (CCharVoice*)tmpHandle;
            if (tmpHandle != NULL) {
                voicePtr = &tmpHandle->voice;
            }
            if (func_802A3C44(self, voicePtr, voiceId) == 0) {
                self->func_802A3B50();
            }
        }
    }
}

// ── Target 1: us-802ab134 (func_802A8A00) ──────────────────────────────────
// Completion callback: if no active voice, call the playback-start virtual.
void func_802A8A00(CVS_THREAD_REVIVE* self) {
    if (func_802A3E88(self) == 0) {
        self->func_802A3B50();
    }
}

// ── Target 2: us-802ab17c (func_802A8A48) ──────────────────────────────────
// Remove a voice from the slots by matching its embedded CCharVoice pointer.
void func_802A8A48(CVS_THREAD_REVIVE* self, CCharVoice* voicePtr) {
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

// Virtual method override: returns the buffer size for this thread type.
// Matches CVS_THREAD::blank1 slot in vtable; REVIVE subclass returns 0xE6 (230).
int CVS_THREAD_REVIVE::blank1() {
    return BUFFER_SIZE;
}

// ── Target 4: us-802ab1fc (func_802A8AC8) ──────────────────────────────────
// Voice-ID init helper: if the handle is active-gated, allocate a 0x6E-byte
// handle buffer, then play voice (arg + 0x1005) via func_802A3D54.
int func_802A8AC8(CVoiceHandle* self, int arg) {
    // Handle must have its 0x3F00 field's bit 1 set
    if (!(self->field_0x3F00 & 2)) return 0;

    // Handle must not be active (virtual dispatch -> r12 chain)
    if (((CVoiceHandleVTV*)self)->isActive() != 0) return 0;

    // Allocate voice-handle buffer (0x6E bytes)
    if (func_802A330C(0x6E, 1) == NULL) return 0;

    CCharVoice* voicePtr = (CCharVoice*)self;
    if (self != NULL) {
        voicePtr = &self->voice;
    }
    func_802A3D54(voicePtr, arg + 0x1005, 0x6E);
    return 0;
}

// ── Target 3: us-802ab2a0 (func_802A8B6C) ──────────────────────────────────
// Voice-ID init helper: if the handle is active-gated, allocate a 0x3C-byte
// handle buffer, then play voice 0x100E via func_802A3D54.
int func_802A8B6C(CVoiceHandle* self) {
    // Handle must have its 0x3F00 field's bit 1 set
    if (!(self->field_0x3F00 & 2)) return 0;

    // Handle must not be active (virtual dispatch -> r12 chain)
    if (((CVoiceHandleVTV*)self)->isActive() != 0) return 0;

    // Allocate voice-handle buffer (0x3C bytes)
    if (func_802A330C(0x3C, 1) == NULL) return 0;

    CCharVoice* voicePtr = (CCharVoice*)self;
    if (self != NULL) {
        voicePtr = &self->voice;
    }
    func_802A3D54(voicePtr, 0x100E, 0x3C);
    return 0;
}