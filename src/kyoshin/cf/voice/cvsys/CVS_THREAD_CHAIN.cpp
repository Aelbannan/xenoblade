// CVS_THREAD_CHAIN: voice thread chain node.
// Six functions: busy-check clear, voice removal, voice play, chain actor
// voice trigger, factory constructor, and chain voice lifetime handler.

#include "kyoshin/cf/voice/cvsys/CVS_THREAD_CHAIN.hpp"
#include "kyoshin/harness_catalog.hpp"
#include "monolib/math/Random.hpp"

// ── Target 1: us-802a802c (func_802A58F8) ──────────────────────────────────
// If the thread reports no active voice, clear the voice handle slot.
void func_802A58F8(cf::CVS_THREAD_CHAIN* self) {
    if (func_802A3E88(self) == 0) {
        self->field_0x20 = NULL;
    }
}

// ── Target 2: us-802a8068 (func_802A5934) ──────────────────────────────────
// Remove a voice by matching its embedded CCharVoice against the slot.
// A CVoiceHandle stores the CCharVoice at offset 0x3E9C, so a non-null handle
// is biased by 0x3E9C before comparing against the incoming voice pointer.
void func_802A5934(cf::CVS_THREAD_CHAIN* self, CCharVoice* voicePtr) {
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

// ── Target 3: us-802a80c0 (func_802A598C) ──────────────────────────────────
// Prepare/play a voice: run the completion check (and playback-start helper
// if busy), store the inverse-biased handle into the slot, re-bias to the
// embedded CCharVoice and play the given voice ID.
int func_802A598C(cf::CVS_THREAD_CHAIN* self, CCharVoice* voicePtr, int voiceId) {
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
    return func_802A3C44(self, vp, voiceId);
}

// ── Target 4: us-802a8238 (func_802A5B04) ──────────────────────────────────
// Chain actor voice trigger. The actor handle must have its factory-active
// flag (bit 1 of the 0x3F00 manager word) set; a thread buffer (0xA) is
// reserved, then a random chain voice (mtRand(2) + 0x321) is played. Always
// returns 0.
int func_802A5B04(CVoiceHandle* self, int flag) {
    if ((self->field_0x3F00 & 2) == 0) {
        return 0;
    }
    if (func_802A330C(0xA, 1) == NULL) {
        return 0;
    }
    CCharVoice* biased = (CCharVoice*)self;
    if (self != NULL) {
        biased = &self->voice;
    }
    func_802A3D54(biased, ml::math::mtRand(2) + 0x321, 0xA);
    return 0;
}

// ── Target 5: us-802a7f64 (__ct__802A5830) ─────────────────────────────────
// Factory for CVS_THREAD_CHAIN. Allocates the handle buffer ((1,0) - the
// handle is discarded), then the 0x24-byte thread object, constructs the
// base, sets vtable/owner fields and copies init data from lbl_eu_80539A30.
cf::CVS_THREAD_CHAIN* __ct__802A5830() {
    CVoiceHandle* handleBuf = func_802A330C(1, 0);
    if (handleBuf == NULL) {
        return NULL;
    }
    cf::CVS_THREAD_CHAIN* self = (cf::CVS_THREAD_CHAIN*)func_802A34E4(0x24);
    if (self == NULL) {
        return NULL;
    }

    // Construct the base (can throw -> EH guard), then set vtable + link slot.
    if (self != NULL) {
        try {
            __ct__cf_CVS_THREAD();

            // Set the vtable at offset 0x1C (right after the 7 CVS_THREAD base words).
            ((void**)self)[7] = (void**)lbl_eu_80539A3C;
            self->field_0x20 = NULL;
        } catch (...) {
            throw;
        }
    }

    // Copy the init-state triple into the first 3 u32s (outside try).
    const u32* base = lbl_eu_80539A30;
    self->unk0 = (u32*)base[0];
    self->unk4 = base[1];
    self->unk8 = base[2];

    return self;
}

// ── Target 6: us-802a8148 (func_802A5A14) ──────────────────────────────────
// Chain voice lifetime handler. Validates the factory-active flag, that the
// voice is not already active (vtable[0x2BC/4]), and that the previous sound
// is not the chain-stop marker; then reserves a 0x12C buffer and plays either
// a random chain voice (flag==0) or a fixed chain voice (flag!=0). Returns 0.
int func_802A5A14(CVoiceHandle* self, int flag) {
    if ((self->field_0x3F00 & 2) == 0) {
        return 0;
    }
    if (((int (*)(CVoiceHandle*))self->vtable[0x2BC / 4])(self) != 0) {
        return 0;
    }
    if (func_800BE8F4(&self->voice) == 0x12C) {
        return 0;
    }
    if (func_802A330C(0x12C, 1) == NULL) {
        return 0;
    }
    if (flag == 0) {
        CCharVoice* biased = (CCharVoice*)self;
        if (self != NULL) {
            biased = &self->voice;
        }
        func_802A3D54(biased, ml::math::mtRand(2) + 0x2BD, 0x12C);
    } else {
        CCharVoice* biased = (CCharVoice*)self;
        if (self != NULL) {
            biased = &self->voice;
        }
        func_802A3D54(biased, 0x2BF, 0x12C);
    }
    return 0;
}