// Voice thread state machines for the battle-main voice system.
#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/voice/cvsys/CVS_THREAD.hpp"
#include "kyoshin/code_802B8A3C.hpp" // func_802A3D54 playback helper decl

// ── Shared views over the voice-owner object ─────────────────────────────

// View over the voice-owner object covering the manager flags at +0x3F00
// (bit 2 gates battle-main playback) and the sub-state id at +0x3F28.
struct BattleMainOwnerView {
    u8 _pad[0x3F00];
    u32 field_0x3F00;                 // 0x3F00
    u8 _pad2[0x3F28 - 0x3F04];
    u16 field_0x3F28;                 // 0x3F28: id matched against the table
};

// One 60-byte entry of the battle-main voice-id table; the leading word is
// the id (0 terminates the list) and the rest hold three halfword pools
// consumed by the per-event voice-id selectors.
struct VoiceIdListEntry {
    s32 id;          // 0x00
    s16 valsA[4];    // 0x04: pool for func_802AED0C
    s16 valsB[22];   // 0x0C: pool for func_802AEF80
    s16 valsC[2];    // 0x38: pool for func_802AF2DC
};

extern VoiceIdListEntry lbl_eu_8053A4B8[];

// Phantom vtable view over a CVoiceHandle vtable (vptr at +0x00) so the
// idle-check slot at byte offset 0x2BC (declared slot 173, MWCC places
// declared slot P at vtable offset (P+2)*4) dispatches as the retail
// r12-chained virtual call. Only cast + call, so no vtable is emitted.
struct CVoiceVTV {
#define CVTV_N(n) virtual void s##n();
    CVTV_N(0) CVTV_N(1) CVTV_N(2) CVTV_N(3) CVTV_N(4) CVTV_N(5) CVTV_N(6) CVTV_N(7) CVTV_N(8) CVTV_N(9)
    CVTV_N(10) CVTV_N(11) CVTV_N(12) CVTV_N(13) CVTV_N(14) CVTV_N(15) CVTV_N(16) CVTV_N(17) CVTV_N(18) CVTV_N(19)
    CVTV_N(20) CVTV_N(21) CVTV_N(22) CVTV_N(23) CVTV_N(24) CVTV_N(25) CVTV_N(26) CVTV_N(27) CVTV_N(28) CVTV_N(29)
    CVTV_N(30) CVTV_N(31) CVTV_N(32) CVTV_N(33) CVTV_N(34) CVTV_N(35) CVTV_N(36) CVTV_N(37) CVTV_N(38) CVTV_N(39)
    CVTV_N(40) CVTV_N(41) CVTV_N(42) CVTV_N(43) CVTV_N(44) CVTV_N(45) CVTV_N(46) CVTV_N(47) CVTV_N(48) CVTV_N(49)
    CVTV_N(50) CVTV_N(51) CVTV_N(52) CVTV_N(53) CVTV_N(54) CVTV_N(55) CVTV_N(56) CVTV_N(57) CVTV_N(58) CVTV_N(59)
    CVTV_N(60) CVTV_N(61) CVTV_N(62) CVTV_N(63) CVTV_N(64) CVTV_N(65) CVTV_N(66) CVTV_N(67) CVTV_N(68) CVTV_N(69)
    CVTV_N(70) CVTV_N(71) CVTV_N(72) CVTV_N(73) CVTV_N(74) CVTV_N(75) CVTV_N(76) CVTV_N(77) CVTV_N(78) CVTV_N(79)
    CVTV_N(80) CVTV_N(81) CVTV_N(82) CVTV_N(83) CVTV_N(84) CVTV_N(85) CVTV_N(86) CVTV_N(87) CVTV_N(88) CVTV_N(89)
    CVTV_N(90) CVTV_N(91) CVTV_N(92) CVTV_N(93) CVTV_N(94) CVTV_N(95) CVTV_N(96) CVTV_N(97) CVTV_N(98) CVTV_N(99)
    CVTV_N(100) CVTV_N(101) CVTV_N(102) CVTV_N(103) CVTV_N(104) CVTV_N(105) CVTV_N(106) CVTV_N(107) CVTV_N(108) CVTV_N(109)
    CVTV_N(110) CVTV_N(111) CVTV_N(112) CVTV_N(113) CVTV_N(114) CVTV_N(115) CVTV_N(116) CVTV_N(117) CVTV_N(118) CVTV_N(119)
    CVTV_N(120) CVTV_N(121) CVTV_N(122) CVTV_N(123) CVTV_N(124) CVTV_N(125) CVTV_N(126) CVTV_N(127) CVTV_N(128) CVTV_N(129)
    CVTV_N(130) CVTV_N(131) CVTV_N(132) CVTV_N(133) CVTV_N(134) CVTV_N(135) CVTV_N(136) CVTV_N(137) CVTV_N(138) CVTV_N(139)
    CVTV_N(140) CVTV_N(141) CVTV_N(142) CVTV_N(143) CVTV_N(144) CVTV_N(145) CVTV_N(146) CVTV_N(147) CVTV_N(148) CVTV_N(149)
    CVTV_N(150) CVTV_N(151) CVTV_N(152) CVTV_N(153) CVTV_N(154) CVTV_N(155) CVTV_N(156) CVTV_N(157) CVTV_N(158) CVTV_N(159)
    CVTV_N(160) CVTV_N(161) CVTV_N(162) CVTV_N(163) CVTV_N(164) CVTV_N(165) CVTV_N(166) CVTV_N(167) CVTV_N(168) CVTV_N(169)
    CVTV_N(170) CVTV_N(171) CVTV_N(172)
#undef CVTV_N
    virtual int idle();               // slot 173 -> vtable offset 0x2BC
};

