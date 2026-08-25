// CVS_THREAD_HAGE: Voice thread variant for "Hage" (bald/bare) audio.
// FULL_MATCH: func_802A724C -- buffer-size getter (virtual method override).

#include "kyoshin/cf/voice/cvsys/CVS_THREAD_HAGE.hpp"
#include "kyoshin/harness_catalog.hpp"
#include "monolib/math/Random.hpp"

// us-802a98c0 (func_802A718C)
// Completion callback: if no active voice, call the playback-start virtual.
void func_802A718C(CVS_THREAD_HAGE* self) {
    if (func_802A3E88(self) == 0) {
        self->func_802A3B50();
    }
}

// us-802a9908 (func_802A71D4)
// Remove a voice from the slots by matching its embedded CCharVoice pointer.
void func_802A71D4(CVS_THREAD_HAGE* self, CCharVoice* voicePtr) {
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

// us-802a97fc (func_802A70C8)
// Advance/play function for voice slot 2 (field_0x24).
// Copies init data from lbl_eu_80539BC0 to fields 0x00-0x08, checks if
// the voice is still active (vtable method at offset 0x2BC), and if
// inactive, plays a random voice ID (mtRand(2) + 0xC27).
void func_802A70C8(CVS_THREAD_HAGE* self) {
    if (func_802A3E88(self) == 0) {
        // v0 declared before p so the lwzu result claims the lower scratch
        // register (retail: r4) and the base pointer takes r5.
        u32 v0;
        const u32* p = lbl_eu_80539BC0;
        v0 = *p++;
        CVoiceHandle* handle = self->field_0x24;
        self->unk4 = *p++;
        self->unk0 = (u32*)v0;
        self->unk8 = *p;

        if (handle != NULL) {
            // Voice idle check via phantom-vtable slot 173 (byte offset 0x2BC);
            // emitted as a true r12-chained virtual dispatch.
            if (((CVoiceHandleVt*)handle)->isVoiceActive() == 0) {
                // Voice is not active -- try to play a random voice
                // Use conditional bias pattern to match retail (no else branch)
                CVoiceHandle* tmpHandle = self->field_0x24;
                CCharVoice* voicePtr = (CCharVoice*)tmpHandle;
                if (tmpHandle != NULL) {
                    voicePtr = &tmpHandle->voice;
                }
                int voiceId = ml::math::mtRand(2);
                voiceId += 0xC27;
                if (func_802A3C44(self, voicePtr, voiceId) == 0) {
                    // Playback failed -- call completion callback
                    self->func_802A3B50();
                }
            }
        }
    }
}

// us-802a95b8 (__ct__802A6E84)
// Factory/constructor for CVS_THREAD_HAGE. Takes two owner objects (with a
// field at offset 0x3F00 that must have bit 1 set), allocates a handle
// (0xF0 bytes, discarded) and the object itself (0x28 bytes), calls the
// base constructor, sets vtable/owner fields, and copies init data from
// lbl_eu_80539BA8.
CVS_THREAD_HAGE* __ct__802A6E84(CVoiceHandle* owner1, CVoiceHandle* owner2) {
    // Both owners must have their 0x3F00 field's bit 1 set
    if (!(owner1->field_0x3F00 & 2)) return NULL;
    if (!(owner2->field_0x3F00 & 2)) return NULL;

    // Allocate handle buffer (0xF0 bytes, discarded)
    if (func_802A330C(0xF0, 1) == NULL) return NULL;

    // Allocate the actual CVS_THREAD_HAGE object
    CVS_THREAD_HAGE* self = (CVS_THREAD_HAGE*)func_802A34E4(0x28);
    if (self == NULL) return NULL;

    // Retail emits a redundant null re-check here (the `beq` at .L_802A9640)
    // guarding the constructor try-block; mirror it so the guard survives.
    // The catch rethrows via the runtime __throw(0,0,0) (retail `li r3,0;
    // li r4,0; li r5,0; bl __throw`), avoiding the __end__catch epilogue.
    if (self != NULL) {
        try {
            // Base constructor (self already in r3), then vtable/owner fields.
            __ct__cf_CVS_THREAD();
            ((CVS_THREAD_HAGE_raw*)self)->vtable =
                (const CVS_THREAD_HAGE_VTable*)lbl_eu_80539BCC;
            self->field_0x20 = owner1;
            self->field_0x24 = owner2;
        } catch (...) {
            __throw(0, 0, 0);
        }
    }

    // Copy the init-state triple into the first 3 u32s (outside try). The
    // integer cast keeps the label materialized once via lis+addi.
    const u32* base = (const u32*)(u32)lbl_eu_80539BA8;
    register u32* p0;
    u32 v1;
    v1 = base[1];
    p0 = (u32*)base[0];
    ((CVS_THREAD_HAGE_raw*)self)->state0 = p0;
    ((CVS_THREAD_HAGE_raw*)self)->state1 = v1;
    ((CVS_THREAD_HAGE_raw*)self)->state2 = base[2];

    return self;
}

// us-802a96c0 (func_802A6F8C)
// Advance/play function for voice slot 1 (field_0x20). Uses a weighted
// random selection: mtRand(4) picks between three specific voice IDs
// (0xC24, 0xC25, 0xC26) or a dynamic ID based on the slot 2 iterator
// (iter + 0xC1C) if the iterator check and slot conflict check both pass.
// Retries until a voice ID is selected, then plays via func_802A3C44.
void func_802A6F8C(CVS_THREAD_HAGE* self) {
    // Copy init data; slot-1 handle is read mid-copy (between word 0 and
    // word 1 loads) as in retail, so keep the reads interleaved.
    // v0 declared before p so the lwzu result claims the lower scratch
    // register (retail: r4) and the label base takes r5.
    u32 v0;
    const u32* p = lbl_eu_80539BB4;
    CVoiceHandle* handle;
    v0 = *p++;
    handle = self->field_0x20;
    self->unk4 = *p++;
    self->unk0 = (u32*)v0;
    self->unk8 = *p;

    // Both slots must be populated (slot 2 is re-read from memory here)
    if (handle == NULL) return;
    if (self->field_0x24 == NULL) return;

    // Voice idle check via phantom-vtable slot 173 (byte offset 0x2BC);
    // emitted as a true r12-chained virtual dispatch.
    if (((CVoiceHandleVt*)handle)->isVoiceActive() != 0) return;

    // Get voice iterator from slot 2 handle
    int iter = func_802A77E8(self->field_0x24);
    int voiceId = -1;

    // Loop until a voice ID is selected
    while (voiceId == -1) {
        int roll = ml::math::mtRand(4);
        // Default listed first so MWCC lays it out inline after the compare
        // chain, with the constant-ID cases as forward branches (retail).
        switch (roll) {
        default: // 0 -- dynamic selection: iterator must pass its check and
                 // the two slots must not conflict, otherwise retry.
            if (func_802A7850(iter) != 0 &&
                func_802A7B90(self->field_0x20, self->field_0x24) == 0) {
                voiceId = iter + 0xC1C;
            }
            break;
        case 1:
            voiceId = 0xC24;
            break;
        case 2:
            voiceId = 0xC25;
            break;
        case 3:
            voiceId = 0xC26;
            break;
        }
    }

    // Try to play the selected voice on slot 1's embedded CCharVoice
    // (conditional bias pattern, no else branch).
    CVoiceHandle* playHandle = self->field_0x20;
    CCharVoice* voicePtr = (CCharVoice*)playHandle;
    if (playHandle != NULL) {
        voicePtr = &playHandle->voice;
    }
    if (func_802A3C44(self, voicePtr, voiceId) == 0) {
        // Playback failed -- call completion callback
        self->func_802A3B50();
    }
}

// Virtual method override: returns the buffer size for this thread type.
// Matches CVS_THREAD::blank1 slot in vtable; HAGE subclass returns 0xF0 (240).
int CVS_THREAD_HAGE::blank1() {
    return BUFFER_SIZE;
}