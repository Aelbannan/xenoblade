// CVS_THREAD_SUDDEN: voice thread for "sudden" audio events.
// Four functions: busy-check clear, voice removal, voice play, and constructor.

#include "kyoshin/cf/voice/cvsys/CVS_THREAD_SUDDEN.hpp"

// us-802ab338 (__ct__802A8C04)
// Factory for CVS_THREAD_SUDDEN. Allocates the handle buffer (1, 0 - the
// handle is discarded), then the 0x24-byte thread object, constructs the
// base, sets vtable/owner fields and copies init data from lbl_eu_80539CE8.
CVS_THREAD_SUDDEN* CVS_THREAD_SUDDEN::create() {
    // Allocate the (effectively discarded) handle buffer, then the 0x24-byte
    // thread object. Allocation failures return NULL directly (bypassing the
    // construction try/catch cleanup, matching retail's return-NULL jumps).
    CVoiceHandle* handleBuf = CCharVoiceMan_DispatchById(1, 0);
    if (handleBuf == NULL) {
        return NULL;
    }
    CVS_THREAD_SUDDEN* self = (CVS_THREAD_SUDDEN*)CCharVoiceMan_AllocVoiceArena(0x24);
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

    // Install the initial voice-event handler as a real member-function
    // pointer; MWCC emits the {0, -1, handler} constant itself.
    self->mState = static_cast<void (CVS_THREAD::*)()>(
        &CVS_THREAD_SUDDEN::clearIdleSlot);

    return self;
}

// us-802ab400 (SuddenVxClearIdleSlot)
// If no active voice, clear the handle slot.
void CVS_THREAD_SUDDEN::clearIdleSlot() {
    if (IsThreadVoiceActive(this) == 0) {
        voiceHandle = NULL;
    }
}

// us-802ab43c (SuddenVxRemoveVoice)
// Remove a voice by matching its embedded CCharVoice pointer against the
// slot (voiceHandle). A CVoiceHandle stores the CCharVoice at offset 0x3E9C,
// so a non-null handle is biased by 0x3E9C before comparing.
void SuddenVxRemoveVoice(CVS_THREAD_SUDDEN* self, CCharVoice* voicePtr) {
    DetachThreadVoice(self, voicePtr);

    CVoiceHandle* handle = self->voiceHandle;
    CCharVoice* embedded = (CCharVoice*)handle;
    if (handle != NULL) {
        embedded = &handle->voice;
    }
    if (embedded == voicePtr) {
        self->voiceHandle = NULL;
    }
}

// us-802ab494 (SuddenVxPlayVoice)
// Prepare/play a voice. Calls the completion check (and the playback-start
// helper if busy), stores the inverse-bias of voicePtr (back to the owning
// CVoiceHandle) into the slot, re-biases to the CCharVoice, then plays.
void SuddenVxPlayVoice(CVS_THREAD_SUDDEN* self, CCharVoice* voicePtr, int voiceId) {
    if (IsThreadVoiceActive(self) != 0) {
        StopThreadVoice(self);
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
    TryRequestVoice(self, vp, voiceId);
}

// Virtual override of blank1() (retail func_802A8DE0): returns 1 for SUDDEN
// thread (minimal buffer flag).
int CVS_THREAD_SUDDEN::SuddenVx_GetBufferSizeValue() { return 1; }

// Retail section reconstruction (measured in
// .scratch/dlabels/a2/CVS_THREAD_SUDDEN.txt): the .rodata typeinfo name,
// the .data init-state triple ({0, -1, callback}), the 7-word thread
// vtable ({typeinfo, 0, play, remove, bufsize, alwaysTrue, playVoice}), the
// trailing typeinfo-base word plus padding, and the .sdata typeinfo object
// ({name, trailing}). Global-scope data symbols: MWCC does not mangle them.
//
// NOTE: retail spells the bufsize vtable slot
// SuddenVx_GetBufferSizeValue__17CVS_THREAD_SUDDENFv (18-char class name);
// decomp's class is 17 chars, so the slot references decomp's own __17
// spelling (same .text offset 0x1DC, same linked bytes). The literal-name
// extern "C" decl below addresses that in-TU definition.
extern "C" void PlayThreadVoice();
extern "C" bool CCharVoiceMan_AlwaysTrue();
extern "C" int CCharVoiceMan_NullVoiceDispatch(void* self);
extern "C" int SuddenVx_GetBufferSizeValue__17CVS_THREAD_SUDDENFv();
extern u32 __RTTI__Q22cf10CVS_THREAD[2];
extern const u32 __RTTI__Q22cf17CVS_THREAD_SUDDEN[2];

u32 lbl_eu_80539CF4[7] = {
    (u32)__RTTI__Q22cf17CVS_THREAD_SUDDEN,
    0,
    (u32)PlayThreadVoice,
    (u32)SuddenVxRemoveVoice,
    (u32)SuddenVx_GetBufferSizeValue__17CVS_THREAD_SUDDENFv,
    (u32)CCharVoiceMan_AlwaysTrue,
    (u32)SuddenVxPlayVoice,
};

u32 lbl_eu_80539D10[4] = {
    (u32)__RTTI__Q22cf10CVS_THREAD,
    0,
    0,
    0,
};

extern const u32 __RTTI__Q22cf17CVS_THREAD_SUDDEN[2] = {
    (u32)"cf::CVS_THREAD_SUDDEN",
    (u32)lbl_eu_80539D10,
};