// Sibling voice-id selectors implemented elsewhere in this unit.
extern "C" int func_802AEDB8(BattleMainOwnerView* owner);
extern "C" int func_802AF02C(BattleMainOwnerView* owner);
extern "C" int func_802AF13C(BattleMainOwnerView* owner);

int func_802AED0C(BattleMainOwnerView* owner);
int func_802AEF80(BattleMainOwnerView* owner);
int func_802AF2DC(BattleMainOwnerView* owner);
int func_802AF388(BattleMainOwnerView* owner);
int func_802AF3DC(CVoiceHandle* self);
int func_802AF43C(CVoiceHandle* self);
int func_802AF49C(CVoiceHandle* self);
int func_802AF4FC(CVoiceHandle* self, int param);
int func_802AF56C(CVoiceHandle* self);
int func_802AF9D0(CVoiceHandle* self, int param, int size);

// Shared tail of the three voice-id selectors: walk forward past leading
// non-positive halfwords (the walk is capped at one step), then pick a
// random element from that prefix range via mtRand.
static s16 PickRandomVoiceId(s16* vals) {
    int i = 0;
    while (i < 1 && vals[i] <= 0)
        i++;
    int n = i < 1 ? 1 : i;
    return vals[ml::math::mtRand(n)];
}

// Scan the 60-stride list at lbl_eu_8053A4B8 for the entry whose id matches
// the owner's +0x3F28 sub-state, then draw a random voice from pool A.
int func_802AED0C(BattleMainOwnerView* owner) {
    VoiceIdListEntry* p = lbl_eu_8053A4B8;
    goto check;
top:
    if (p->id != (s32)owner->field_0x3F28)
        goto next;
    goto found;
next:
    p++;
check:
    if (p->id != 0)
        goto top;
    p = NULL;
found:
    if (p == NULL)
        return -1;
    return PickRandomVoiceId(p->valsA);
}

// Same scan as func_802AED0C, drawing from pool B.
int func_802AEF80(BattleMainOwnerView* owner) {
    VoiceIdListEntry* p;
    for (p = lbl_eu_8053A4B8; p->id != 0; p++) {
        if (p->id == owner->field_0x3F28)
            goto found;
    }
    p = NULL;
found:
    if (p == NULL)
        return -1;
    return PickRandomVoiceId(p->valsB);
}

// Same scan as func_802AED0C, drawing from pool C.
int func_802AF2DC(BattleMainOwnerView* owner) {
    VoiceIdListEntry* p;
    for (p = lbl_eu_8053A4B8; p->id != 0; p++) {
        if (p->id == owner->field_0x3F28)
            goto found;
    }
    p = NULL;
found:
    if (p == NULL)
        return -1;
    return PickRandomVoiceId(p->valsC);
}

