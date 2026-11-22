// Auto-scaffolded catalog TU for kyoshin/cf/voice/cvsys/CVS_THREAD_HP
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/cf/voice/cvsys/CVS_THREAD_HP.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/harness_catalog.hpp"
#include "monolib/math/Random.hpp"

// us-802a9988 (__ct__802A7254)
// Factory for CVS_THREAD_HP. Takes a source voice handle and an integer
// value (negative -> refuse). Gates on the handle's manager flag (bit 1) and
// idle state, then picks the HP-change voice ID (0x516 / 0x515) from two
// HP-ratio thresholds computed via vtable slots 72/73, allocates a 0xD2-byte
// handle buffer and the 0x28-byte thread object, runs the base constructor
// and sets the derived vtable / slot fields inside the try block, then installs
// its advancePlay handler as a real member-function pointer.

// Retail saves r28-r31 / f30-f31 individually; unit-level -O4,s merges them
// into savegpr helpers, so size-opt must be off here.
#pragma optimize_for_size off
CVS_THREAD_HP* CVS_THREAD_HP::create(CVoiceHandle* handle, int arg) {
    if (arg >= 0) {
        return NULL;
    }
    if ((handle->field_0x3F00 & 2) == 0) {
        return NULL;
    }

    int voiceId;
    if (((CVoiceVTV*)handle)->idle() == 0) {
        // HP-ratio math: slot 73 (0x12C) is the denominator, slot 72 (0x128)
        // the numerator. Remainder ratio is (num - (float)arg) / den.
        f32 den = ((CVoiceVTV*)handle)->s73();
        f32 num = ((CVoiceVTV*)handle)->s72();
        f32 rem = num - (float)arg;
        rem = rem / den;
        den = ((CVoiceVTV*)handle)->s73();
        f32 ratio = ((CVoiceVTV*)handle)->s72() / den;
        if (ratio <= 0.1f && 0.1f >= rem) {
            voiceId = 0x516;
        } else if (ratio <= 0.5f && 0.5f >= rem) {
            voiceId = 0x515;
        } else {
            voiceId = -1;
        }
    } else {
        voiceId = -1;
    }

    if (voiceId <= 0) {
        return NULL;
    }

    // Allocate the handle buffer (0xD2 bytes, discarded) and the object.
    if (CCharVoiceMan_DispatchById(0xD2, 1) == NULL) {
        return NULL;
    }
    CVS_THREAD_HP* self = (CVS_THREAD_HP*)CCharVoiceMan_AllocVoiceArena(0x28);
    if (self == NULL) {
        return NULL;
    }

    // Retail guards the constructor region with a redundant self null
    // re-check (the `beq`) and a zero-arg __throw pad; the try-block form
    // below reproduces that exception lowering.
    try {
        __ct__cf_CVS_THREAD(self);
        ((void**)self)[7] = (void*)lbl_eu_80539C1C;
        self->field_0x20 = handle;
        self->field_0x24 = voiceId;
    } catch (...) {
        throw;
    }

    self->mState = static_cast<void (CVS_THREAD::*)()>(
        &CVS_THREAD_HP::advancePlay);
    return self;
}
#pragma optimize_for_size on

// us-802a9b98 (CVS_THREAD_HP::advancePlay)
// Advance/play handler: install the playChange handler as a real member-function
// pointer, then if the voice handle slot is present and idle, play the HP-change
// voice ID; on failure fire the playback-start virtual (PlayThreadVoice).
void CVS_THREAD_HP::advancePlay() {
    CVS_THREAD_HP* self = this;
    mState = static_cast<void (CVS_THREAD::*)()>(
        &CVS_THREAD_HP::playChange);

    // Idle-check via the handle's vtable slot at 0x2BC (real r12 chain).
    CVoiceHandle* handle = self->field_0x20;
    if (handle != NULL && ((CVoiceVTV*)handle)->idle() == 0) {
        // Bias a non-null handle to its embedded CCharVoice (+0x3E9C).
        CCharVoice* biased = (CCharVoice*)self->field_0x20;
        if (self->field_0x20 != NULL) {
            biased = &self->field_0x20->voice;
        }
        if (TryRequestVoice(self, biased, self->field_0x24) == 0) {
            self->PlayThreadVoice();
        }
    }
}

