// CVS_THREAD_PARTY_GAGE - voice thread for party gauge change events.
// Virtual method overrides and thread management for party gauge voice processing.

#include "kyoshin/cf/voice/cvsys/CVS_THREAD_PARTY_GAGE.hpp"
#include "monolib/math/Random.hpp"

// us-802aa8a8 (constructor factory)
// Factory/constructor.  Picks a gauge threshold level from the two owner
// counts, allocates the two voice-handle buffers and the object itself, runs
// the base constructor, sets vtable/owner fields, and copies init data.
CVS_THREAD_PARTY_GAGE* CVS_THREAD_PARTY_GAGE::create(int owner1, int owner2) {
    if (owner1 <= owner2) return NULL;
    if (HpVoice_CountActiveVoices(NULL) < 2) return NULL;

    int level;
    if (owner1 >= 0x12c) {
        level = 2;
    } else if (owner2 < 0xc8 && owner1 >= 0xc8) {
        level = 1;
    } else if (owner2 < 0x64 && owner1 >= 0x64) {
        level = 0;
    } else {
        return NULL;
    }

    CVoiceHandle* member = HpVoice_PickIdleVoice((CVoiceHandle*)0);
    CVoiceHandle* gauge = HpVoice_PickIdleVoice(member);
    if (member == NULL) return NULL;
    if (CCharVoiceMan_DispatchById(0x8c, 1) == NULL) return NULL;

    CVS_THREAD_PARTY_GAGE* self = (CVS_THREAD_PARTY_GAGE*)CCharVoiceMan_AllocVoiceArena(0x2c);
    if (self == NULL) return NULL;
    CVS_THREAD_PARTY_GAGE_raw* raw = (CVS_THREAD_PARTY_GAGE_raw*)self;

    // Retail emits a redundant re-check (beq past the construct block) here.
    // The try only wraps the base-constructor call + field stores; the catch
    // rethrows via the runtime __throw(0,0,0) (retail `li r3,0; li r4,0;
    // li r5,0; bl __throw`).
    if (self != NULL) {
        try {
            __ct__cf_CVS_THREAD(self);
            // Set the subclass vtable at offset 0x1C (right after base fields).
            raw->vtable = lbl_eu_80539C6C;
            self->partyMember = member;
            self->gaugeData = gauge;
            self->thresholdLevel = (u32)level;
        } catch (...) {
            __throw(0, 0, 0);
        }
    }

    // Install the initial voice-event handler as a real member-function
    // pointer; MWCC emits the {0, -1, handler} constant itself.
    self->mState = static_cast<void (CVS_THREAD::*)()>(
        &CVS_THREAD_PARTY_GAGE::advanceA);
    return self;
}


// us-802aaa08 (PartyGage_AdvanceA_82D4)
// Advance/play function.  Copies init data into the base fields, then if the
// party-member handle is present and idle, plays a threshold-dependent voice;
// otherwise fires the playback-start virtual (blank1).
void CVS_THREAD_PARTY_GAGE::advanceA() {
    CVS_THREAD_PARTY_GAGE* self = this;
    mState = static_cast<void (CVS_THREAD::*)()>(
        &CVS_THREAD_PARTY_GAGE::advanceB);
    CVoiceHandle* member = self->partyMember;

    if (member != NULL && ((CVoiceVTV*)member)->idle() == 0) {
        // Choose the voice ID based on the gauge-change threshold level.
        int voiceId = -1;
        switch ((s32)self->thresholdLevel) {
        case 0:
            voiceId = 0x641;
            break;
        case 1:
            voiceId = 0x641 + (HpVoice_CheckHpBase(self->partyMember) == 0);
            break;
        case 2:
            voiceId = lbl_eu_80662D18[ml::math::mtRand(2)];
            break;
        }

        // Retail re-reads self->partyMember here rather than reusing the
        // cached `member` loaded at the top (both biased to embedded voice).
        CCharVoice* voicePtr = (CCharVoice*)self->partyMember;
        if (self->partyMember != NULL) {
            voicePtr = &self->partyMember->voice;
        }
        if (TryRequestVoice(self, voicePtr, voiceId) != 0) return;
    }
    self->PlayThreadVoice();
}

