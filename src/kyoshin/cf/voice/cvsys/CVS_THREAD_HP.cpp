// Auto-scaffolded catalog TU for kyoshin/cf/voice/cvsys/CVS_THREAD_HP
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/cf/voice/cvsys/CVS_THREAD_HP.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/harness_catalog.hpp"
#include "monolib/math/Random.hpp"

// ── Target 4: us-802a9988 (__ct__802A7254) ──────────────────────────────────
// Factory for CVS_THREAD_HP. Takes a source voice handle and an integer
// value (negative -> refuse). Gates on the handle's manager flag (bit 1) and
// idle state, then picks the HP-change voice ID (0x516 / 0x515) from two
// HP-ratio thresholds computed via vtable slots 74/75, allocates a 0xD2-byte
// handle buffer and the 0x28-byte thread object, runs the base constructor
// and sets the derived vtable / slot fields inside the try block, then copies
// the init-state triple.
CVS_THREAD_HP* __ct__802A7254(CVoiceHandle* handle, int arg) {
    if (arg < 0) {
        return NULL;
    }
    if ((handle->field_0x3F00 & 2) == 0) {
        return NULL;
    }

    int voiceId;
    if (((CVoiceVTV*)handle)->idle() != 0) {
        voiceId = -1;
    } else {
        // HP-ratio math: slots 74 (0x128) / 75 (0x12C) are read twice, once
        // for the remainder ratio (v74 - arg) / v75 and once for v74 / v75.
        float v75 = ((CVoiceVTV*)handle)->s73();
        float v74 = ((CVoiceVTV*)handle)->s72();
        float rem = (v74 - (float)arg) / v75;
        float v75b = ((CVoiceVTV*)handle)->s73();
        float ratio = ((CVoiceVTV*)handle)->s72() / v75b;
        if (ratio <= lbl_eu_80668CA0 && lbl_eu_80668CA0 < rem) {
            voiceId = 0x516;
        } else if (ratio <= lbl_eu_80668CA4 && lbl_eu_80668CA4 < rem) {
            voiceId = 0x515;
        } else {
            voiceId = -1;
        }
    }

    if (voiceId <= 0) {
        return NULL;
    }

    // Allocate the handle buffer (0xD2 bytes, discarded) and the object.
    if (func_802A330C(0xD2, 1) == NULL) {
        return NULL;
    }
    CVS_THREAD_HP* self = (CVS_THREAD_HP*)func_802A34E4(0x28);
    if (self == NULL) {
        return NULL;
    }

    // Retail emits a redundant self null re-check (the `beq`) guarding the
    // constructor try-block; mirror it so the guard survives.
    if (self != NULL) {
        try {
            __ct__cf_CVS_THREAD(self);
            ((void**)self)[7] = (void**)lbl_eu_80539C1C;
            self->field_0x20 = handle;
            self->field_0x24 = voiceId;
        } catch (...) {
            throw;
        }
    }

    // Copy init data from the global table using a single base pointer.
    const u32* fin = lbl_eu_80539BF8;
    self->unk0 = (u32*)fin[0];
    self->unk4 = fin[1];
    self->unk8 = fin[2];
    return self;
}

// vtable[0x2BC/4] is-active check on a voice handle.
typedef int (*IsActiveFunc)(CVoiceHandle*);

// ── Target 3: us-802a9b98 (func_802A7464) ──────────────────────────────────
// Advance/play function: restore the base state triple from the init table,
// then if the voice handle slot is present and idle, play the HP-change
// voice ID; on failure fire the playback-start virtual (blank1).
void func_802A7464(CVS_THREAD_HP* self) {
    // Restore the base state triple (lwzu/spread load-with-update form).
    // v0 is declared first so MWCC colours the lwzu destination low and the
    // base pointer high, matching retail register order.
    u32 v0;
    const u32* p = lbl_eu_80539C04;
    v0 = *p++;
    CVoiceHandle* handle = self->field_0x20;
    self->unk4 = *p++;
    self->unk0 = (u32*)v0;
    self->unk8 = *p;

    if (handle != NULL && ((IsActiveFunc)handle->vtable[0x2BC / 4])(handle) == 0) {
        // Retail re-reads the slot from memory (biased to embedded CCharVoice).
        CCharVoice* biased = (CCharVoice*)self->field_0x20;
        if (self->field_0x20 != NULL) {
            biased = &self->field_0x20->voice;
        }
        if (func_802A3C44(self, biased, self->field_0x24) == 0) {
            self->blank1();
        }
    }
}