// us-802a9c3c (CVS_THREAD_HP::playChange)
// Retail saves/restores r30/r31 individually; unit-level -O4,s merges them
// into stmw/lmw, so size-opt must be off here (same as HpVoice_RemoveVoice).
#pragma optimize_for_size off
// Advance/play: restore the base state triple from the init table, then pick
// a random idle voice handle (excluding this thread's own slot) and play the
// HP-change voice; on failure fire the playback-start virtual (blank1).
void CVS_THREAD_HP::playChange() {
    CVS_THREAD_HP* self = this;
    if (IsThreadVoiceActive(self) != 0) {
        return;
    }
    u32 v0;
    const u32* p = lbl_eu_80539C10;
    v0 = *p++;
    CVoiceHandle* handle = self->field_0x20;
    u32* words = (u32*)self;
    words[1] = *p++;
    words[0] = (u32)v0;
    words[2] = *p;

    // Pick a random idle handle, excluding this thread's own voice slot.
    handle = HpVoice_PickIdleVoice(handle);
    if (handle == NULL) {
        self->PlayThreadVoice();
        return;
    }
    // Bias the handle to its embedded CCharVoice (voice lives at +0x3E9C).
    CCharVoice* biased = (CCharVoice*)handle;
    if (handle != NULL) {
        biased = &handle->voice;
    }
    if (TryRequestVoice(self, biased, ml::math::mtRand(2) + 0x51F) == 0) {
        self->PlayThreadVoice();
    }
}
#pragma optimize_for_size on

// us-802a9d00 (CVS_THREAD_HP::notifyComplete)
// Completion handler: if no active voice, invoke the playback-start virtual
// (CVS_THREAD::PlayThreadVoice, vtable slot 1).
void CVS_THREAD_HP::notifyComplete() {
    CVS_THREAD_HP* self = this;
    if (IsThreadVoiceActive(self) == 0) {
        self->PlayThreadVoice();
    }
}

// Remove a voice by matching its embedded CCharVoice pointer against the
// single slot (field_0x20). A CVoiceHandle stores the CCharVoice at offset
// 0x3E9C, so a non-null handle is biased by 0x3E9C before comparing.
// Retail saves r30/r31 with individual interleaved stw/mr pairs, so size-opt
// must be off for this function (the unit-level -O4,s merges the pair into
// stmw/lmw, which retail does not use here).
#pragma optimize_for_size off
void HpVoice_RemoveVoice(CVS_THREAD_HP* self, CCharVoice* voicePtr) {
    DetachThreadVoice(self, voicePtr);

    CVoiceHandle* handle = self->field_0x20;
    CCharVoice* biased = (CCharVoice*)handle;
    if (handle != NULL) {
        biased = &handle->voice;
    }
    if (biased == voicePtr) {
        self->field_0x20 = NULL;
    }
}
#pragma optimize_for_size on


// Matches CVS_THREAD::blank1 slot in vtable; HP subclass returns 0xD2 (210).
// Retail exports the override body under the bare symbol HpVoice_GetBufSize, so
// the definition keeps the retail name (CVS_THREAD_BATTLE_END_SP precedent:
// CVSBattleEndSPGetBufSize); the blank1() member decl still satisfies the
// class interface. Returns the buffer size for this thread type.
extern "C" int HpVoice_GetBufSize() {
    return CVS_THREAD_HP::BUFFER_SIZE;
}

// us-802a9da8 (HpVoice_InitPlayerVoice)
// Retail saves/restores r30/r31 individually; unit-level -O4,s merges them
// into stmw/lmw, so size-opt must be off here (same as HpVoice_PlayChange).
#pragma optimize_for_size off
// Player-voice init: only when the handle's voice is the first player's and
// the handle is idle, allocate a voice buffer and play a random HP-change
// voice (base 0xED9). Always returns 0.
int HpVoice_InitPlayerVoice(CVoiceHandle* self) {
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
    if (CCharVoiceMan_DispatchById(0x136, 1) == NULL) {
        return 0;
    }
    CCharVoice* selfBiased = (CCharVoice*)self;
    if (self != NULL) {
        selfBiased = &self->voice;
    }
    RequestVoicePlayback(selfBiased, ml::math::mtRand(2) + 0xED9, 0x136);
    return 0;
}
#pragma optimize_for_size on

// us-802a9e78 (HpVoice_InitVoiceId)
// Voice-ID init helper. If the handle has its manager flag set, is not
// actively playing, and a handle buffer can be allocated, play a random
// HP-change voice (base 0xC81) through the sound system. Always returns 0.
int HpVoice_InitVoiceId(CVoiceHandle* self) {
    if ((self->field_0x3F00 & 2) == 0) {
        return 0;
    }
    if (((CVoiceVTV*)self)->idle() != 0) {
        return 0;
    }
    if (CCharVoiceMan_DispatchById(0x41, 1) == NULL) {
        return 0;
    }

    // Bias the handle to its embedded CCharVoice (voice lives at +0x3E9C).
    CCharVoice* biased = (CCharVoice*)self;
    if (self != NULL) {
        biased = &self->voice;
    }
    RequestVoicePlayback(biased, ml::math::mtRand(3) + 0xC81, 0x41);
    return 0;
}

