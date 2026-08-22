// CVS_THREAD_PARTY_GAGE - voice thread for party gauge change events.
// Virtual method overrides and thread management for party gauge voice processing.

#include "kyoshin/cf/voice/cvsys/CVS_THREAD_PARTY_GAGE.hpp"
#include "monolib/math/Random.hpp"

// ── Target: us-802aac94 (func_802A8560) ────────────────────────────────
// Completion callback: if the thread is not currently busy, fire the
// playback-start virtual (blank1 slot, vtable offset 0x08).
void func_802A8560(CVS_THREAD_PARTY_GAGE* self) {
    if (func_802A3E88(self) == 0) {
        self->func_802A3B50();
    }
}

// ── Target: us-802aacdc (func_802A85A8) ────────────────────────────────
// Remove a released voice from the two slots (partyMember, gaugeData) by
// matching the handle's embedded CCharVoice pointer against the freed voice.
void func_802A85A8(CVS_THREAD_PARTY_GAGE* self, CCharVoice* voicePtr) {
    func_802A3BEC(self, voicePtr);

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

// ── Target: us-802aad5c (func_802A8628) ────────────────────────────────
// Voice-ID init helper.  If the handle has its manager flag set, is not
// actively playing, and a handle buffer can be allocated, play a random
// party-gauge voice (base 0x385) through the sound system.
int func_802A8628(CVoiceHandle* self) {
    if ((self->field_0x3F00 & 2) == 0) return 0;
    if (((CVoiceVTV*)self)->idle() != 0) return 0;
    if (func_802A330C(0xc8, 1) == NULL) return 0;

    CCharVoice* voicePtr = (CCharVoice*)self;
    if (self != NULL) {
        voicePtr = &self->voice;
    }
    func_802A3D54(voicePtr, ml::math::mtRand(2) + 0x385, 0xc8);
    return 0;
}

// ── Target: us-802aaa08 (func_802A82D4) ────────────────────────────────
// Advance/play function.  Copies init data into the base fields, then if the
// party-member handle is present and idle, plays a threshold-dependent voice;
// otherwise fires the playback-start virtual (blank1).
void func_802A82D4(CVS_THREAD_PARTY_GAGE* self) {
    u32 v0;
    const u32* p = lbl_eu_80539C54;
    v0 = *p++;
    CVoiceHandle* member = self->partyMember;
    self->unk4 = *p++;
    self->unk0 = (u32*)v0;
    self->unk8 = *p;

    if (member != NULL && ((CVoiceVTV*)member)->idle() == 0) {
        // Choose the voice ID based on the gauge-change threshold level.
        int voiceId = -1;
        switch ((s32)self->thresholdLevel) {
        case 0:
            voiceId = 0x641;
            break;
        case 1:
            voiceId = 0x641 + (func_802A7DF8(self->partyMember) == 0);
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
        if (func_802A3C44(self, voicePtr, voiceId) != 0) return;
    }
    self->func_802A3B50();
}

// ── Target: us-802aab00 (func_802A83CC) ────────────────────────────────
// Advance/play function.  If the thread is free, copies init data into the
// base fields, then (when both parties are present and the gauge is idle)
// selects a threshold-dependent voice ID, applies correction clauses for
// specific IDs, plays through func_802A3C44, and otherwise fires the
// playback-start virtual (blank1 slot, vtable offset 0x08).
void func_802A83CC(CVS_THREAD_PARTY_GAGE* self) {
    if (func_802A3E88(self) != 0) return;

    // Copy init data -- index 0 first (lwzu pointer-increment) so the retail
    // lis @ha + lwzu @l base-load form is produced.  v0 declared before p so
    // MWCC colours the lwzu destination into r3 and the base pointer into r4.
    u32 v0;
    const u32* p = lbl_eu_80539C60;
    v0 = *p++;
    self->unk4 = *p++;
    self->unk0 = (u32*)v0;
    self->unk8 = *p;

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
    iter = func_802A77E8(self->partyMember);
    voiceId = -1;
    switch ((s32)self->thresholdLevel) {
    case 0:
        if (func_802A7850(iter) != 0) voiceId = iter + 0x643;
        break;
    case 1:
        if (func_802A7850(iter) != 0) voiceId = iter + 0x64a;
        break;
    case 2:
        voiceId = ml::math::mtRand(3) + 0x65b;
        break;
    }

    // Correction clauses: remap certain voice IDs based on companion state.
    if (voiceId == 0x649) {
        if (func_802A7B90(self->gaugeData, self->partyMember) != 0) voiceId = 0x65b;
    }
    if (voiceId == 0x657) {
        if (func_802A7B90(self->gaugeData, self->partyMember) != 0) voiceId = 0x65d;
    }
    if (voiceId == 0x64d) {
        if (func_802A7CC4(self->gaugeData, self->partyMember) != 0) voiceId = 0x5e8;
    }

    if (voiceId > 0) {
        // Bias the gauge handle to its embedded CCharVoice (voice lives at +0x3E9C).
        CVoiceHandle* g2 = self->gaugeData;
        CCharVoice* vp = (CCharVoice*)g2;
        if (g2 != NULL) vp = &g2->voice;
        if (func_802A3C44(self, vp, voiceId) != 0) return;
    }

fire:
    self->func_802A3B50();
}

// Buffer-size getter recorded in the PARTY_GAGE vtable (the CVS_THREAD::blank2
// slot): this thread type uses a 0x8C (140) byte voice buffer.
int func_802A8620() {
    return CVS_THREAD_PARTY_GAGE::BUFFER_SIZE;
}

// ── Target: us-802aa8a8 (constructor factory) ────────────────────────────────
// Factory/constructor.  Picks a gauge threshold level from the two owner
// counts, allocates the two voice-handle buffers and the object itself, runs
// the base constructor, sets vtable/owner fields, and copies init data.
CVS_THREAD_PARTY_GAGE* __ct__CVS_THREAD_PARTY_GAGE(int owner1, int owner2) {
    if (owner1 <= owner2) return NULL;
    if (func_802A790C(NULL) < 2) return NULL;

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

    CVoiceHandle* member = func_802A7998((CVoiceHandle*)0);
    CVoiceHandle* gauge = func_802A7998(member);
    if (member == NULL) return NULL;
    if (func_802A330C(0x8c, 1) == NULL) return NULL;

    CVS_THREAD_PARTY_GAGE* self = (CVS_THREAD_PARTY_GAGE*)func_802A34E4(0x2c);
    if (self == NULL) return NULL;

    // Retail emits a redundant re-check (beq past the construct block) here.
    // The try only wraps the base-constructor call + field stores; the catch
    // rethrows via the runtime __throw(0,0,0) (retail `li r3,0; li r4,0;
    // li r5,0; bl __throw`).
    if (self != NULL) {
        try {
            __ct__cf_CVS_THREAD();
            // Set the subclass vtable at offset 0x1C (right after base fields).
            ((void**)self)[7] = (void*)lbl_eu_80539C6C;
            self->partyMember = member;
            self->gaugeData = gauge;
            self->thresholdLevel = (u32)level;
        } catch (...) {
            __throw(0, 0, 0);
        }
    }

    // Copy init data from the global table.
    const u32* base = (const u32*)(u32)lbl_eu_80539C48;
    u32 v1 = base[1], v0 = base[0];
    self->unk0 = (u32*)v0;
    self->unk4 = v1;
    self->unk8 = base[2];
    return self;
}