// ── Target 2: us-802a9c3c (func_802A7508) ─────────────────────────────────
// Advance/play: restore the base state triple from the init table, then pick
// a random idle voice handle (excluding this thread's own slot) and play the
// HP-change voice; on failure fire the playback-start virtual (blank1).
void func_802A7508(CVS_THREAD_HP* self) {
    if (func_802A3E88(self) != 0) {
        return;
    }
    // Restore the base state triple (lwzu/spread load-with-update form).
    // v0 is declared first so MWCC colours the lwzu destination low and the
    // base pointer high, matching retail register order.
    u32 v0;
    const u32* p = lbl_eu_80539C10;
    v0 = *p++;
    CVoiceHandle* handle = self->field_0x20;
    self->unk4 = *p++;
    self->unk0 = (u32*)v0;
    self->unk8 = *p;

    // Pick a random idle handle, excluding this thread's own voice slot.
    handle = func_802A7998(handle);
    if (handle == NULL) {
        self->func_802A3B50();
        return;
    }
    // Bias the handle to its embedded CCharVoice (voice lives at +0x3E9C).
    CCharVoice* biased = (CCharVoice*)handle;
    if (handle != NULL) {
        biased = &handle->voice;
    }
    if (func_802A3C44(self, biased, ml::math::mtRand(2) + 0x51F) == 0) {
        self->func_802A3B50();
    }
}

// Completion callback: if no active voice, invoke the playback-start virtual
// (CVS_THREAD::func_802A3B50, vtable slot 1).
void func_802A75CC(CVS_THREAD* self) {
    if (func_802A3E88(self) == 0) {
        self->func_802A3B50();
    }
}