// us-802aab00 (PartyGage_AdvanceB_83CC)
// Advance/play function.  If the thread is free, copies init data into the
// base fields, then (when both parties are present and the gauge is idle)
// selects a threshold-dependent voice ID, applies correction clauses for
// specific IDs, plays through TryRequestVoice, and otherwise fires the
// playback-start virtual (blank1 slot, vtable offset 0x08).
void CVS_THREAD_PARTY_GAGE::advanceB() {
    CVS_THREAD_PARTY_GAGE* self = this;
    if (IsThreadVoiceActive(self) != 0) return;

    mState = static_cast<void (CVS_THREAD::*)()>(
        &CVS_THREAD_PARTY_GAGE::notifyDone);

    // Declarations hoisted above the gotos (MWCC forbids jumping past an
    // initialized declaration).
    int iter;
    int voiceId;

    if (self->partyMember == NULL) goto fire;

    CVoiceHandle* gauge;
    gauge = self->gaugeData;
    if (gauge == NULL) goto fire;
    // Virtual dispatch via the gauge vtable slot at 0x2BC (r12 chain).
    if (((CVoiceVTV*)gauge)->idle() != 0) goto fire;

    // Measure the member's iterator and pick a voice ID by gauge threshold.
    // Retail re-reads self->partyMember from memory (no long-lived cache).
    iter = HpVoice_MapSubState(self->partyMember);
    voiceId = -1;
    switch ((s32)self->thresholdLevel) {
    case 0:
        if (HpVoice_ValidIter(iter) != 0) voiceId = iter + 0x643;
        break;
    case 1:
        if (HpVoice_ValidIter(iter) != 0) voiceId = iter + 0x64a;
        break;
    case 2:
        voiceId = ml::math::mtRand(3) + 0x65b;
        break;
    }

    // Correction clauses: remap certain voice IDs based on companion state.
    if (voiceId == 0x649) {
        if (HpVoice_CheckPair26(self->gaugeData, self->partyMember) != 0) voiceId = 0x65b;
    }
    if (voiceId == 0x657) {
        if (HpVoice_CheckPair26(self->gaugeData, self->partyMember) != 0) voiceId = 0x65d;
    }
    if (voiceId == 0x64d) {
        if (HpVoice_CheckPair13(self->gaugeData, self->partyMember) != 0) voiceId = 0x5e8;
    }

    if (voiceId > 0) {
        // Bias the gauge handle to its embedded CCharVoice (voice lives at +0x3E9C).
        CVoiceHandle* g2 = self->gaugeData;
        CCharVoice* vp = (CCharVoice*)g2;
        if (g2 != NULL) vp = &g2->voice;
        if (TryRequestVoice(self, vp, voiceId) != 0) return;
    }

fire:
    self->PlayThreadVoice();
}

// Buffer-size getter recorded in the PARTY_GAGE vtable (the CVS_THREAD::blank2
// slot): this thread type uses a 0x8C (140) byte voice buffer.
// Retail exports this under the flat map name PartyGage_GetBufSize_8620 (not a mangled
// member), so it stays a C-linkage free function.
int PartyGage_GetBufSize_8620() {
    return CVS_THREAD_PARTY_GAGE::BUFFER_SIZE;
}


// us-802aac94 (PartyGage_NotifyDone_8560)
// Completion callback: if the thread is not currently busy, fire the
// playback-start virtual (blank1 slot, vtable offset 0x08).
void CVS_THREAD_PARTY_GAGE::notifyDone() {
    if (IsThreadVoiceActive(this) == 0) {
        PlayThreadVoice();
    }
}

