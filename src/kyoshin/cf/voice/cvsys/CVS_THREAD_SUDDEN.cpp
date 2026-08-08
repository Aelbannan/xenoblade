// CVS_THREAD_SUDDEN: voice thread for "sudden" audio events.
// Four functions: busy-check clear, voice removal, voice play, and constructor.

#include "kyoshin/cf/voice/cvsys/CVS_THREAD_SUDDEN.hpp"
#include "kyoshin/harness_catalog.hpp"

// ── Target 1: us-802ab400 (func_802A8CCC) ──────────────────────────────────
// If no active voice, clear the handle slot.
void func_802A8CCC(CVS_THREAD_SUDDEN* self) {
    if (func_802A3E88(self) == 0) {
        self->field_0x20 = NULL;
    }
}

// ── Target 2: us-802ab43c (func_802A8D08) ──────────────────────────────────
// Remove a voice by matching its embedded CCharVoice pointer against the
// slot (field_0x20). A CVoiceHandle stores the CCharVoice at offset 0x3E9C,
// so a non-null handle is biased by 0x3E9C before comparing.
void func_802A8D08(CVS_THREAD_SUDDEN* self, CCharVoice* voicePtr) {
    func_802A3BEC(self, voicePtr);

    CVoiceHandle* handle = self->field_0x20;
    CCharVoice* biased = (CCharVoice*)handle;
    if (handle != NULL) {
        biased = &handle->voice;
    }
    if (biased == voicePtr) {
        self->field_0x20 = NULL;
    }
}

// ── Target 3: us-802ab494 (func_802A8D60) ──────────────────────────────────
// Prepare/play a voice. Calls the completion check (and the playback-start
// helper if busy), stores the inverse-bias of voicePtr (back to the owning
// CVoiceHandle) into the slot, re-biases to the CCharVoice, then plays.
void func_802A8D60(CVS_THREAD_SUDDEN* self, CCharVoice* voicePtr, int voiceId) {
    if (func_802A3E88(self) != 0) {
        func_802A3E28(self);
    }

    // Convert CCharVoice* to the containing CVoiceHandle*.
    CVoiceHandle* handle = (CVoiceHandle*)voicePtr;
    if (voicePtr != NULL) {
        handle = (CVoiceHandle*)((char*)voicePtr - 0x3E9C);
    }
    self->field_0x20 = handle;

    // Convert back to the embedded CCharVoice for the play call.
    CCharVoice* vp = (CCharVoice*)handle;
    if (handle != NULL) {
        vp = &handle->voice;
    }
    func_802A3C44(self, vp, voiceId);
}

// ── Target 4: us-802ab338 (__ct__802A8C04) ─────────────────────────────────
// Factory for CVS_THREAD_SUDDEN. Allocates the handle buffer (1, 0 - the
// handle is discarded), then the 0x24-byte thread object, constructs the
// base, sets vtable/owner fields and copies init data from lbl_eu_80539CE8.
CVS_THREAD_SUDDEN* __ct__802A8C04() try {
    CVoiceHandle* handleBuf = func_802A330C(1, 0);
    if (handleBuf == NULL) {
        return NULL;
    }
    CVS_THREAD_SUDDEN* self = (CVS_THREAD_SUDDEN*)func_802A34E4(0x24);
    if (self == NULL) {
        return NULL;
    }

    // Base constructor (self in r3).
    __ct__cf_CVS_THREAD();

    // Set the vtable at offset 0x1C (right after the 7 CVS_THREAD base words).
    ((void**)self)[7] = (void**)lbl_eu_80539CF4;
    self->field_0x20 = NULL;

    // Copy the slot-state init data triple into the first 3 u32s.
    const u32* base = lbl_eu_80539CE8;
    self->unk0 = (u32*)base[0];
    self->unk4 = base[1];
    self->unk8 = base[2];

    return self;
} catch (...) {
    throw;
}

// Virtual override of blank1(): returns 1 for SUDDEN thread (minimal buffer flag).
int CVS_THREAD_SUDDEN::blank1() { return 1; }