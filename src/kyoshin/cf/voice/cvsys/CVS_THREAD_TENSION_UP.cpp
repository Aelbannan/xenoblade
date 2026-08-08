// CVS_THREAD_TENSION_UP: Voice thread for tension-up audio events.
// FULL_MATCH: func_802A92D0 -- buffer-size getter (virtual method override).
// 5 matched functions: completion callback, voice removal, constructor,
// update function, and voice-select + play function.

#include "kyoshin/cf/voice/cvsys/CVS_THREAD_TENSION_UP.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/harness_catalog.hpp"

// Forward declarations for external functions not yet in headers.
extern char* func_802A34E4(int size);
extern void __ct__cf_CVS_THREAD();
extern int func_802A77E8(CVoiceHandle* handle);
extern int func_802A7B90(CVoiceHandle* handle, CVoiceHandle* owner);

// C-linkage imports previously supplied by CVS_THREAD_EHP.hpp, which is no
// longer included here (it redefines CVoiceHandle; the canonical definition
// now lives in the shared base header CVS_THREAD.hpp).
extern "C" {
    int func_802A3E88(CVS_THREAD* self);
    void func_802A3BEC(CVS_THREAD* self, CCharVoice* voicePtr);
    int func_802A3C44(CVS_THREAD* self, CCharVoice* voicePtr, int voiceId);
    CVoiceHandle* func_802A330C(int size, int align);
    CVoiceHandle* func_802A7998(CVoiceHandle* exclude);
}

// The voice manager/factory object. The flags field at offset 0x3F00
// controls thread construction (bit 1 = TENSION_UP factory active).
struct CVoiceFactory {
    char _pad[0x3F00];
    u32 flags;
};

// ── Target 1: us-802ab968 (func_802A9230) ──────────────────────────────────
// Completion callback: if no active voice is playing, invoke the
// playback-start virtual (CVS_THREAD::func_802A3B50, vtable slot 2).
void func_802A9230(CVS_THREAD_TENSION_UP* self) {
    if (func_802A3E88(self) == 0) {
        self->func_802A3B50();
    }
}

