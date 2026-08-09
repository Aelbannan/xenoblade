// CVS_THREAD_BUF: Voice thread buffer management.
// FULL_MATCH: func_802A5828 -- buffer-size getter (virtual method override).
// Remaining functions are NOT_STARTED harness stubs.

#include "kyoshin/cf/voice/cvsys/CVS_THREAD_BUF.hpp"
#include "kyoshin/harness_catalog.hpp"

void __ct__802A4E48(){}

void func_802A5174(){}

// ── Target 3: us-802a76d8 (func_802A4FA4) ─────────────────────────────────
// Advance the current voice slot: restore the base state triple, then if the
// current slot's voice is still strongly active pull the next line; otherwise
// (no handle / active / play rejected) restart via the playback virtual.
void func_802A4FA4(CVS_THREAD_BUF* self) {
    // Restore the base state triple via the lwzu/spread load-with-update
    // pattern (v0 declared first so the lwzu destination colours low).
    u32 v0;
    const u32* p = lbl_eu_805399EC;
    v0 = *p++;
    self->unk4 = *p++;
    self->unk0 = (u32*)v0;
    self->unk8 = *p;

    CVoiceHandle* handle = self->slotHandles[self->field_0x3c];
    if (handle == NULL) {
        goto restart;
    }
    // If the voice is still strongly active, do not push another line.
    if (((int (*)(CVoiceHandle*))handle->vtable[0x2BC / 4])(handle) != 0) {
        goto restart;
    }

    // Voice not active -- try to play the slot's recorded voice ID.
    handle = self->slotHandles[self->field_0x3c];
    CCharVoice* vp = (CCharVoice*)handle;
    if (handle != NULL) {
        vp = &handle->voice;
    }
    if (func_802A3C44(self, vp, self->slotIds[self->field_0x3c]) == 0) {
        goto restart;
    }
    return;

restart:
    self->func_802A3B50();
}

// ── Target 1: us-802a7794 (func_802A5060) ─────────────────────────────────
// Advance the slot index; when it passes the slot limit, restart the thread,
// otherwise re-copy the init-state triple into the base fields.
void func_802A5060(CVS_THREAD_BUF* self) {
    if (func_802A3E88(self) != 0) {
        return;
    }

    int idx = self->field_0x3c + 1;
    self->field_0x3c = idx;
    if (idx < self->field_0x38) {
        u32 v0;
        const u32* p = lbl_eu_805399F8;
        v0 = *p++;
        self->unk4 = *p++;
        self->unk0 = (u32*)v0;
        self->unk8 = *p;
    } else {
        self->func_802A3B50();
    }
}

// ── Target 2: us-802a7814 (func_802A50E0) ─────────────────────────────────
// Remove a released voice from the three slots by matching its embedded
// CCharVoice pointer against the one being freed.
void func_802A50E0(CVS_THREAD_BUF* self, CCharVoice* voicePtr) {
    func_802A3BEC(self, voicePtr);

    CVoiceHandle* h;
    CCharVoice* biased;

    h = self->slotHandles[0];
    biased = (CCharVoice*)h;
    if (h != NULL) {
        biased = &h->voice;
    }
    if (biased == voicePtr) {
        self->slotHandles[0] = NULL;
    }

    h = self->slotHandles[1];
    biased = (CCharVoice*)h;
    if (h != NULL) {
        biased = &h->voice;
    }
    if (biased == voicePtr) {
        self->slotHandles[1] = NULL;
    }

    h = self->slotHandles[2];
    biased = (CCharVoice*)h;
    if (h != NULL) {
        biased = &h->voice;
    }
    if (biased == voicePtr) {
        self->slotHandles[2] = NULL;
    }
}

// Virtual method override: returns the buffer size for this thread type.
// Matches CVS_THREAD::blank1 slot in vtable; BUF subclass returns 0x104 (260).
int CVS_THREAD_BUF::blank1() {
    return BUFFER_SIZE;
}