// Map a voice handle's sub-state index (+0x3F28) to a voice-iterator value:
// 8 -> 3, 9 -> 8, 0xA -> 9, 0xB -> 0xA, 0xC -> 0xB, 0xD -> 0xC; any other
// value is returned unchanged.
int HpVoice_MapSubState(CVoiceHandle* handle) {
    // Masking to 0xFFFF folds into the lhz but types the value as signed
    // int, so MWCC emits signed cmpi like retail.
    int subState = ((CVoiceHandleState*)handle)->field_0x3F28 & 0xFFFF;
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
// Retail keeps two signed cmpwi (GC/3.0a5.2 codegen; the unit is GC/3.0a5.2 -
// Wii/1.1 merges the range into the unsigned (iter-1) <= 6 trick).
extern "C" int HpVoice_ValidIter(int iter) {
    int result = 0;
    if (iter >= 1) {
        if (iter <= 7) {
            result = 1;
        }
    }
    return result;
}

// us-802a9fa4 (HpVoice_CollectIdle)
// Collect idle voice handles from the voice-manager circular list into an
// output array (excluding one handle), returning how many were stored.
// `capacity` is a caller-side hint and is not used by the retail body.
// NOTE: index the store as out[count] - a manual byte-offset local folds the
// address into a pointer walk (stw+addi rOut,4); out[count] keeps base+index
// separate (retail stwx rD,rOut,rOff; addi rOff,4) under GC/3.0a5.2 -O4,s.
int HpVoice_CollectIdle(CVoiceHandle** out, int capacity, CVoiceHandle* exclude) {
    CVoiceHandle* handle;
    CVoiceManager* mgr = getListB28();
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

// us-802aa040 (HpVoice_CountActiveVoices)
// Count idle voice handles in the voice-manager circular list, excluding one
// handle (used by the party-gauge factory to check the free-voice pool).
int HpVoice_CountActiveVoices(CVoiceHandle* exclude) {
    int count;
    CVoiceHandle* handle;
    CVoiceListNode* node;
    CVoiceManager* mgr = getListB28();
    // Sentinel is re-read from the manager every iteration (the idle-check
    // virtual call may alias it, so MWCC cannot cache it in a register).
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
    }
    return count;
}

// us-802aa0cc (HpVoice_PickIdleVoice)
// Randomly pick one idle voice handle from the voice-manager circular list,
// excluding `exclude`. Returns NULL when no idle handle is available.
CVoiceHandle* HpVoice_PickIdleVoice(CVoiceHandle* exclude) {
    // Indexing the store as out[count++] keeps base and offset registers
    // separate under GC/3.0a5.2 -O4,s (retail stwx rD,rOut,rOff; addi rOff,4);
    // a manual byte-offset local folds into a pointer walk and drops a reg.
    CVoiceManager* mgr = getListB28();
    int count = 0;
    CVoiceHandle* handle;
    CVoiceHandle* out[3];
    CVoiceListNode* node = mgr->field_4->field_0;
    while (node != mgr->field_4) {
        // field_8 points at the embedded CCharVoice; bias it back to the
        // owning handle in place (retail keeps one register for both).
        handle = (CVoiceHandle*)node->field_8;
        if (handle != NULL) {
            handle = (CVoiceHandle*)((u8*)handle - 0x3E9C);
        }
        if (((CVoiceVTV*)handle)->idle() == 0 && handle != exclude) {
            out[count] = handle;
            count++;
        }
        node = node->field_0;
    }
    if (count != 0) {
        // Random pick among the collected idle handles.
        return out[ml::math::mtRand(count)];
    }
    return NULL;
}

// us-802aa188 (HpVoice_FindBySubState)
// Collect idle voice handles from the voice-manager circular list, then
// return the first one whose sub-state maps to the given iterator value
// `match` (NULL when none matches).
CVoiceHandle* HpVoice_FindBySubState(int match) {
    CVoiceManager* mgr = getListB28();
    CVoiceHandle* out[7];
    // Declaration order colours the registers like retail (MWCC colors in
    // reverse declaration order here): count -> r28, handle -> r27,
    // node -> r26.
    int count;
    CVoiceHandle* handle;
    CVoiceListNode* node;
    CVoiceHandle** p;
    int i;
    // Indexed store keeps the base and offset registers separate
    // (stwx rD,rBase,rOff; addi rOff,4), matching retail.
    for (count = 0, node = mgr->field_4->field_0; node != mgr->field_4;
         node = node->field_0) {
        // field_8 points at the embedded CCharVoice; bias it back to the
        // owning handle in place (retail keeps one register for both).
        handle = (CVoiceHandle*)node->field_8;
        if (handle != NULL) {
            handle = (CVoiceHandle*)((u8*)handle - 0x3E9C);
        }
        // Idle-check via the handle's vtable slot at 0x2BC (real r12 chain).
        if (((CVoiceVTV*)handle)->idle() == 0 && handle != NULL) {
            out[count] = handle;
            count++;
        }
    }

    // Scan with a walking pointer plus an index so the match return re-reads
    // out[i] by scaled index (retail slwi/lwzx), and the loop counter goes to
    // mtctr/bdnz under -O4,s.
    for (p = out, i = 0; i < count; p++, i++) {
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
    }
    return NULL;
}

// us-802aa2c4 (HpVoice_CheckPair26)
// Battle-state gate: map both handles' sub-state indexes to voice-iterator
// values; the HP-change voice may fire only when the pair is (2, 6) and the
// cf sequence counter is below 0x91.
int HpVoice_CheckPair26(CVoiceHandleState* a, CVoiceHandleState* b) {
    if (a == NULL) {
        return 0;
    }
    if (b == NULL) {
        return 0;
    }
    // u16 -> int conversion: the lhz still loads, but the compares become
    // signed cmpi like retail.
    int iterA;
    int subA = a->field_0x3F28;
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
    int subB = b->field_0x3F28;
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
    if (iterA == 2) {
        if (iterB == 6 && (u32)cf::CfGameManager::getQueuedFileEventCount() < 0x91u) {
            result = 1;
        }
    }
    return result;
}

// us-802aa3f8 (HpVoice_CheckPair13)
// Battle-state gate: same sub-state mapping as HpVoice_CheckPair26; the voice may
// fire only when the pair is (1, 3) and the cf sequence counter is below
// 0x24.
int HpVoice_CheckPair13(CVoiceHandleState* a, CVoiceHandleState* b) {
    if (a == NULL) {
        return 0;
    }
    if (b == NULL) {
        return 0;
    }
    int iterA;
    int subA = a->field_0x3F28;
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
    int subB = b->field_0x3F28;
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
    if (iterA == 1 && iterB == 3 && (u32)cf::CfGameManager::getQueuedFileEventCount() < 0x24u) {
        result = 1;
    }
    return result;
}

// us-802aa52c (HpVoice_CheckHpBase)
// Map a voice handle's sub-state index (+0x3F28) to a voice-iterator value
// (same mapping as HpVoice_MapSubState); when the result is the HP base state (3),
// also require the game-manager sequence counter to be below 0x24.
int HpVoice_CheckHpBase(CVoiceHandle* handle) {
    if (handle == NULL) {
        return 0;
    }
    int iter;
    // Masking to 0xFFFF folds into the lhz but types the value as signed
    // int, so MWCC emits signed cmpi like retail.
    int subState = ((CVoiceHandleState*)handle)->field_0x3F28 & 0xFFFF;
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
    if (iter == 3 && (u32)cf::CfGameManager::getQueuedFileEventCount() < 0x24u) {
        result = 1;
    }
    return result;
}

// us-802aa5e4 (HpVoice_CheckPairGate)
// Battle-state gate: map both handles' sub-state indexes to voice-iterator
// values; the HP-change voice may fire only when the pair is (7, 1) and the
// cf sequence counter is below 0x85.
int HpVoice_CheckPairGate(CVoiceHandle* a, CVoiceHandle* b) {
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
    if (iterA == 7 && iterB == 1 && (u32)cf::CfGameManager::getQueuedFileEventCount() < 0x85u) {
        result = 1;
    }
    return result;
}

// us-802aa718 (HpVoice_CheckIdleGate)
// Retail saves r28-r31 individually; unit-level -O4,s merges them into
// stmw/lmw, so size-opt must be off here (same as HpVoice_PlayChange).
#pragma optimize_for_size off
// Count idle voice handles in the voice-manager circular list; when the
// source handle's sub-state maps to one of the iterator values {1, 3, 7},
// return whether at most one idle handle is available.
int HpVoice_CheckIdleGate(CVoiceHandle* handle) {
    if (handle == NULL) {
        return 0;
    }
    int sub = ((CVoiceHandleState*)handle)->field_0x3F28;
    int iter;
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

    // {1, 3, 7} membership: each equality materialises as a cntlzw 0/1 and
    // re-tests the previous result between them (retail shape).
    int result = (iter == 1);
    if (!result) {
        result = (iter == 3);
    }
    if (!result) {
        result = (iter == 7);
    }
    if (result) {
        CVoiceManager* mgr = getListB28();
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
        result = count <= 1;
    }
    return result;
}
#pragma optimize_for_size on

// Returns whether the cf sequence counter is below the battle-end shutdown
// threshold (0x94). Retail emits the branchless highest-differing-bit scan
// ((C << cntlzw(v ^ C)) & 0x80000000), so write that shape literally.
int HpVoice_SeqBelow94() {
    int v = cf::CfGameManager::getQueuedFileEventCount();
    return (u32)(0x94 << __cntlzw(v ^ 0x94)) >> 31;
}
