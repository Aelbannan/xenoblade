// CVS_THREAD_FAINT: Voice thread for the "faint" status effect.
// FULL_MATCH: func_802A6DEC -- buffer-size getter (virtual method override).
// Remaining six functions were NOT_STARTED stubs; decompiled below.

#include "kyoshin/cf/voice/cvsys/CVS_THREAD_FAINT.hpp"
#include "kyoshin/cf/voice/cvsys/CVS_THREAD_CHAIN.hpp"
#include "monolib/math/Random.hpp"

// ── Target 1: us-802a9460 (func_802A6D2C) ──────────────────────────────────
// Completion callback: if no active voice, call the playback-start virtual.
void func_802A6D2C(CVS_THREAD_FAINT* self) {
    if (func_802A3E88(self) == 0) {
        self->func_802A3B50();
    }
}

// ── Target 2: us-802a94a8 (func_802A6D74) ──────────────────────────────────
// Remove a voice from the slots by matching its embedded CCharVoice pointer.
void func_802A6D74(CVS_THREAD_FAINT* self, CCharVoice* voicePtr) {
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

// ── Target 3: us-802a9528 (func_802A6DF4) ──────────────────────────────────
// Owner-level voice play: requires factory flags set, allocates a throwaway
// handle, bails if the owner voice is already the current one (0xA), and
// otherwise plays the 0x2BF line.
int func_802A6DF4(CVoiceHandle* self) {
    if (!(self->field_0x3F00 & 2)) return 0;
    if (func_802A330C(0xa, 1) == NULL) return 0;
    if (func_800BE8F4(&self->voice) == 0xa) return 0;

    CCharVoice* voice = (CCharVoice*)self;
    if (self != NULL) {
        voice = &self->voice;
    }
    func_802A3D54(voice, 0x2bf, 0xa);
    return 0;
}

// ── Target 4: us-802a92e4 (func_802A6BB0) ──────────────────────────────────
// Advance/play function for voice slot 1 (field_0x20).
// Copies init data from lbl_eu_80539B64 to fields 0x00-0x08, checks if
// the voice is still active (vtable method at offset 0x2BC), and if
// inactive, plays a random voice ID (mtRand(2) + 0x709).
void func_802A6BB0(CVS_THREAD_FAINT* self) {
    // Restore the base state triple via the lwzu/spread load-with-update
    // pattern (v0 declared first so the lwzu destination colours low).
    u32 v0;
    const u32* src = lbl_eu_80539B64;
    v0 = *src++;
    self->unk4 = *src++;
    self->unk0 = (u32*)v0;
    self->unk8 = *src;

    CVoiceHandle* handle20 = self->field_0x20;
    if (handle20 != NULL) {
        // Call vtable method at offset 0x2BC (is-active check) via the
        // phantom vtable view so MWCC emits the r12-chained virtual call.
        if (((CVoiceChainVTV*)handle20)->isActive() == 0) {
            // Voice is not active -- try to play a random voice
            handle20 = self->field_0x20;
            CCharVoice* voicePtr = (CCharVoice*)handle20;
            if (handle20 != NULL) {
                voicePtr = &handle20->voice;
            }
            if (func_802A3C44(self, voicePtr, ml::math::mtRand(2) + 0x709) == 0) {
                self->func_802A3B50();
            }
        }
    }
}

// ── Target 5: us-802a93a0 (func_802A6C6C) ──────────────────────────────────
// Advance/play function for voice slot 2 (field_0x24).
// Copies init data from lbl_eu_80539B70 to fields 0x00-0x08, checks if
// the voice is still active, and if inactive, plays a voice ID chosen by
// mtRand(2): 0x712 when rand==0, otherwise 0x70B.
void func_802A6C6C(CVS_THREAD_FAINT* self) {
    if (func_802A3E88(self) != 0) {
        return;
    }

    u32 v0;
    const u32* src = lbl_eu_80539B70;
    v0 = *src++;
    self->unk4 = *src++;
    self->unk0 = (u32*)v0;
    self->unk8 = *src;

    CVoiceHandle* handle24 = self->field_0x24;
    if (handle24 != NULL) {
        // Call vtable method at offset 0x2BC (is-active check) via the
        // phantom vtable view so MWCC emits the r12-chained virtual call.
        if (((CVoiceChainVTV*)handle24)->isActive() == 0) {
            // Voice is not active -- play a weighted random voice.
            // Retail computes the random voice ID first, then reloads the
            // slot handle so it is NOT live across the mtRand call.
            int voiceId = ml::math::mtRand(2) != 0 ? 0x70B : 0x712;
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

// ── Target 6: us-802a91dc (__ct__802A6AA8) ──────────────────────────────────
// Factory/constructor for CVS_THREAD_FAINT. Takes two owner objects (with a
// field at offset 0x3F00 that must have bit 1 set), allocates a handle
// (0xF0 bytes, discarded) and the object itself (0x28 bytes), calls the
// base constructor, sets vtable/owner fields, and copies init data from
// lbl_eu_80539B58.
CVS_THREAD_FAINT* __ct__802A6AA8(CVoiceHandle* owner1, CVoiceHandle* owner2) {
    // Both owners must have their 0x3F00 field's bit 1 set
    if (!(owner1->field_0x3F00 & 2)) return NULL;
    if (!(owner2->field_0x3F00 & 2)) return NULL;

    // Allocate handle buffer (0xF0 bytes, discarded)
    if (func_802A330C(0xF0, 1) == NULL) return NULL;

    // Allocate the actual CVS_THREAD_FAINT object
    CVS_THREAD_FAINT* self = (CVS_THREAD_FAINT*)func_802A34E4(0x28);
    if (self == NULL) return NULL;

    // Retail emits a redundant null re-check here (the `beq` at .L_802A9264)
    // guarding the constructor try-block; mirror it so the guard survives.
    // The catch rethrows via the runtime __throw(0,0,0) (retail `li r3,0;
    // li r4,0; li r5,0; bl __throw`), avoiding the __end__catch epilogue.
    if (self != NULL) {
        try {
            // Base constructor (self in r3), then vtable/owner fields.
            __ct__cf_CVS_THREAD();
            ((void**)self)[7] = (void**)lbl_eu_80539B7C;
            self->field_0x20 = owner1;
            self->field_0x24 = owner2;
        } catch (...) {
            __throw(0, 0, 0);
        }
    }

    // Copy the init-state triple into the first 3 u32s (outside try). Temps
    // are loaded before the stores; the address is forced through an integer
    // cast so the full base (lis+addi) is materialized once before any load.
    // (Residual: retail colors the unk4 value r0 and the unk0 value r4; MWCC
    // emits the reverse for every source shape tried - allocator fixed point,
    // cf. sibling factory drafts. The (u32) cast also bakes the literal into
    // the addi immediate, producing the LO layout reloc drift at +0xc6.)
    u32 v0;
    u32 v1;
    const u32* base = (const u32*)(u32)lbl_eu_80539B58;
    v1 = base[1];
    v0 = base[0];
    self->unk0 = (u32*)v0;
    self->unk4 = v1;
    self->unk8 = base[2];

    return self;
}

// Virtual method override: returns the buffer size for this thread type.
// Matches CVS_THREAD::blank1 slot in vtable; FAINT subclass returns 0xF0 (240).
int CVS_THREAD_FAINT::blank1() {
    return BUFFER_SIZE;
}