// us-802aacdc (PartyGage_RemoveVoice_85A8)
// Remove a released voice from the two slots (partyMember, gaugeData) by
// matching the handle's embedded CCharVoice pointer against the freed voice.
void PartyGage_RemoveVoice_85A8(CVS_THREAD_PARTY_GAGE* self, CCharVoice* voicePtr) {
    DetachThreadVoice(self, voicePtr);

    // Slots 0x20/0x24: bias handle to its embedded voice, compare with voicePtr.
    CVoiceHandle* handle = self->partyMember;
    CCharVoice* biased = (CCharVoice*)handle;
    if (handle != NULL) {
        biased = &handle->voice;
    }
    if (biased == voicePtr) {
        self->partyMember = NULL;
    }

    handle = self->gaugeData;
    biased = (CCharVoice*)handle;
    if (handle != NULL) {
        biased = &handle->voice;
    }
    if (biased == voicePtr) {
        self->gaugeData = NULL;
    }
}


// us-802aad5c (PartyGage_InitVoiceId_8628)
// Voice-ID init helper.  If the handle has its manager flag set, is not
// actively playing, and a handle buffer can be allocated, play a random
// party-gauge voice (base 0x385) through the sound system.
extern "C" int PartyGage_InitVoiceId_8628(CVoiceHandle* self) {
    if ((self->field_0x3F00 & 2) == 0) return 0;
    if (((CVoiceVTV*)self)->idle() != 0) return 0;
    if (CCharVoiceMan_DispatchById(0xc8, 1) == NULL) return 0;

    CCharVoice* voicePtr = (CCharVoice*)self;
    if (self != NULL) {
        voicePtr = &self->voice;
    }
    RequestVoicePlayback(voicePtr, ml::math::mtRand(2) + 0x385, 0xc8);
    return 0;
}


// Retail section reconstruction (measured in
// .scratch/dlabels/a2/CVS_THREAD_PARTY_GAGE.txt): the .rodata typeinfo name
// (sized to absorb the retail pad), three .data init-state triples
// ({0, -1, callback}), the 7-word thread vtable ({typeinfo, 0, play,
// remove, bufsize, alwaysTrue, nullDispatch}), the trailing typeinfo-base
// word plus padding, and two .sdata objects: the voice-ID pair the code
// reads via @sda21 ({0x659, 0x65A}) and the typeinfo object ({name,
// trailing}). Global-scope data symbols: MWCC does not mangle them.
// __RTTI__Q22cf10CVS_THREAD lives in the CCharVoiceMan range and is referenced, not
// defined, here.
extern "C" void PlayThreadVoice();
extern "C" bool CCharVoiceMan_AlwaysTrue();
extern "C" int CCharVoiceMan_NullVoiceDispatch(void* self);
extern u32 __RTTI__Q22cf10CVS_THREAD[2];
extern u32 __RTTI__Q22cf21CVS_THREAD_PARTY_GAGE[2];

extern const char lbl_eu_805107C8[0x20] = "cf::CVS_THREAD_PARTY_GAGE";

u32 lbl_eu_80539C6C[7] = {
    (u32)__RTTI__Q22cf21CVS_THREAD_PARTY_GAGE,
    0,
    (u32)PlayThreadVoice,
    (u32)PartyGage_RemoveVoice_85A8,
    (u32)PartyGage_GetBufSize_8620,
    (u32)CCharVoiceMan_AlwaysTrue,
    (u32)CCharVoiceMan_NullVoiceDispatch,
};

u32 lbl_eu_80539C88[4] = {
    (u32)__RTTI__Q22cf10CVS_THREAD,
    0,
    0,
    0,
};

__declspec(section ".sdata") extern u32 lbl_eu_80662D18[2] = {
    0x659,
    0x65A,
};

__declspec(section ".sdata") extern u32 __RTTI__Q22cf21CVS_THREAD_PARTY_GAGE[2] = {
    (u32)lbl_eu_805107C8,
    (u32)lbl_eu_80539C88,
};