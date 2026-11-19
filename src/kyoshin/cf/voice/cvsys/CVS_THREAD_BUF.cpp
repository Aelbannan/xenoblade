// CVS_THREAD_BUF: Voice thread buffer management.
// FULL_MATCH: BufVxAllocSize -- buffer-size getter (virtual method override).
// Remaining functions are NOT_STARTED harness stubs.

#include "kyoshin/cf/voice/cvsys/CVS_THREAD_BUF.hpp"
#include "kyoshin/harness_catalog.hpp"

void CVS_THREAD_BUF::create() {}

void BufVxFillSchedDesc(){}

// us-802a76d8 (CVS_THREAD_BUF::advanceSlot)
// Advance the current voice slot: restore the base state triple, then if the
// current slot's voice is still strongly active pull the next line; otherwise
// (no handle / active / play rejected) restart via the playback virtual.
void CVS_THREAD_BUF::advanceSlot() {
    CVS_THREAD_BUF* self = this;
    mState = lbl_eu_805399EC;

    CVoiceHandle* handle = self->slotHandles[self->field_0x3c];
    if (handle == NULL) {
        goto restart;
    }
    // If the voice is still strongly active, do not push another line.
    if (((CVoiceVTV*)handle)->idle() != 0) {
        goto restart;
    }

    // Voice not active -- try to play the slot's recorded voice ID.
    handle = self->slotHandles[self->field_0x3c];
    CCharVoice* vp = (CCharVoice*)handle;
    if (handle != NULL) {
        vp = &handle->voice;
    }
    if (TryRequestVoice(self, vp, self->slotIds[self->field_0x3c]) == 0) {
        goto restart;
    }
    return;

restart:
    self->PlayThreadVoice();
}

// us-802a7794 (CVS_THREAD_BUF::advanceIndex)
// Advance the slot index; when it passes the slot limit, restart the thread,
// otherwise re-copy the init-state triple into the base fields.
void CVS_THREAD_BUF::advanceIndex() {
    CVS_THREAD_BUF* self = this;
    if (IsThreadVoiceActive(self) != 0) {
        return;
    }

    int idx = self->field_0x3c + 1;
    self->field_0x3c = idx;
    if (idx < self->field_0x38) {
        mState = lbl_eu_805399F8;
    } else {
        self->PlayThreadVoice();
    }
}

// us-802a7814 (BufVxRemoveVoice)
// Remove a released voice from the three slots: the base handler runs first,
// then each slot whose biased embedded CCharVoice matches is cleared. Written
// unrolled -- retail shows three independent check/store sequences.
void BufVxRemoveVoice(CVS_THREAD_BUF* self, CCharVoice* voicePtr) {
    DetachThreadVoice(self, voicePtr);

    for (int i = 0; i < 3; i++) {
        CVoiceHandle* h = self->slotHandles[i];
        if (h != NULL && &h->voice == voicePtr) {
            self->slotHandles[i] = NULL;
        }
    }
}

// Virtual method override: returns the buffer size for this thread type.
// Matches CVS_THREAD::blank1 slot in vtable; BUF subclass returns 0x104 (260).
int CVS_THREAD_BUF::blank1() {
    return BUFFER_SIZE;
}

// us-802a7f5c (BufVxAllocSize): retail C-ABI buffer-size getter (0x8).
// Plain function returning BUFFER_SIZE; mirrors EndVoice_AllocSize pattern
// in the sibling BATTLE_END TU (plain C function, not the virtual).
extern "C" int BufVxAllocSize() {
    return 0x104;
}