// Bit-2 gate on +0x3F00, then scan the 60-stride list at lbl_eu_8053A4B8 for
// an entry matching the +0x3F28 id; returns whether a match was found.
int func_802AF388(BattleMainOwnerView* self) {
    if (!(self->field_0x3F00 & 4))
        return 0;
    // Scan until the 0 terminator; on a match keep the entry pointer, else
    // clear it so the final boolean reports whether anything matched.
    const VoiceIdListEntry* p;
    for (p = lbl_eu_8053A4B8; p->id != 0; p++) {
        if (p->id == self->field_0x3F28)
            goto found;
    }
    p = NULL;
found:
    return p != NULL;
}

// Battle-main play helper (0xA0-byte variant): when the owner passes the
// +0x3F00 bit-2 gate, aim the embedded voice at +0x3E9C, fetch the voice id
// from the selector, and call the playback helper.
int func_802AF3DC(CVoiceHandle* self) {
    if (!(self->field_0x3F00 & 4))
        return 0;
    CCharVoice* voicePtr = (CCharVoice*)self;
    if (self != NULL) {
        voicePtr = &self->voice;
    }
    int voiceId = func_802AF02C((BattleMainOwnerView*)self);
    func_802A3D54(voicePtr, voiceId, 0xA0);
    return 0;
}

// Same shape as func_802AF3DC with a different selector and buffer size.
int func_802AF43C(CVoiceHandle* self) {
    if (!(self->field_0x3F00 & 4))
        return 0;
    CCharVoice* voicePtr = (CCharVoice*)self;
    if (self != NULL) {
        voicePtr = &self->voice;
    }
    int voiceId = func_802AEDB8((BattleMainOwnerView*)self);
    func_802A3D54(voicePtr, voiceId, 0x12C);
    return 0;
}

int func_802AF49C(CVoiceHandle* self) {
    if (!(self->field_0x3F00 & 4))
        return 0;
    CCharVoice* voicePtr = (CCharVoice*)self;
    if (self != NULL) {
        voicePtr = &self->voice;
    }
    int voiceId = func_802AEF80((BattleMainOwnerView*)self);
    func_802A3D54(voicePtr, voiceId, 0xA);
    return 0;
}

// Playback helper gated on a negative request parameter: only when param < 0
// and the +0x3F00 bit-2 gate passes is a voice drawn from the local selector
// and played through the embedded CCharVoice with a 0xB4 buffer.
int func_802AF4FC(CVoiceHandle* self, int param) {
    if (param >= 0)
        return 0;
    if (!(self->field_0x3F00 & 4))
        return 0;
    CCharVoice* voicePtr = (CCharVoice*)self;
    if (self != NULL) {
        voicePtr = &self->voice;
    }
    int voiceId = func_802AF13C((BattleMainOwnerView*)self);
    func_802A3D54(voicePtr, voiceId, 0xB4);
    return 0;
}

int func_802AF56C(CVoiceHandle* self) {
    if (!(self->field_0x3F00 & 4))
        return 0;
    CCharVoice* voicePtr = (CCharVoice*)self;
    if (self != NULL) {
        voicePtr = &self->voice;
    }
    int voiceId = func_802AF2DC((BattleMainOwnerView*)self);
    func_802A3D54(voicePtr, voiceId, 0x32);
    return 0;
}

void __ct__802AF5CC(){}

void func_802AF724(){}

void func_802AF844(){}

// When the CVS thread is not blocked, dispatch the vtable+8 slot (the
// battle-main update virtual) through the +0x1C vtable pointer.
extern "C" void func_802AF980(void* self) {
    if (func_802A3E88((CVS_THREAD*)self) != 0)
        return;
    ((CVS_THREAD*)self)->func_802A3B50();
}

int func_802AF9C8() { return 270; }

// Bit-1 gate on +0x3F00 plus the handle's idle-check virtual (vtable 0x2BC),
// then reserve a scratch buffer and play the requested voice through the
// embedded CCharVoice. Any failure returns 0 without playing.
int func_802AF9D0(CVoiceHandle* self, int param, int size) {
    if (!(self->field_0x3F00 & 2))
        return 0;
    if (((CVoiceVTV*)self)->idle() != 0)
        return 0;
    if (func_802A330C(size, 1) == NULL)
        return 0;
    CCharVoice* voicePtr = (CCharVoice*)self;
    if (self != NULL) {
        voicePtr = &self->voice;
    }
    func_802A3D54(voicePtr, param, size);
    return 0;
}