// ── Target 2: us-802ab9b0 (func_802A9278) ──────────────────────────────────
// Remove a voice by matching its embedded CCharVoice pointer against the
// single slot (field_0x20). A CVoiceHandle stores the CCharVoice at offset
// 0x3E9C, so a non-null handle is biased by 0x3E9C before comparing.
void func_802A9278(CVS_THREAD_TENSION_UP* self, CCharVoice* voicePtr) {
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

// ── Target 3: us-802ab51c (__ct__802A8DE8) ─────────────────────────────────
// Factory constructor for CVS_THREAD_TENSION_UP.
// Takes a factory/manager pointer and a thread index (must be >= 3).
// Allocates a buffer and the thread object, constructs the base,
// sets vtable/fields, copies init data, returns the object (or NULL).
CVS_THREAD_TENSION_UP* __ct__802A8DE8(CVoiceFactory* factory, int index) try {
    if ((factory->flags & 0x2) == 0) {
        return NULL;
    }
    if (index < 3) {
        return NULL;
    }
    if (func_802A330C(0x82, 1) == NULL) {
        return NULL;
    }
    CVS_THREAD_TENSION_UP* self = (CVS_THREAD_TENSION_UP*)func_802A34E4(0x2c);
    if (self == NULL) {
        return NULL;
    }
    __ct__cf_CVS_THREAD();

    // Set the vtable for this subclass.
    ((void**)self)[7] = (void**)&lbl_eu_80539D44;

    // Set fields.
    self->field_0x20 = (CVoiceHandle*)factory;
    self->field_0x24 = index;
    self->field_0x28 = 0;

    // Copy the slot-state init data triple into the object's first 3 u32s.
    const u32* base = lbl_eu_80539D20;
    self->unk0 = (u32*)base[0];
    self->unk4 = base[1];
    self->unk8 = base[2];

    return self;
} catch (...) {
    throw;
}

// ── Target 4: us-802ab628 (func_802A8EEC) ──────────────────────────────────
// Update function: reloads the slot-state triple, checks voice state,
// plays appropriate voice ID (0x5DE standard or 0x5DD reversed).
void func_802A8EEC(CVS_THREAD_TENSION_UP* self) {
    self->unk4 = lbl_eu_80539D2C[1];
    self->unk0 = (u32*)lbl_eu_80539D2C[0];
    self->unk8 = lbl_eu_80539D2C[2];

    CVoiceHandle* handle = self->field_0x20;
    if (handle != NULL) {
        typedef int (*IsActiveFunc)(CVoiceHandle*);
        IsActiveFunc isActive = (IsActiveFunc)handle->vtable[0x2BC / 4];
        if (isActive(handle) != 0) {
            goto fallback;
        }
    }

    if (self->field_0x24 == 4) {
        self->field_0x28 = 0;
        CVoiceHandle* h = self->field_0x20;
        CCharVoice* voicePtr = (CCharVoice*)h;
        if (h != NULL) {
            voicePtr = &h->voice;
        }
        if (func_802A3C44(self, voicePtr, 0x5DE) != 0) {
            return;
        }
    }

    if (self->field_0x24 == 3) {
        int ownerState = func_802A77E8(self->field_0x20);
        int isThird;
        if (ownerState == 4) {
            isThird = (cf::CfGameManager::func_800822F4() < 4) ? 1 : 0;
        } else {
            isThird = 0;
        }

        if (isThird != 0) {
            self->field_0x28 = 0;
            CVoiceHandle* h = self->field_0x20;
            CCharVoice* voicePtr = (CCharVoice*)h;
            if (h != NULL) {
                voicePtr = &h->voice;
            }
            if (func_802A3C44(self, voicePtr, 0x5DE) != 0) {
                return;
            }
        } else {
            self->field_0x28 = 1;
            CVoiceHandle* h = self->field_0x20;
            CCharVoice* voicePtr = (CCharVoice*)h;
            if (h != NULL) {
                voicePtr = &h->voice;
            }
            if (func_802A3C44(self, voicePtr, 0x5DD) != 0) {
                return;
            }
        }
    }

fallback:
    self->func_802A3B50();
}

// ── Target 5: us-802ab784 (func_802A9030) ──────────────────────────────────
// Voice-select and play function. Reloads slot-state, finds a free handle,
// selects voice ID based on owner state and flag.
void func_802A9030(CVS_THREAD_TENSION_UP* self) {
    if (func_802A3E88(self) != 0) {
        return;
    }

    self->unk4 = lbl_eu_80539D38[1];
    self->unk0 = (u32*)lbl_eu_80539D38[0];
    self->unk8 = lbl_eu_80539D38[2];

    CVoiceHandle* handle = func_802A7998(self->field_0x20);
    if (handle == NULL) {
        goto fallback;
    }

    int voiceId;
    int ownerState = func_802A77E8(self->field_0x20);
    switch (ownerState) {
    case 1:
        voiceId = (self->field_0x28 != 0) ? 0x5DF : 0x5E6;
        break;
    case 2:
        voiceId = (self->field_0x28 != 0) ? 0x5E0 : 0x5E7;
        break;
    case 3:
        voiceId = (self->field_0x28 != 0) ? 0x5E1 : 0x5E8;
        break;
    case 4:
        voiceId = (self->field_0x28 != 0) ? 0x5E2 : 0x5E9;
        break;
    case 5:
        voiceId = (self->field_0x28 != 0) ? 0x5E3 : 0x5EA;
        break;
    case 6:
        voiceId = (self->field_0x28 != 0) ? 0x5E4 : 0x5EB;
        break;
    case 7:
        voiceId = (self->field_0x28 != 0) ? 0x5E5 : 0x5EC;
        break;
    default:
        voiceId = -1;
        break;
    }

    if (voiceId == 0x5E4 && func_802A7B90(handle, self->field_0x20) != 0) {
        voiceId = 0x89F;
    }
    if (voiceId == 0x5EB && func_802A7B90(handle, self->field_0x20) != 0) {
        voiceId = 0x451;
    }

    if (voiceId > 0) {
        CVoiceHandle* h = handle;
        CCharVoice* voicePtr = (CCharVoice*)h;
        if (h != NULL) {
            voicePtr = &h->voice;
        }
        if (func_802A3C44(self, voicePtr, voiceId) != 0) {
            return;
        }
    }

fallback:
    self->func_802A3B50();
}

// Virtual method override: returns the buffer size for this thread type.
// Matches CVS_THREAD::blank1 slot in vtable; TENSION_UP subclass returns 0x82 (130).
int CVS_THREAD_TENSION_UP::blank1() {
    return BUFFER_SIZE;
}