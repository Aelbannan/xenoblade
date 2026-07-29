// CVS_THREAD_EHP: Voice thread for EHP (Emergency HP recovery) sequences.
// Five matched functions: completion callback, voice removal, slot-rotation
// advance, slot-2 play, and the standalone EHP voice selector.

#include "kyoshin/cf/voice/cvsys/CVS_THREAD_EHP.hpp"
#include "monolib/math/Random.hpp"

// Init-data tables (3 u32s each: {0, -1, callback}). Each slot state carries
// a {field_0, field_4, callback} triple that the rotation function copies into
// unk0/unk4/unk8 when the rotating index is not at the stop index.
extern "C" u32 lbl_eu_80539B14[3]; // {0, -1, func_802A6408}
extern "C" u32 lbl_eu_80539B20[3]; // {0, -1, func_802A6718}

// Virtual method override: returns the buffer size for this thread type.
// Matches CVS_THREAD::blank1 slot in vtable; EHP subclass returns 0xB4 (180).
int func_802A6818() {
    return 0xB4;
}

// ── Target 1: us-802a8e4c (func_802A6718) ──────────────────────────────────
// Completion callback: if no active voice is playing, invoke the
// playback-start virtual (CVS_THREAD::func_802A3B50, vtable slot 0).
void func_802A6718(CVS_THREAD_EHP* self) {
    if (func_802A3E88(self) == 0) {
        self->func_802A3B50();
    }
}

// ── Target 2: us-802a8e94 (func_802A6760) ──────────────────────────────────
// Remove a voice from all slots by matching its embedded CCharVoice pointer.
// A CVoiceHandle stores the CCharVoice at offset 0x3E9C, so a non-null handle
// is biased by 0x3E9C before comparing against the incoming voice pointer.
void func_802A6760(CVS_THREAD_EHP* self, CCharVoice* voicePtr) {
    func_802A3BEC(self, voicePtr);

    // Slot 0x20
    CVoiceHandle* handle = self->field_0x20;
    CCharVoice* biased = (CCharVoice*)handle;
    if (handle != NULL) {
        biased = &handle->voice;
    }
    if (biased == voicePtr) {
        self->field_0x20 = NULL;
    }

    // Slot 0x24
    handle = self->field_0x24;
    biased = (CCharVoice*)handle;
    if (handle != NULL) {
        biased = &handle->voice;
    }
    if (biased == voicePtr) {
        self->field_0x24 = NULL;
    }

    // Rotating slots 0x2C..0x34 (count in field_0x3c)
    for (int i = 0; i < self->field_0x3c; i++) {
        handle = self->field_0x2c[i];
        biased = (CCharVoice*)handle;
        if (handle != NULL) {
            biased = &handle->voice;
        }
        if (biased == voicePtr) {
            self->field_0x2c[i] = NULL;
        }
    }
}

// ── Target 3: us-802a8cc0 (func_802A658C) ──────────────────────────────────
// Advance the rotating index (field_0x38) forward or backward depending on the
// direction flag (field_0x44), wrapping at the bounds (0..field_0x3c). When the
// index lands on the stop index (field_0x40), the playback-start virtual is
// invoked; otherwise the slot-state triple is reloaded from lbl_eu_80539B14.
void func_802A658C(CVS_THREAD_EHP* self) {
    if (func_802A3E88(self) != 0) {
        return;
    }

    if (self->field_0x44 == 0) {
        // Forward: increment, wrap to 0 once past the bound.
        int newIdx = self->field_0x38 + 1;
        self->field_0x38 = newIdx;
        if (self->field_0x3c < newIdx) {
            self->field_0x38 = 0;
        }
    } else {
        // Backward: decrement, wrap to bound-1 on underflow.
        int newIdx = self->field_0x38 - 1;
        self->field_0x38 = newIdx;
        if (newIdx < 0) {
            self->field_0x38 = self->field_0x3c - 1;
        }
    }

    if (self->field_0x38 == self->field_0x40) {
        self->func_802A3B50();
    } else {
        // Reload slot-state triple {field_0, field_4, callback}.
        self->unk4 = lbl_eu_80539B14[1];
        self->unk0 = (u32*)lbl_eu_80539B14[0];
        self->unk8 = lbl_eu_80539B14[2];
    }
}

// ── Target 4: us-802a8d84 (func_802A6650) ──────────────────────────────────
// Play function for slot 2. Reloads the slot-state triple from lbl_eu_80539B20,
// finds a free voice handle (excluding slot 2's own handle), and if that handle
// is inactive plays a random voice ID (mtRand(2) + 0x51D). On any failure the
// playback-start virtual is invoked as a fallback.
void func_802A6650(CVS_THREAD_EHP* self) {
    if (func_802A3E88(self) != 0) {
        return;
    }

    // Reload slot-state triple {field_0, field_4, callback}.
    self->unk4 = lbl_eu_80539B20[1];
    self->unk0 = (u32*)lbl_eu_80539B20[0];
    self->unk8 = lbl_eu_80539B20[2];

    CVoiceHandle* handle = func_802A7998(self->field_0x24);
    if (handle != NULL) {
        // is-active check via the handle's vtable (offset 0x2BC).
        typedef int (*VtableFunc)(CVoiceHandle*);
        VtableFunc isActive = (VtableFunc)handle->vtable[0x2BC / 4];
        if (isActive(handle) == 0) {
            // Bias handle pointer to reach the embedded CCharVoice.
            CCharVoice* voicePtr;
            if (handle != NULL) {
                voicePtr = &handle->voice;
            } else {
                voicePtr = NULL;
            }

            int voiceId = ml::math::mtRand(2) + 0x51D;
            if (func_802A3C44(self, voicePtr, voiceId) != 0) {
                return;
            }
        }
    }

    // Fallback: invoke the playback-start virtual.
    self->func_802A3B50();
}