// Remove a voice by matching its embedded CCharVoice pointer against the
// single slot (field_0x20). A CVoiceHandle stores the CCharVoice at offset
// 0x3E9C, so a non-null handle is biased by 0x3E9C before comparing.
void func_802A7614(CVS_THREAD_HP* self, CCharVoice* voicePtr) {
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

// Virtual method override: returns the buffer size for this thread type.
// Matches CVS_THREAD::blank1 slot in vtable; HP subclass returns 0xD2 (210).
int CVS_THREAD_HP::blank1() {
    return BUFFER_SIZE;
}

// ── Target 3: us-802a9da8 (func_802A7674) ─────────────────────────────────
// Player-voice init: only when the handle's voice is the first player's and
// the handle is idle, allocate a voice buffer and play a random HP-change
// voice (base 0xED9). Always returns 0.
int func_802A7674(CVoiceHandle* self) {
    if (self == NULL) {
        return 0;
    }
    // Bias the handle to its embedded CCharVoice (voice lives at +0x3E9C).
    CCharVoice* biased = (CCharVoice*)self;
    if (self != NULL) {
        biased = &self->voice;
    }
    if (biased != (CCharVoice*)cf::CfGameManager::getPlayer(0)) {
        return 0;
    }
    if (((CVoiceVTV*)self)->idle() != 0) {
        return 0;
    }
    if (func_802A330C(0x136, 1) == NULL) {
        return 0;
    }
    CCharVoice* selfBiased = (CCharVoice*)self;
    if (self != NULL) {
        selfBiased = &self->voice;
    }
    func_802A3D54(selfBiased, ml::math::mtRand(2) + 0xED9, 0x136);
    return 0;
}

// ── Target 4: us-802a9e78 (func_802A7744) ──────────────────────────────────
// Voice-ID init helper. If the handle has its manager flag set, is not
// actively playing, and a handle buffer can be allocated, play a random
// HP-change voice (base 0xC81) through the sound system. Always returns 0.
int func_802A7744(CVoiceHandle* self) {
    if ((self->field_0x3F00 & 2) == 0) {
        return 0;
    }
    if (((IsActiveFunc)self->vtable[0x2BC / 4])(self) != 0) {
        return 0;
    }
    if (func_802A330C(0x41, 1) == NULL) {
        return 0;
    }

    // Bias the handle to its embedded CCharVoice (voice lives at +0x3E9C).
    CCharVoice* biased = (CCharVoice*)self;
    if (self != NULL) {
        biased = &self->voice;
    }
    func_802A3D54(biased, ml::math::mtRand(3) + 0xC81, 0x41);
    return 0;
}

// Map a voice handle's sub-state index (+0x3F28) to a voice-iterator value:
// 8 -> 3, 9 -> 8, 0xA -> 9, 0xB -> 0xA, 0xC -> 0xB, 0xD -> 0xC; any other
// value is returned unchanged.
int func_802A77E8(CVoiceHandle* handle) {
    u16 subState = ((CVoiceHandleState*)handle)->field_0x3F28;
    if (subState == 8) {
        return 3;
    }
    if (subState == 9) {
        return 8;
    }
    if (subState == 0xA) {
        return 9;
    }
    if (subState == 0xB) {
        return 0xA;
    }
    if (subState == 0xC) {
        return 0xB;
    }
    if (subState == 0xD) {
        return 0xC;
    }
    return subState;
}

// Validates the voice iterator: nonzero only for the state range [1, 7].
// Retail keeps two signed cmpwi (GC/3.0a5.2 codegen; the unit is GC/3.0a5.2 —
// Wii/1.1 merges the range into the unsigned (iter-1) <= 6 trick).
extern "C" int func_802A7850(int iter) {
    int result = 0;
    if (iter >= 1) {
        if (iter <= 7) {
            result = 1;
        }
    }
    return result;
}

// ── Target 2: us-802a9fa4 (func_802A7870) ──────────────────────────────────
// Collect idle voice handles from the voice-manager circular list into an
// output array (excluding one handle), returning how many were stored.
// `capacity` is a caller-side hint and is not used by the retail body.
// NOTE: index the store as out[count] — a manual byte-offset local folds the
// address into a pointer walk (stw+addi rOut,4); out[count] keeps base+index
// separate (retail stwx rD,rOut,rOff; addi rOff,4) under GC/3.0a5.2 -O4,s.
int func_802A7870(CVoiceHandle** out, int capacity, CVoiceHandle* exclude) {
    CVoiceHandle* handle;
    CVoiceManager* mgr = func_800B6BA4();
    CVoiceListNode* node;
    int count;
    for (count = 0, node = mgr->field_4->field_0; node != mgr->field_4;
         node = node->field_0) {
        // field_8 points at the embedded CCharVoice; bias it back to the
        // owning handle in place (retail keeps one register for both).
        handle = (CVoiceHandle*)node->field_8;
        if (handle != NULL) {
            handle = (CVoiceHandle*)((u8*)handle - 0x3E9C);
        }
        // Idle-check via the handle's vtable slot at 0x2BC (real r12 chain).
        if (((CVoiceVTV*)handle)->idle() == 0 && handle != exclude) {
            out[count++] = handle;
        }
    }
    return count;
}

// ── Target 1: us-802aa040 (func_802A790C) ──────────────────────────────────
// Count idle voice handles in the voice-manager circular list, excluding one
// handle (used by the party-gauge factory to check the free-voice pool).
int func_802A790C(CVoiceHandle* exclude) {
    CVoiceHandle* handle;
    CVoiceManager* mgr = func_800B6BA4();
    CVoiceListNode* node;
    int count;
    for (count = 0, node = mgr->field_4->field_0; node != mgr->field_4;
         node = node->field_0) {
        // field_8 points at the embedded CCharVoice; bias it back to the
        // owning handle in place (retail keeps one register for both).
        handle = (CVoiceHandle*)node->field_8;
        if (handle != NULL) {
            handle = (CVoiceHandle*)((u8*)handle - 0x3E9C);
        }
        // Idle-check via the handle's vtable slot at 0x2BC (real r12 chain).
        if (((CVoiceVTV*)handle)->idle() == 0 && handle != exclude) {
            count++;
        }
        node = node->field_0;
    }
    return count;
}

// ── Target 1: us-802aa0cc (func_802A7998) ─────────────────────────────────
// Randomly pick one idle voice handle from the voice-manager circular list,
// excluding `exclude`. Returns NULL when no idle handle is available.
CVoiceHandle* func_802A7998(CVoiceHandle* exclude) {
    CVoiceListNode* node;
    CVoiceHandle* handle;
    int count;
    CVoiceManager* mgr;
    CVoiceHandle* out[7];
    int off;

    mgr = func_800B6BA4();
    node = mgr->field_4->field_0;
    count = 0;
    off = 0;
    while (node != mgr->field_4) {
        // field_8 points at the embedded CCharVoice; bias it back to the
        // owning handle in place (retail keeps one register for both).
        handle = (CVoiceHandle*)node->field_8;
        if (handle != NULL) {
            handle = (CVoiceHandle*)((u8*)handle - 0x3E9C);
        }
        // Idle-check via the handle's vtable slot at 0x2BC (real r12 chain).
        if (((CVoiceVTV*)handle)->idle() == 0 && handle != exclude) {
            *(CVoiceHandle**)((u8*)out + off) = handle;
            count++;
            off += 4;
        }
        node = node->field_0;
    }
    if (count == 0) {
        return NULL;
    }
    return out[ml::math::mtRand(count)];
}

// ── Target 2: us-802aa188 (func_802A7A54) ──────────────────────────────────
// Collect idle voice handles from the voice-manager circular list, then
// return the first one whose sub-state maps to the given iterator value
// `match` (NULL when none matches). The loop walks a pointer while keeping
// the index separately so the return re-reads out[i] by index like retail.
CVoiceHandle* func_802A7A54(int match) {
    CVoiceManager* mgr = func_800B6BA4();
    CVoiceHandle* out[7];
    int count = 0;
    int off = 0;  // byte offset into the output array (retail keeps it separate)
    CVoiceHandle* handle;
    CVoiceListNode* node = mgr->field_4->field_0;
    while (node != mgr->field_4) {
        // field_8 points at the embedded CCharVoice; bias it back to the
        // owning handle in place (retail keeps one register for both).
        handle = (CVoiceHandle*)node->field_8;
        if (handle != NULL) {
            handle = (CVoiceHandle*)((u8*)handle - 0x3E9C);
        }
        // Idle-check via the handle's vtable slot at 0x2BC (real r12 chain).
        if (((CVoiceVTV*)handle)->idle() == 0 && handle != NULL) {
            *(CVoiceHandle**)((u8*)out + off) = handle;
            count++;
            off += 4;
        }
        node = node->field_0;
    }

    CVoiceHandle** p = out;
    for (int i = 0; i < count; i++) {
        CVoiceHandle* h = *p;
        int iter;
        int sub = ((CVoiceHandleState*)h)->field_0x3F28;
        if (sub == 8) {
            iter = 3;
        } else if (sub == 9) {
            iter = 8;
        } else if (sub == 0xA) {
            iter = 9;
        } else if (sub == 0xB) {
            iter = 0xA;
        } else if (sub == 0xC) {
            iter = 0xB;
        } else if (sub == 0xD) {
            iter = 0xC;
        } else {
            iter = sub;
        }
        if (match == iter) {
            return out[i];
        }
        p++;
    }
    return NULL;
}

// ── Target 4: us-802aa2c4 (func_802A7B90) ─────────────────────────────────
// Battle-state gate: map both handles' sub-state indexes to voice-iterator
// values; the HP-change voice may fire only when the pair is (2, 6) and the
// cf sequence counter is below 0x91.
int func_802A7B90(CVoiceHandleState* a, CVoiceHandleState* b) {
    if (a == NULL) {
        return 0;
    }
    if (b == NULL) {
        return 0;
    }
    u16 subA = a->field_0x3F28;
    int iterA;
    if (subA == 8) {
        iterA = 3;
    } else if (subA == 9) {
        iterA = 8;
    } else if (subA == 0xA) {
        iterA = 9;
    } else if (subA == 0xB) {
        iterA = 0xA;
    } else if (subA == 0xC) {
        iterA = 0xB;
    } else if (subA == 0xD) {
        iterA = 0xC;
    } else {
        iterA = subA;
    }
    u16 subB = b->field_0x3F28;
    int iterB;
    if (subB == 8) {
        iterB = 3;
    } else if (subB == 9) {
        iterB = 8;
    } else if (subB == 0xA) {
        iterB = 9;
    } else if (subB == 0xB) {
        iterB = 0xA;
    } else if (subB == 0xC) {
        iterB = 0xB;
    } else if (subB == 0xD) {
        iterB = 0xC;
    } else {
        iterB = subB;
    }

    int result = 0;
    if (iterA == 2 && iterB == 6 && (u32)cf::CfGameManager::func_800822F4() < 0x91u) {
        result = 1;
    }
    return result;
}

// ── Target 5: us-802aa3f8 (func_802A7CC4) ─────────────────────────────────
// Battle-state gate: same sub-state mapping as func_802A7B90; the voice may
// fire only when the pair is (1, 3) and the cf sequence counter is below
// 0x24.
int func_802A7CC4(CVoiceHandleState* a, CVoiceHandleState* b) {
    if (a == NULL) {
        return 0;
    }
    if (b == NULL) {
        return 0;
    }
    u16 subA = a->field_0x3F28;
    int iterA;
    if (subA == 8) {
        iterA = 3;
    } else if (subA == 9) {
        iterA = 8;
    } else if (subA == 0xA) {
        iterA = 9;
    } else if (subA == 0xB) {
        iterA = 0xA;
    } else if (subA == 0xC) {
        iterA = 0xB;
    } else if (subA == 0xD) {
        iterA = 0xC;
    } else {
        iterA = subA;
    }
    u16 subB = b->field_0x3F28;
    int iterB;
    if (subB == 8) {
        iterB = 3;
    } else if (subB == 9) {
        iterB = 8;
    } else if (subB == 0xA) {
        iterB = 9;
    } else if (subB == 0xB) {
        iterB = 0xA;
    } else if (subB == 0xC) {
        iterB = 0xB;
    } else if (subB == 0xD) {
        iterB = 0xC;
    } else {
        iterB = subB;
    }

    int result = 0;
    if (iterA == 1 && iterB == 3 && (u32)cf::CfGameManager::func_800822F4() < 0x24u) {
        result = 1;
    }
    return result;
}

// ── Target 5: us-802aa52c (func_802A7DF8) ──────────────────────────────────
// Map a voice handle's sub-state index (+0x3F28) to a voice-iterator value
// (same mapping as func_802A77E8); when the result is the HP base state (3),
// also require the game-manager sequence counter to be below 0x24.
int func_802A7DF8(CVoiceHandle* handle) {
    if (handle == NULL) {
        return 0;
    }
    u16 subState = ((CVoiceHandleState*)handle)->field_0x3F28;
    int iter;
    if (subState == 8) {
        iter = 3;
    } else if (subState == 9) {
        iter = 8;
    } else if (subState == 0xA) {
        iter = 9;
    } else if (subState == 0xB) {
        iter = 0xA;
    } else if (subState == 0xC) {
        iter = 0xB;
    } else if (subState == 0xD) {
        iter = 0xC;
    } else {
        iter = subState;
    }

    int result = 0;
    if (iter == 3 && (u32)cf::CfGameManager::func_800822F4() < 0x24u) {
        result = 1;
    }
    return result;
}

// ── Target 1: us-802aa5e4 (func_802A7EB0) ──────────────────────────────────
// Battle-state gate: map both handles' sub-state indexes to voice-iterator
// values; the HP-change voice may fire only when the pair is (7, 1) and the
// cf sequence counter is below 0x85.
int func_802A7EB0(CVoiceHandle* a, CVoiceHandle* b) {
    if (a == NULL) {
        return 0;
    }
    if (b == NULL) {
        return 0;
    }
    int iterA;
    int subA = ((CVoiceHandleState*)a)->field_0x3F28;
    if (subA == 8) {
        iterA = 3;
    } else if (subA == 9) {
        iterA = 8;
    } else if (subA == 0xA) {
        iterA = 9;
    } else if (subA == 0xB) {
        iterA = 0xA;
    } else if (subA == 0xC) {
        iterA = 0xB;
    } else if (subA == 0xD) {
        iterA = 0xC;
    } else {
        iterA = subA;
    }
    int iterB;
    int subB = ((CVoiceHandleState*)b)->field_0x3F28;
    if (subB == 8) {
        iterB = 3;
    } else if (subB == 9) {
        iterB = 8;
    } else if (subB == 0xA) {
        iterB = 9;
    } else if (subB == 0xB) {
        iterB = 0xA;
    } else if (subB == 0xC) {
        iterB = 0xB;
    } else if (subB == 0xD) {
        iterB = 0xC;
    } else {
        iterB = subB;
    }

    int result = 0;
    if (iterA == 7 && iterB == 1 && (u32)cf::CfGameManager::func_800822F4() < 0x85u) {
        result = 1;
    }
    return result;
}

// ── Target 3: us-802aa718 (func_802A7FE4) ──────────────────────────────────
// Count idle voice handles in the voice-manager circular list; when the
// source handle's sub-state maps to one of the iterator values {1, 3, 7},
// return whether at most one idle handle is available.
int func_802A7FE4(CVoiceHandle* handle) {
    if (handle == NULL) {
        return 0;
    }
    int iter;
    int sub = ((CVoiceHandleState*)handle)->field_0x3F28;
    if (sub == 8) {
        iter = 3;
    } else if (sub == 9) {
        iter = 8;
    } else if (sub == 0xA) {
        iter = 9;
    } else if (sub == 0xB) {
        iter = 0xA;
    } else if (sub == 0xC) {
        iter = 0xB;
    } else if (sub == 0xD) {
        iter = 0xC;
    } else {
        iter = sub;
    }

    // {1, 3, 7} membership as explicit boolean chain (retail materialises
    // each equality as a 0/1 via cntlzw and re-tests between them).
    bool b = (iter == 1);
    if (!b) b = (iter == 3);
    if (!b) b = (iter == 7);
    if (b) {
        CVoiceManager* mgr = func_800B6BA4();
        int count = 0;
        CVoiceHandle* h;
        CVoiceListNode* node = mgr->field_4->field_0;
        while (node != mgr->field_4) {
            h = (CVoiceHandle*)node->field_8;
            if (h != NULL) {
                h = (CVoiceHandle*)((u8*)h - 0x3E9C);
            }
            if (((CVoiceVTV*)h)->idle() == 0 && h != NULL) {
                count++;
            }
            node = node->field_0;
        }
        return count <= 1;
    }
    return 0;
}

// Returns whether the cf sequence counter is still below the battle-end
// shutdown threshold (0x94 = 148).
int func_802A8140() {
    return (u32)cf::CfGameManager::func_800822F4() < 0x94u;
}
