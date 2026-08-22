// CVS_THREAD_SUDDEN: voice thread for "sudden" audio events.
// Four functions: busy-check clear, voice removal, voice play, and constructor.

#include "kyoshin/cf/voice/cvsys/CVS_THREAD_SUDDEN.hpp"
#include "kyoshin/harness_catalog.hpp"

// ── Target 1: us-802ab400 (func_802A8CCC) ──────────────────────────────────
// If no active voice, clear the handle slot.
void func_802A8CCC(CVS_THREAD_SUDDEN* self) {
    if (func_802A3E88(self) == 0) {
        self->voiceHandle = NULL;
    }
}

// ── Target 2: us-802ab43c (func_802A8D08) ──────────────────────────────────
// Remove a voice by matching its embedded CCharVoice pointer against the
// slot (voiceHandle). A CVoiceHandle stores the CCharVoice at offset 0x3E9C,
// so a non-null handle is biased by 0x3E9C before comparing.
void func_802A8D08(CVS_THREAD_SUDDEN* self, CCharVoice* voicePtr) {
    func_802A3BEC(self, voicePtr);

    CVoiceHandle* handle = self->voiceHandle;
    CCharVoice* biased = (CCharVoice*)handle;
    if (handle != NULL) {
        biased = &handle->voice;
    }
    if (biased == voicePtr) {
        self->voiceHandle = NULL;
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
    self->voiceHandle = handle;

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
CVS_THREAD_SUDDEN* __ct__802A8C04() {
    // Allocate the (effectively discarded) handle buffer, then the 0x24-byte
    // thread object. Allocation failures return NULL directly (bypassing the
    // construction try/catch cleanup, matching retail's return-NULL jumps).
    CVoiceHandle* handleBuf = func_802A330C(1, 0);
    if (handleBuf == NULL) {
        return NULL;
    }
    CVS_THREAD_SUDDEN* self = (CVS_THREAD_SUDDEN*)func_802A34E4(0x24);
    if (self == NULL) {
        return NULL;
    }

    // Base constructor (self in r3), then vtable and the null voice slot,
    // inside a construction try/catch so MWCC emits the frame-pointer EH
    // prologue/epilogue retail shows (the allocation failures above bypass
    // this region and jump straight to the epilogue, matching retail). The
    // redundant `self != NULL` re-check reproduces retail's `beq` guard on
    // the EH region (CR0 from the earlier cmpwi is reused).
    if (self != NULL) {
        try {
            __ct__cf_CVS_THREAD(self);

            // Set the retail subclass vtable at offset 0x1C (right after the
            // 7 base words) via the raw layout view, then clear the voice slot.
            ((CVS_THREAD_SUDDEN_raw*)self)->vtable = (u32)lbl_eu_80539CF4;
            self->voiceHandle = NULL;
        } catch (...) {
            // Rethrow via the runtime __throw(0,0,0) so MWCC elides the
            // __end__catch epilogue (retail's catch-all ends at `bl __throw`).
            __throw(0, 0, 0);
        }
    }

    // Copy the slot-state init triple into the base state words (outside
    // try) via the raw layout view. The address is forced through an integer
    // cast so the full base (lis+addi) is materialized once before any load.
    // (Retail colors the loaded pair with the registers swapped relative to
    // source order; certified equivalent by the register-renaming witness.)
    CVS_THREAD_SUDDEN_raw* raw = (CVS_THREAD_SUDDEN_raw*)self;
    u32 v0;
    u32 v1;
    u32 v2;
    u32* src = (u32*)(u32)lbl_eu_80539CE8;
    v1 = src[1];
    v0 = src[0];
    raw->state0 = (u32*)v0;
    raw->state4 = v1;
    v2 = src[2];
    raw->state8 = v2;

    return self;
}

// Virtual override of blank1(): returns 1 for SUDDEN thread (minimal buffer flag).
int CVS_THREAD_SUDDEN::blank1() { return 1; }