// ── Target 5: us-802a8f54 (func_802A6820) ──────────────────────────────────
// Standalone EHP voice selector. Given two party-slot indices (a, b), it picks
// a voice ID based on their relationship, allocates a 0xAA-byte buffer, and
// plays the voice on the currently free handle. Returns 0 in all paths.
int func_802A6820(int a, int b) {
    if (a == b) {
        return 0;
    }

    CVoiceHandle* handle = func_802A7998(NULL);
    if (handle == NULL) {
        return 0;
    }

    // is-active check via the handle's vtable (offset 0x2BC).
    {
        typedef int (*VtableFunc)(CVoiceHandle*);
        VtableFunc isActive = (VtableFunc)handle->vtable[0x2BC / 4];
        if (isActive(handle) != 0) {
            return 0;
        }
    }

    // Read a u32 value from the handle's sub-object (vtable offset 0x30)
    // and check a category flag via func_80174C98.
    CVSubObj* subobj = handle->field_0x04;
    {
        typedef u32* (*GetValueFunc)(CVSubObj*);
        GetValueFunc getValue = (GetValueFunc)subobj->vtable[0x30 / 4];
        u32* p = getValue(subobj);
        u32 value = *p;
        if (func_80174C98(handle, &value, 0x803) == 0) {
            return 0;
        }
    }

    // Choose the voice ID from the relationship between a and b.
    // r30 holds b and is later reused for the selected voice ID.
    if (b < a && a >= 2) {
        b = ml::math::mtRand(2) + 0x6A5;
    } else if (a < b && a == 1) {
        b = 0x6A7;
    } else {
        return 0;
    }

    // Allocate the 0xAA-byte playback buffer.
    if (func_802A330C(0xAA, 1) == NULL) {
        return 0;
    }

    // Bias handle pointer to reach the embedded CCharVoice.
    CCharVoice* voicePtr;
    if (handle != NULL) {
        voicePtr = &handle->voice;
    } else {
        voicePtr = NULL;
    }

    func_802A3D54(voicePtr, b, 0xAA);
    return 0;
}

// Stubs for functions not yet decompiled in this TU.
void __ct__802A5ED4() {}
void func_802A617C() {}
void func_802A6408() {}

// ── Target 3: us-802a908c (func_802A6958) ──────────────────────────────────
// Voice play function.  Checks a manager flag (field_0x3f08 bit 16), then
// scans the global voice-handle list for an inactive handle whose category
// matches (func_80174C98 with 0x803).  On success allocates 0x28 bytes and
// plays voice ID 0xA8D on the original handle's embedded CCharVoice.
int func_802A6958(CVoiceHandle* arg) {
    int found = 0;

    // Test bit 16 (0x10000) of the manager flag.
    if (!(arg->field_0x3f08 & 0x10000)) {
        return 0;
    }

    // is-active check via the handle's vtable (offset 0x2BC).
    {
        typedef int (*VtableFunc)(CVoiceHandle*);
        VtableFunc isActive = (VtableFunc)arg->vtable[0x2BC / 4];
        if (isActive(arg) != 0) {
            return 0;
        }
    }

    CVoiceHandleList* list = func_800B6BC8();
    CVoiceHandleListNode* node = list->end->next;
    while (node != list->end) {
        // The list stores pointers to the embedded CCharVoice (at
        // offset voice within CVoiceHandle).  Recover the containing
        // CVoiceHandle via offsetof.
        CVoiceHandle* handle;
        if (node->value != NULL) {
            handle = (CVoiceHandle*)((char*)node->value
                - offsetof(CVoiceHandle, voice));
        } else {
            handle = NULL;
        }
        // is-active check on the recovered handle.
        {
            typedef int (*VtableFunc)(CVoiceHandle*);
            VtableFunc isActive = (VtableFunc)handle->vtable[0x2BC / 4];
            if (isActive(handle) == 0) {
                // Check category via the sub-object at field_0x04.
                CVSubObj* subobj = handle->field_0x04;
                typedef u32* (*GetValueFunc)(CVSubObj*);
                GetValueFunc getValue =
                    (GetValueFunc)subobj->vtable[0x30 / 4];
                u32* p = getValue(subobj);
                u32 value = *p;
                if (func_80174C98(handle, &value, 0x803) != 0) {
                    found = 1;
                    break;
                }
            }
        }

        node = node->next;
    }

    if (found == 0) {
        return 0;
    }

    // Allocate 0x28 bytes.
    if (func_802A330C(0x28, 1) == NULL) {
        return 0;
    }

    if (func_802A7FE4(arg) != 0) {
        return 0;
    }

    // Bias arg to get the embedded CCharVoice.
    CCharVoice* voicePtr;
    if (arg != NULL) {
        voicePtr = &arg->voice;
    } else {
        voicePtr = NULL;
    }

    func_802A3D54(voicePtr, 0xA8D, 0x28);
    return 0;
}
