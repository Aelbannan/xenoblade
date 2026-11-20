// Voice thread state machines for the battle-main voice system.
#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/voice/cvsys/HpVoiceApi.hpp"
#include "kyoshin/cf/CfMapItemManager.hpp"
#include "kyoshin/cf/voice/cvsys/CVS_THREAD.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "monolib/math/Random.hpp"

// C-linkage imports from sibling modules (no compatible declaring header in
// scope; mirrors CVS_THREAD_CHAIN.hpp).  Declared here instead of via
// code_802B8A3C.hpp so the base-ctor import carries its self argument
// (retail passes r3=self).
extern "C" {
    int  IsThreadVoiceActive(CVS_THREAD* self);
    int  TryRequestVoice(CVS_THREAD* self, CCharVoice* voicePtr, int voiceId);
    int  RequestVoicePlayback(CCharVoice* voicePtr, int voiceId, int arg);
    void __ct__cf_CVS_THREAD(void* self);
    void* CCharVoiceMan_AllocVoiceArena(int size);
    // Runtime rethrow (NMWException.h): noreturn so MWCC elides the
    // __end__catch epilogue of the catch-all handler.
    __declspec(noreturn) void __throw(char* throwtype, u32 location,
                                      u32 dtor);
}

// ── Shared views over the voice-owner object ─────────────────────────────

// Voice-owner object is CVoiceHandle (CVS_THREAD.hpp): manager flags at
// +0x3F00 (bit 2 gates battle-main playback), sub-state id at +0x3F28,
// target chain at +0x3F60.
// Retired owner-view struct -> CVoiceHandle (members field_0x3F00,
// field_0x3F28, unkTarget all declared in the shared header).
// (was: pad 0x3F00 / u32 field_0x3F00 / pad / u16 field_0x3F28 / pad /
// UnkTarget* unkTarget.)
// (line-count padding: this comment occupies the 9-line struct footprint)
// (padding)
// (padding)
// (padding)

// One 60-byte entry of the battle-main voice-id table; the leading word is
// the id (0 terminates the list) and the rest hold three halfword pools
// consumed by the per-event voice-id selectors.
struct VoiceIdListEntry {
    s32 id;          // 0x00
    s16 valsA[4];    // 0x04: pool for BmVoice_FindPoolA
    s16 valsB[22];   // 0x0C: pool for BmVoice_FindPoolB
    s16 valsC[2];    // 0x38: pool for BmVoice_FindPoolC
};
extern VoiceIdListEntry bmVoiceIdTable[];

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

// Sibling voice-id selector implemented below; the one-argument view is
// what retail BmVoice_Play12C calls it through.

extern "C" int BmVoice_FindEquip(CVoiceHandle* owner);
// Retail exports this selector as an unmangled symbol, so it keeps C linkage
// (callers' bl relocs target the plain name).
extern "C" int BmVoice_FindRatio(CVoiceHandle* owner, int arg);

// Import without a declaring header (compile-only scaffolding).
extern "C" CVoiceHandle* HpVoice_PickIdleVoice(CVoiceHandle* handle);

// Typed pointer-to-member state constants (retail 12-byte init triples).
typedef void (CVS_THREAD::*BattleMainState)();

// Word-wise overlay over the base thread's 12-byte handler ptmf (offsets
// 0x00-0x0B): each variant's init tables write three words that together
// form the member-function pointer.
struct BmStateTriple {
    u32 unk0;  // +0x00
    u32 unk4;  // +0x04
    u32 unk8;  // +0x08
};

// This subclass's vtable.
extern u32 lbl_eu_8053AA7C[7];

// Battle-main voice thread object (0x24 bytes): derives from CVS_THREAD
// (init triple at +0x00, hand-placed vtable pointer at +0x1C) plus a flag
// byte at +0x20 selecting the voice-id range.
struct BmThreadVTable {
    u8 _pad[8];
    void (*playStart)(); // vtable+8: playback-start slot (dispatched via the
                         // CVS_THREAD base virtual, not called directly)
};

// Raw view used only by the factory to plant the retail vtable pointer.
struct BmThreadRaw {
    u32 words[7];            // 0x00-0x18: init triple + base data
    BmThreadVTable* vptr;    // 0x1C
    u8 field_0x20;           // 0x20
};

class CVS_THREAD_BATTLE_MAIN : public CVS_THREAD {
public:
    u8 field_0x20;           // 0x20: flag selecting the voice-id range

    // Voice-event state handlers installed into the inherited state member.
    void refreshBase();
    void refreshRemap();
    void dispatchUpdate();
    static void* create(int arg);
};

extern "C" BattleMainState const lbl_eu_8053AA58 = &CVS_THREAD_BATTLE_MAIN::refreshBase;
extern "C" BattleMainState const lbl_eu_8053AA64 = &CVS_THREAD_BATTLE_MAIN::refreshRemap;
extern "C" BattleMainState const lbl_eu_8053AA70 = &CVS_THREAD_BATTLE_MAIN::dispatchUpdate;

// Phantom view over the owner object's vtable exposing the slot at byte
// offset 0x2A4 (declared slot 166), which returns the work object whose
// field_0x50 points at the equipment data consumed below.
struct BmOwnerVTV {
#define BM_VTV_N(n) virtual void s##n();
    BM_VTV_N(0) BM_VTV_N(1) BM_VTV_N(2) BM_VTV_N(3) BM_VTV_N(4) BM_VTV_N(5) BM_VTV_N(6) BM_VTV_N(7) BM_VTV_N(8) BM_VTV_N(9)
    BM_VTV_N(10) BM_VTV_N(11) BM_VTV_N(12) BM_VTV_N(13) BM_VTV_N(14) BM_VTV_N(15) BM_VTV_N(16) BM_VTV_N(17) BM_VTV_N(18) BM_VTV_N(19)
    BM_VTV_N(20) BM_VTV_N(21) BM_VTV_N(22) BM_VTV_N(23) BM_VTV_N(24) BM_VTV_N(25) BM_VTV_N(26) BM_VTV_N(27) BM_VTV_N(28) BM_VTV_N(29)
    BM_VTV_N(30) BM_VTV_N(31) BM_VTV_N(32) BM_VTV_N(33) BM_VTV_N(34) BM_VTV_N(35) BM_VTV_N(36) BM_VTV_N(37) BM_VTV_N(38) BM_VTV_N(39)
    BM_VTV_N(40) BM_VTV_N(41) BM_VTV_N(42) BM_VTV_N(43) BM_VTV_N(44) BM_VTV_N(45) BM_VTV_N(46) BM_VTV_N(47) BM_VTV_N(48) BM_VTV_N(49)
    BM_VTV_N(50) BM_VTV_N(51) BM_VTV_N(52) BM_VTV_N(53) BM_VTV_N(54) BM_VTV_N(55) BM_VTV_N(56) BM_VTV_N(57) BM_VTV_N(58) BM_VTV_N(59)
    BM_VTV_N(60) BM_VTV_N(61) BM_VTV_N(62) BM_VTV_N(63) BM_VTV_N(64) BM_VTV_N(65) BM_VTV_N(66) BM_VTV_N(67) BM_VTV_N(68) BM_VTV_N(69)
    BM_VTV_N(70) BM_VTV_N(71) BM_VTV_N(72) BM_VTV_N(73) BM_VTV_N(74) BM_VTV_N(75) BM_VTV_N(76) BM_VTV_N(77) BM_VTV_N(78) BM_VTV_N(79)
    BM_VTV_N(80) BM_VTV_N(81) BM_VTV_N(82) BM_VTV_N(83) BM_VTV_N(84) BM_VTV_N(85) BM_VTV_N(86) BM_VTV_N(87) BM_VTV_N(88) BM_VTV_N(89)
    BM_VTV_N(90) BM_VTV_N(91) BM_VTV_N(92) BM_VTV_N(93) BM_VTV_N(94) BM_VTV_N(95) BM_VTV_N(96) BM_VTV_N(97) BM_VTV_N(98) BM_VTV_N(99)
    BM_VTV_N(100) BM_VTV_N(101) BM_VTV_N(102) BM_VTV_N(103) BM_VTV_N(104) BM_VTV_N(105) BM_VTV_N(106) BM_VTV_N(107) BM_VTV_N(108) BM_VTV_N(109)
    BM_VTV_N(110) BM_VTV_N(111) BM_VTV_N(112) BM_VTV_N(113) BM_VTV_N(114) BM_VTV_N(115) BM_VTV_N(116) BM_VTV_N(117) BM_VTV_N(118) BM_VTV_N(119)
    BM_VTV_N(120) BM_VTV_N(121) BM_VTV_N(122) BM_VTV_N(123) BM_VTV_N(124) BM_VTV_N(125) BM_VTV_N(126) BM_VTV_N(127) BM_VTV_N(128) BM_VTV_N(129)
    BM_VTV_N(130) BM_VTV_N(131) BM_VTV_N(132) BM_VTV_N(133) BM_VTV_N(134) BM_VTV_N(135) BM_VTV_N(136) BM_VTV_N(137) BM_VTV_N(138) BM_VTV_N(139)
    BM_VTV_N(140) BM_VTV_N(141) BM_VTV_N(142) BM_VTV_N(143) BM_VTV_N(144) BM_VTV_N(145) BM_VTV_N(146) BM_VTV_N(147) BM_VTV_N(148) BM_VTV_N(149)
    BM_VTV_N(150) BM_VTV_N(151) BM_VTV_N(152) BM_VTV_N(153) BM_VTV_N(154) BM_VTV_N(155) BM_VTV_N(156) BM_VTV_N(157) BM_VTV_N(158) BM_VTV_N(159)
    BM_VTV_N(160) BM_VTV_N(161) BM_VTV_N(162) BM_VTV_N(163) BM_VTV_N(164) BM_VTV_N(165) BM_VTV_N(166)
#undef BM_VTV_N
    virtual UnkWorkObj* getWork(); // slot 167 -> vtable offset 0x2A4
};

// Phantom view exposing the float-returning pair at slots 73/74
// (byte offsets 0x128/0x12C) used by the ratio selector.
struct BmOwnerFloatVTV {
#define BMF_VTV_N(n) virtual void s##n();
    BMF_VTV_N(0) BMF_VTV_N(1) BMF_VTV_N(2) BMF_VTV_N(3) BMF_VTV_N(4) BMF_VTV_N(5) BMF_VTV_N(6) BMF_VTV_N(7) BMF_VTV_N(8) BMF_VTV_N(9)
    BMF_VTV_N(10) BMF_VTV_N(11) BMF_VTV_N(12) BMF_VTV_N(13) BMF_VTV_N(14) BMF_VTV_N(15) BMF_VTV_N(16) BMF_VTV_N(17) BMF_VTV_N(18) BMF_VTV_N(19)
    BMF_VTV_N(20) BMF_VTV_N(21) BMF_VTV_N(22) BMF_VTV_N(23) BMF_VTV_N(24) BMF_VTV_N(25) BMF_VTV_N(26) BMF_VTV_N(27) BMF_VTV_N(28) BMF_VTV_N(29)
    BMF_VTV_N(30) BMF_VTV_N(31) BMF_VTV_N(32) BMF_VTV_N(33) BMF_VTV_N(34) BMF_VTV_N(35) BMF_VTV_N(36) BMF_VTV_N(37) BMF_VTV_N(38) BMF_VTV_N(39)
    BMF_VTV_N(40) BMF_VTV_N(41) BMF_VTV_N(42) BMF_VTV_N(43) BMF_VTV_N(44) BMF_VTV_N(45) BMF_VTV_N(46) BMF_VTV_N(47) BMF_VTV_N(48) BMF_VTV_N(49)
    BMF_VTV_N(50) BMF_VTV_N(51) BMF_VTV_N(52) BMF_VTV_N(53) BMF_VTV_N(54) BMF_VTV_N(55) BMF_VTV_N(56) BMF_VTV_N(57) BMF_VTV_N(58) BMF_VTV_N(59)
    BMF_VTV_N(60) BMF_VTV_N(61) BMF_VTV_N(62) BMF_VTV_N(63) BMF_VTV_N(64) BMF_VTV_N(65) BMF_VTV_N(66) BMF_VTV_N(67) BMF_VTV_N(68) BMF_VTV_N(69)
    BMF_VTV_N(70) BMF_VTV_N(71)
#undef BMF_VTV_N
    virtual float get128(); // slot 72 -> 0x128
    virtual float get12C(); // slot 73 -> 0x12C
    virtual float get130(); // slot 74 -> 0x130
};

// Stack-frame record built by the factory __ct__802AF5CC (retail returns
// the frame base pointer).
struct BmCtorFrame {
    u32 field_0x00;
    u8 _pad4[4];
    u32 field_0x08;          // value captured via the handle->field_4 virtual
};

// View over the sub-object hanging off the voice handle at +0x04: its
// vtable slot at byte offset 0x30 (declared slot 10) is probed before the
// capture, and its leading word is re-read afterwards.
struct BmSubResult {
    u32 word0;               // +0x00: word captured into the frame local
};
struct BmSubVTV {
#define BSUB_N(n) virtual void s##n();
    BSUB_N(0) BSUB_N(1) BSUB_N(2) BSUB_N(3) BSUB_N(4) BSUB_N(5)
    BSUB_N(6) BSUB_N(7) BSUB_N(8) BSUB_N(9)
#undef BSUB_N
    virtual BmSubResult* getResult(); // slot 10 -> vtable offset 0x30
};
// BmHandleSubView retired -> CVoiceHandle::unk4 (declared in CVS_THREAD.hpp).
// (was: address-view struct over handle+0x04; the member exists, untyped.)
// (kept as 4 comment lines to preserve downstream line numbers.)
//

int BmVoice_FindPoolA(CVoiceHandle* owner);
int BmVoice_PlayA0(CVoiceHandle* self);
int BmVoice_Play12C(CVoiceHandle* self);
int BmVoice_Play0A(CVoiceHandle* self);
int BmVoice_PlayReqB4(CVoiceHandle* self, int param);
int BmVoice_Play32(CVoiceHandle* self);
int BmVoice_PlayGated(CVoiceHandle* self, int param, int size);
extern "C" int BmVoice_FindPoolB(CVoiceHandle* owner);
// C-linkage so the retail unmangled symbol BmVoice_FindPoolC is referenced.
extern "C" int BmVoice_FindPoolC(CVoiceHandle* owner);
int BmVoice_HasEntry(CVoiceHandle* owner);

// Scan the 60-stride list at bmVoiceIdTable for the entry whose id matches
// the owner's +0x3F28 sub-state, then draw a random voice from pool A.
// Goto shapes pin MWCC's branch layout: two-way exit at 'top' (bne/b) and a
// pre-tested walk loop whose counter is tested before each pool halfword.
int BmVoice_FindPoolA(CVoiceHandle* owner) {
    VoiceIdListEntry* e = bmVoiceIdTable;
    int cur;
    // Structured scan: the shared 'cur' feeds both the terminator test and
    // the match compare; the continue-arm pins retail's bne/b branch split.
    while ((cur = e->id) != 0) {
        // Two-way exit; MWCC canonicalizes this pair into a single inverted
        // branch regardless of syntactic form (goto pairs, continue,
        // switch, volatile) - see open-item notes.
        if (cur != (s32)owner->field_0x3F28)
            e++;
        else
            goto found;
    }
    e = NULL;
found:
    if (e == NULL)
        return -1;

    // Walk the leading positive run of pool A (one step max).
    char* c = (char*)e;
    int i = 0;
    goto wcheck;
winc:
    c += 2;
    i++;
wcheck:
    if (i >= 1)
        goto wdone;
    if (*(s16*)(c + 4) > 0)
        goto winc;
wdone:
    int n = i < 1 ? 1 : i;
    return *(s16*)((char*)e + ml::math::mtRand(n) * 2 + 4);
}

// Battle-main play helper (0x0A-byte variant): when the owner passes the
// +0x3F00 bit-2 gate, aim the embedded voice at +0x3E9C, fetch the voice id
// from the selector, and call the playback helper.
// Defined BEFORE BmVoice_FindPoolB so MWCC cannot see the callee body and
// inline it (retail keeps the bl).
int BmVoice_Play0A(CVoiceHandle* self) {
    if (!(self->field_0x3F00 & 4))
        return 0;
    CCharVoice* voicePtr = (CCharVoice*)self;
    if (self != NULL) {
        voicePtr = &self->voice;
    }
    // File-scope extern "C" decl (above) + on the
    // definition keep this call a bl (matches retail); MWCC rejects
    // block-scope extern "C", so no local re-declaration here.
    int voiceId = BmVoice_FindPoolB((CVoiceHandle*)self);
    RequestVoicePlayback(voicePtr, voiceId, 0xA);
    return 0;
}

// : retail keeps this selector outlined (callers emit
// bl); without it MWCC's -O4 inliner copies the body into BmVoice_Play0A.
// Same goto-shaped body as BmVoice_FindPoolA (pool B at +0x0C); inlining the
// walk here instead of via the helper keeps MWCC's register colours equal
// to retail.
int BmVoice_FindPoolB(CVoiceHandle* owner) {
    VoiceIdListEntry* e = bmVoiceIdTable;
    int cur;
    // Terminator on the freshly loaded word; negated match arm uses
    // continue-in-block so the two-way exit survives (any goto out of the
    // loop re-triggers MWCC's branch fold into one inverted conditional).
    for (;;) {
        cur = e->id;
        if (cur == 0) {
            e = NULL;
            break;
        }
        if (cur != (s32)owner->field_0x3F28) {
            e++;
            continue;
        }
        break;
    }
found:
    if (e == NULL)
        return -1;

    // Walk the leading positive run of pool B (one step max).
    char* c = (char*)e;
    int i = 0;
    goto wcheck;
winc:
    c += 2;
    i++;
wcheck:
    if (i >= 1)
        goto wdone;
    if (*(s16*)(c + 12) > 0)
        goto winc;
wdone:
    int n = i < 1 ? 1 : i;
    return *(s16*)((char*)e + ml::math::mtRand(n) * 2 + 12);
}

// Same scan as BmVoice_FindPoolA, drawing from pool C.
// : retail keeps this selector outlined; without it
// MWCC's -O4 inliner copies the body into BmVoice_Play32.
// : retail keeps this selector outlined; without it
// MWCC's -O4 inliner copies the body into BmVoice_Play32.
// Same goto-shaped body as BmVoice_FindPoolA (pool C at +0x38).
int BmVoice_FindPoolC(CVoiceHandle* owner) {
    VoiceIdListEntry* e = bmVoiceIdTable;
    int cur;
    // Terminator on the freshly loaded word; negated match arm uses
    // continue-in-block (best-known shape for retail's branch layout -
    // see open-item notes on the loop-exit branch polarity wall).
    for (;;) {
        cur = e->id;
        if (cur == 0) {
            e = NULL;
            break;
        }
        if (cur != (s32)owner->field_0x3F28) {
            e++;
            continue;
        }
        break;
    }
found:
    if (e == NULL)
        return -1;

    // Walk the leading positive run of pool C (one step max).
    char* c = (char*)e;
    int i = 0;
    goto wcheck;
winc:
    c += 2;
    i++;
wcheck:
    if (i >= 1)
        goto wdone;
    if (*(s16*)(c + 0x38) > 0)
        goto winc;
wdone:
    int n = i < 1 ? 1 : i;
    return *(s16*)((char*)e + ml::math::mtRand(n) * 2 + 0x38);
}

// ── us-802b1a9c (BmVoice_FindEquip) ─────────────────────────────────────────────
// Scan the 60-stride list at bmVoiceIdTable for the entry matching the
// owner's +0x3F28 sub-state, validate the 0x3F60 -> 0x08 -> 0x18 chain
// (value must lie in [0x10,0x18]), fetch the equipment-data pool selected
// by the work object's byte at +0x77, and draw a random halfword from the
// leading run of positive entries (capped at 2).
// Same-TU inline search helper (btm_bda_to_acl pattern): the inlined
// `return p` lowers to retail's branch-over-branch `bne next; b found`
// split, which every direct loop form folds into a single beq.
static __inline VoiceIdListEntry* bmFindEntry(CVoiceHandle* owner) {
    VoiceIdListEntry* p = bmVoiceIdTable;
    int cur;
    while ((cur = p->id) != 0) {
        if (cur == (s32)owner->field_0x3F28)
            return p;
        p++;
    }
    return NULL;
}

int BmVoice_FindEquip(CVoiceHandle* owner) {
    VoiceIdListEntry* p = bmFindEntry(owner);
    if (p == NULL)
        return -1;

    // Guard chain: null target gets its own exit; the state range check
    // shares one load across an || so retail emits blt/ble around one
    // shared fail block.
    UnkTarget* target = owner->unkTarget;
    if (target == NULL)
        return -1;
    UnkTargetInner* inner = target->field_0x08;
    int state = inner->field_0x18;
    // Range gate shaped so test 1 branches over test 2 into the shared
    // fail block, and test 2 branches over the fail block when passing.
    if (state < 0x10)
        goto fail;
    // Commuted second bound (0x18 < state) blocks MWCC's subi/cmpli
    // range-check fusion; retail keeps two separate cmpi tests.
    if (0x18 < state)
        goto fail;
    goto ok;
fail:
    return -1;
ok:

    // Slot 0x2A4 yields the work object; its +0x50 selects the pool byte.
    UnkEquipData* equip = ((BmOwnerVTV*)owner)->getWork()->field_0x50;
    if (equip == NULL)
        return -1;

    // Pool base lands in the scan-variable register (r31 in retail) and a
    // separate cursor walks it; the random pick indexes the pool base.
    // Cursor copy of p advanced first; p then re-points via the cursor
    // (retail add-into-temp / addi-into-base / mr chain).
    char* c = (char*)p;
    c += equip->field_0x77 * 4;
    p = (VoiceIdListEntry*)(c + 0xE);
    s16* v = (s16*)p;
    int i = 0;
    goto wchk;
winc:
    v += 1;
    i++;
wchk:
    if (i >= 2)
        goto wdone;
    if (*v > 0)
        goto winc;
wdone:
    int n = i < 1 ? 1 : i;
    return ((s16*)p)[ml::math::mtRand(n)];
}

// Bit-2 gate on +0x3F00, then scan the 60-stride list at bmVoiceIdTable for
// an entry matching the +0x3F28 id; returns whether a match was found.
int BmVoice_HasEntry(CVoiceHandle* self) {
    if (!(self->field_0x3F00 & 4))
        return 0;
    // Scan until the 0 terminator; on a match keep the entry pointer, else
    // clear it so the final boolean reports whether anything matched.
    // The +0x3F28 halfword is reloaded every iteration (retail lhz inside
    // the loop), so the match test sits in the loop body.
    const VoiceIdListEntry* p = bmVoiceIdTable;
    int cur;
    goto check;
top:
    if (cur == (s32)self->field_0x3F28)
        goto found;
next:
    p++;
check:
    cur = p->id;
    if (cur != 0)
        goto top;
    p = NULL;
found:
    return p != NULL;
}

// Battle-main play helper (0xA0-byte variant): when the owner passes the
// +0x3F00 bit-2 gate, aim the embedded voice at +0x3E9C, fetch the voice id
// from the selector, and call the playback helper.
int BmVoice_PlayA0(CVoiceHandle* self) {
    if (!(self->field_0x3F00 & 4))
        return 0;
    CCharVoice* voicePtr = (CCharVoice*)self;
    if (self != NULL) {
        voicePtr = &self->voice;
    }
    int voiceId = BmVoice_FindEquip((CVoiceHandle*)self);
    RequestVoicePlayback(voicePtr, voiceId, 0xA0);
    return 0;
}

// Same shape as BmVoice_PlayA0 with a different selector and buffer size.
// Declared above with one argument on purpose: retail calls it passing
// only r3 (the full three-parameter definition appears further below).
int BmVoice_Play12C(CVoiceHandle* self) {
    // Retail calls the three-parameter selector passing only r3; mirror that
    // with a narrow local declaration so no r4/r5 setup is emitted.
    extern int BmVoice_FindGated(CVoiceHandle* owner);
    if (!(self->field_0x3F00 & 4))
        return 0;
    CCharVoice* voicePtr = (CCharVoice*)self;
    if (self != NULL) {
        voicePtr = &self->voice;
    }
    int voiceId = BmVoice_FindGated((CVoiceHandle*)self);
    RequestVoicePlayback(voicePtr, voiceId, 0x12C);
    return 0;
}

// ── us-802b1828 (BmVoice_FindGated) ─────────────────────────────────────────────
// Full retail signature (thread state, voice handle, sub-state id).
// Scans the 60-stride id table for the thread's +0x3F28 sub-state; on a hit,
// draws a random voice id from the pool selected by the handle's battle
// state. When the handle gate (flag bit 1, state 1, work state 0x10-0x17)
// passes it probes pool +0x36 (cap 1) and only accepts a positive draw,
// otherwise it falls through to the id-based pools (+0xA cap 1 for nonzero
// id, +6 cap 2 for zero id).
extern "C" int BmVoice_FindGated(CVoiceHandle* thread,
                             CVoiceHandle* handle, int id) {
    // Locate the table row whose id equals the thread's +0x3F28 sub-state.
    // Declaration order (table, id copy, handle copy) drives MWCC's prologue
    // scheduling; the shared `cur` keeps the row word in one register for
    // both the terminator test and the match compare.
    // Scan via the shared inline finder: the inlined `return p` lowers to
    // retail's pre-tested layout with the bne-over-increment / b-found
    // two-way exit (same mechanism as BmVoice_FindEquip).
    VoiceIdListEntry* e = bmFindEntry(thread);
found:
    if (e == NULL)
        return -1;

    // Gate: flag bit 1, battle state 1, work state within [0x10,0x17].
    // Every arm assigns `gated` AFTER all calls and each arm ends with an
    // explicit jump to the shared test, mirroring retail's li/b/li layout.
    int gated;
    if (!(handle->field_0x3F00 & 2)) {
        gated = 0;
        goto endg;
    }
    if (HpVoice_MapSubState((CVoiceHandle*)handle) != 1) {
        gated = 0;
        goto endg;
    }
    {
        UnkTargetInner* w = handle->unkTarget->field_0x08;
        int st = w->field_0x18;
        // Separate bounds (second one commuted) keep retail's two cmpi
        // tests - MWCC otherwise fuses them into a subi/subfic range trick.
        if (st < 0x10)
            goto failg;
        if (0x17 < st)
            goto failg;
        goto setg;
    }
failg:
    gated = 0;
    goto endg;
setg:
    gated = 1;
endg:

    if (gated != 0) {
        // Walk the leading positive run of pool +0x36 (one step max).
        // Pre-tested goto walk mirrors retail's b-over-increment layout.
        char* c = (char*)e;
        int i = 0;
        goto g36chk;
g36inc:
        c += 2;
        i++;
g36chk:
        if (i >= 1)
            goto g36done;
        if (reinterpret_cast<VoiceIdListEntry*>(c)->valsB[21] > 0)
            goto g36inc;
g36done:
        int n = i < 1 ? 1 : i;
        // Only accept a positive draw; otherwise fall through to the pools.
        s16 pick = (&e->valsB[21])[ml::math::mtRand(n)];
        if (pick > 0)
            return pick;
    }

    if (id != 0) {
        // Walk the leading positive run of pool +0xA (one step max).
        char* c = (char*)e;
        int i = 0;
        goto ga_chk;
ga_inc:
        c += 2;
        i++;
ga_chk:
        if (i >= 1)
            goto ga_done;
        if (reinterpret_cast<VoiceIdListEntry*>(c)->valsA[3] > 0)
            goto ga_inc;
ga_done:
        int n = i < 1 ? 1 : i;
        return (&e->valsA[3])[ml::math::mtRand(n)];
    }

    // Walk the leading positive run of pool +6 (two steps max).
    char* c = (char*)e;
    int i = 0;
    goto g06chk;
g06inc:
    c += 2;
    i++;
g06chk:
    if (i >= 2)
        goto g06done;
    if (reinterpret_cast<VoiceIdListEntry*>(c)->valsA[1] > 0)
        goto g06inc;
g06done:
    int n = i < 1 ? 1 : i;
    return (&e->valsA[1])[ml::math::mtRand(n)];
}

// Playback helper gated on a negative request parameter: only when param < 0
// and the +0x3F00 bit-2 gate passes is a voice drawn from the local selector
// and played through the embedded CCharVoice with a 0xB4 buffer.
int BmVoice_PlayReqB4(CVoiceHandle* self, int param) {
    if (param >= 0)
        return 0;
    if (!(self->field_0x3F00 & 4))
        return 0;
    CCharVoice* voicePtr = (CCharVoice*)self;
    if (self != NULL) {
        voicePtr = &self->voice;
    }
    // Retail forwards the request parameter into the selector unchanged.
    int voiceId = BmVoice_FindRatio((CVoiceHandle*)self, param);
    RequestVoicePlayback(voicePtr, voiceId, 0xB4);
    return 0;
}

int BmVoice_Play32(CVoiceHandle* self) {
    if (!(self->field_0x3F00 & 4))
        return 0;
    CCharVoice* voicePtr = (CCharVoice*)self;
    if (self != NULL) {
        voicePtr = &self->voice;
    }
    int voiceId = BmVoice_FindPoolC((CVoiceHandle*)self);
    RequestVoicePlayback(voicePtr, voiceId, 0x32);
    return 0;
}

// ── us-802b203c (__ct__802AF5CC) ────────────────────────────────────────────
// Factory/constructor for the battle-main voice thread. Registers against
// the player's voice handle (embedded voice at +0x3E9C), allocates the
// scratch handle (0x10E) plus the 0x24-byte thread, constructs it with the
// battle-main vtable and the caller's id, then seeds the init-state triple.
// Retail returns the stack-frame record address.
void* CVS_THREAD_BATTLE_MAIN::create(int arg) {
    CCharVoice* voice = (CCharVoice*)cf::CfGameManager::getPlayer(0);
    CVoiceHandle* handle = (CVoiceHandle*)voice;
    if (voice != NULL) {
        handle = (CVoiceHandle*)((u8*)voice - 0x3E9C);
    }
    if (handle == NULL)
        return 0;

    // The sub-object's slot-0x30 virtual returns a result node; its leading
    // word is the value captured into the frame local.
    u32 captured =
        ((BmSubVTV*)handle->unk4)->getResult()->word0;
    if (testActorStatus(handle, &captured, 0x803) == 0)
        return 0;

    // Idle check via the handle's vtable slot at 0x2BC.
    if (((CVoiceVTV*)handle)->idle() != 0)
        return 0;

    // Scratch allocation (discarded) then the thread object itself.
    if (CCharVoiceMan_DispatchById(0x10e, 1) == NULL)
        return 0;
    CVS_THREAD_BATTLE_MAIN* self =
        (CVS_THREAD_BATTLE_MAIN*)(u32)CCharVoiceMan_AllocVoiceArena(0x24);
    if (self == NULL)
        return 0;

    if (self != NULL) {
        try {
            __ct__cf_CVS_THREAD(self);
            ((BmThreadRaw*)self)->vptr = (BmThreadVTable*)lbl_eu_8053AA7C;
            self->field_0x20 = (u8)arg;
        } catch (...) {
            __throw(0, 0, 0);
        }
    }

    // Seed the init-state triple; load word 1 before word 0 so the register
    // colours match retail (r0 = word 1, r4 = word 0).  Returns self (the
    // constructed thread), which keeps r3 live to the epilogue.
    self->mState = lbl_eu_8053AA58;

    return self;
}

// ── us-802b2194 (BmVoice_RefreshBase) ─────────────────────────────────────────────
// Periodic battle voice refresh: restore the init-state triple, and while
// the player's voice handle reports idle, play a random line (range picked
// by the thread's flag byte) through the embedded voice.
void CVS_THREAD_BATTLE_MAIN::refreshBase() {
    CVS_THREAD_BATTLE_MAIN* self = this;
    mState = lbl_eu_8053AA64;

    CCharVoice* voice = (CCharVoice*)cf::CfGameManager::getPlayer(0);
    CVoiceHandle* handle = (CVoiceHandle*)voice;
    if (voice != NULL) {
        handle = (CVoiceHandle*)((u8*)voice - 0x3E9C);
    }
    if (handle != NULL && ((CVoiceVTV*)handle)->idle() == 0) {
        if (self->field_0x20 != 0) {
            CCharVoice* v = (CCharVoice*)handle;
            if (handle != NULL) {
                v = &handle->voice;
            }
            if (TryRequestVoice((CVS_THREAD*)self, v, ml::math::mtRand(2) + 0x51B) == 0)
                ((CVS_THREAD*)self)->PlayThreadVoice();
        } else {
            CCharVoice* v = (CCharVoice*)handle;
            if (handle != NULL) {
                v = &handle->voice;
            }
            if (TryRequestVoice((CVS_THREAD*)self, v, ml::math::mtRand(2) + 0x519) == 0)
                ((CVS_THREAD*)self)->PlayThreadVoice();
        }
    }
}

// ── us-802b22b4 (BmVoice_RefreshRemap) ─────────────────────────────────────────────
// Same refresh shape as BmVoice_RefreshBase but gated on the thread-blocked check
// and an extra handle remap through HpVoice_PickIdleVoice before the idle probe.
void CVS_THREAD_BATTLE_MAIN::refreshRemap() {
    CVS_THREAD_BATTLE_MAIN* self = this;
    if (IsThreadVoiceActive((CVS_THREAD*)self) != 0)
        return;

    mState = lbl_eu_8053AA70;

    CCharVoice* voice = (CCharVoice*)cf::CfGameManager::getPlayer(0);
    CVoiceHandle* handle = (CVoiceHandle*)voice;
    if (voice != NULL) {
        handle = (CVoiceHandle*)((u8*)voice - 0x3E9C);
    }
    // Remap the handle, requiring both the input and the result non-null.
    CVoiceHandle* mapped = HpVoice_PickIdleVoice(handle);
    if (handle != NULL && mapped != NULL && ((CVoiceVTV*)mapped)->idle() == 0) {
        if (self->field_0x20 != 0) {
            CCharVoice* v = (CCharVoice*)mapped;
            if (mapped != NULL) {
                v = &mapped->voice;
            }
            if (TryRequestVoice((CVS_THREAD*)self, v, ml::math::mtRand(2) + 0x523) == 0)
                ((CVS_THREAD*)self)->PlayThreadVoice();
        } else {
            CCharVoice* v = (CCharVoice*)mapped;
            if (mapped != NULL) {
                v = &mapped->voice;
            }
            if (TryRequestVoice((CVS_THREAD*)self, v, ml::math::mtRand(2) + 0x521) == 0)
                ((CVS_THREAD*)self)->PlayThreadVoice();
        }
    }
}

// When the CVS thread is not blocked, dispatch the vtable+8 slot (the
// battle-main update virtual) through the +0x1C vtable pointer.
void CVS_THREAD_BATTLE_MAIN::dispatchUpdate() {
    if (IsThreadVoiceActive((CVS_THREAD*)this) != 0)
        return;
    ((CVS_THREAD*)this)->PlayThreadVoice();
}

int BmVoice_Cap270() { return 270; }

// Bit-1 gate on +0x3F00 plus the handle's idle-check virtual (vtable 0x2BC),
// then reserve a scratch buffer and play the requested voice through the
// embedded CCharVoice. Any failure returns 0 without playing.
// ── us-802b1bac (BmVoice_FindRatio) ─────────────────────────────────────────────
// Scan the table for the owner's sub-state like BmVoice_FindEquip, then compute
// the ratio of the live pair of float virtuals (slots 0x12C/0x128) over an
// argument-biased denominator, fetch a third float virtual (slot 0x130),
// bucket the ratio against four descending float thresholds (each must also
// exceed the third virtual's value), and return the halfword pooled at
// entry+0x2E+bucket*2.
// Same-TU inline search helper (bmFindEntry pattern): the inlined `return p`
// lowers to retail's branch-over-branch `bne next; b found` split.
static __inline VoiceIdListEntry* bmFindEntryBM(CVoiceHandle* owner) {
    VoiceIdListEntry* p = bmVoiceIdTable;
    int cur;
    while ((cur = p->id) != 0) {
        if (cur == (s32)owner->field_0x3F28)
            return p;
        p++;
    }
    return NULL;
}

extern "C" int BmVoice_FindRatio(CVoiceHandle* owner, int arg) {
    VoiceIdListEntry* p = bmFindEntryBM(owner);
    if (p == NULL)
        return -1;

    BmOwnerFloatVTV* vtv = (BmOwnerFloatVTV*)owner;
    float num = vtv->get12C();
    float den = vtv->get128();
    // Builtin int->float cast: MWCC lowers this to the 0x4330 trick whose
    // final single-rounded fsubs both removes the bias and rounds. The
    // thresholds are plain source literals; MWCC pools them in first-use
    // order, matching the retail .sdata2 layout.
    float ratio = ((float)arg + den) / num;
    // Third virtual is fetched after the ratio; each threshold must also
    // exceed this value for its bucket to win.
    float c = vtv->get130();

    int idx;
    // Guard shape: test1 is a plain inverted `ratio < T` branch (bge);
    // test2 must stay as negated `T <= c` - folding it to `T > c` loses
    // the cror eq,lt,eq fusion MWCC emits for the le-test.
    if (ratio < 0.2f && 0.2f <= c)
        idx = 3;
    else if (ratio < 0.5f && 0.5f <= c)
        idx = 2;
    else if (ratio < 0.8f && 0.8f <= c)
        idx = 1;
    else if (ratio < 0.9f && 0.9f <= c)
        idx = 0;
    else
        idx = -1;

    // Commuted second bound blocks MWCC's unsigned range-check fusion.
    if (idx < 0)
        return -1;
    if (4 <= idx)
        return -1;
    return (&p->valsB[17])[idx];
}

int BmVoice_PlayGated(CVoiceHandle* self, int param, int size) {
    if (!(self->field_0x3F00 & 2))
        return 0;
    if (((CVoiceVTV*)self)->idle() != 0)
        return 0;
    if (CCharVoiceMan_DispatchById(size, 1) == NULL)
        return 0;
    CCharVoice* voicePtr = (CCharVoice*)self;
    if (self != NULL) {
        voicePtr = &self->voice;
    }
    RequestVoicePlayback(voicePtr, param, size);
    return 0;
}

// name-guess: 0x8053A4B8 table: 22 rows keyed by CVoiceHandle::field_0x3F28
// (0 terminates), each with three voice-id pools; consumed by BmVoice_Find*.
VoiceIdListEntry bmVoiceIdTable[24] = {
    { 268, { 211, 201, 202, 203 }, { -1, 205, 206, 205, 206, 205, 206, 205, 206, 205, 206, 205, 206, -1, -1, -1, -1, -1, -1, -1, -1, -1 }, { -1, 0 } },
    { 416, { 211, 201, 202, 203 }, { -1, 206, -1, 207, -1, 205, -1, 208, -1, 209, -1, -1, -1, -1, -1, -1, -1, 212, 213, -1, -1, 214 }, { -1, 0 } },
    { 417, { 211, 201, 202, 203 }, { 203, 205, -1, 206, -1, 205, -1, 208, -1, 207, -1, 207, -1, 213, -1, -1, -1, -1, -1, 212, 204, -1 }, { -1, 0 } },
    { 905, { 211, 201, 202, 203 }, { 204, 206, 208, 206, 208, 207, -1, 205, -1, 209, -1, 210, -1, -1, -1, -1, -1, -1, -1, 212, 213, -1 }, { -1, 0 } },
    { 1205, { 211, 201, 202, 203 }, { 204, 206, 208, 205, -1, 208, -1, 207, -1, 206, -1, -1, -1, -1, -1, -1, -1, -1, 213, 214, 215, 212 }, { -1, 0 } },
    { 1436, { 211, 201, 202, 203 }, { 204, 206, 207, 208, -1, 205, 208, 206, -1, 208, 205, 209, -1, 205, -1, -1, -1, -1, -1, 212, 213, -1 }, { -1, 0 } },
    { 1535, { 211, 201, 202, 203 }, { 204, 209, 207, 205, 208, 206, -1, 209, 211, 212, -1, 209, -1, -1, -1, -1, -1, -1, -1, 214, 215, -1 }, { -1, 0 } },
    { 1622, { 211, 201, 202, 203 }, { -1, 207, -1, 205, -1, 205, -1, 206, -1, 205, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 }, { -1, 0 } },
    { 1914, { 211, 201, 202, 203 }, { -1, 205, -1, 206, -1, 205, -1, 206, -1, 205, 206, 205, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 }, { -1, 0 } },
    { 2115, { 211, 201, 202, 203 }, { -1, 209, 207, 208, 205, 205, 208, 206, -1, 205, -1, 209, 207, -1, -1, -1, -1, -1, -1, -1, -1, -1 }, { -1, 0 } },
    { 2116, { 212, 201, 202, 203 }, { 204, 209, 207, 208, 205, 205, 208, 206, -1, 205, -1, 209, 207, -1, -1, -1, -1, -1, -1, 213, -1, -1 }, { -1, 0 } },
    { 2117, { 211, 201, 202, 203 }, { 204, 208, -1, 209, -1, 205, 206, 207, -1, 208, -1, 208, -1, -1, -1, -1, -1, -1, -1, 212, 213, -1 }, { -1, 0 } },
    { 2123, { 211, 201, 202, 203 }, { -1, 209, -1, 207, 208, 206, -1, 207, 205, 205, 209, 205, -1, -1, -1, -1, -1, 212, -1, -1, -1, -1 }, { -1, 0 } },
    { 2501, { 207, 201, 202, 203 }, { 204, 208, 205, 208, 205, 208, 205, 205, 206, 205, -1, 207, -1, 207, -1, -1, -1, -1, -1, 213, 212, -1 }, { 204, 0 } },
    { 2505, { 211, 201, 202, 203 }, { -1, 208, -1, 208, -1, 206, -1, 205, 207, 205, 207, 206, -1, 206, -1, -1, -1, 212, -1, -1, -1, -1 }, { -1, 0 } },
    { 2309, { 211, 201, 202, 203 }, { 204, 207, -1, 205, -1, 207, -1, 209, 206, 210, -1, 208, -1, -1, -1, -1, -1, -1, 213, 214, 215, -1 }, { -1, 0 } },
    { 1316, { 211, 201, 202, 203 }, { -1, 205, 206, 205, 206, 205, 206, 205, 206, 205, 206, 208, -1, 207, -1, 209, -1, -1, 213, -1, -1, -1 }, { -1, 0 } },
    { 1317, { 212, 201, 202, 203 }, { 204, 209, -1, 206, 205, 208, 207, 207, 206, 210, -1, 207, -1, 208, -1, 209, -1, -1, 213, 214, 215, -1 }, { -1, 0 } },
    { 2411, { 211, 201, 202, 203 }, { -1, 207, -1, -1, -1, -1, -1, 206, -1, -1, -1, -1, -1, 205, -1, -1, -1, -1, -1, -1, 213, -1 }, { 212, 0 } },
    { 2407, { 214, 201, 202, 203 }, { -1, 205, 206, 205, 208, 209, -1, 206, -1, 205, 208, 209, -1, 208, 206, 210, 207, -1, -1, -1, -1, -1 }, { -1, 0 } },
    { 2408, { 211, 201, 202, 203 }, { 204, 205, 206, 205, 208, 209, -1, 206, -1, 205, 208, 209, -1, 208, 206, 210, 207, -1, 212, 213, 214, -1 }, { -1, 0 } },
    { 1619, { -1, 201, 202, 203 }, { -1, 211, 213, 211, 213, 204, -1, 216, 213, -1, -1, -1, -1, -1, -1, -1, -1, 216, 215, -1, -1, -1 }, { 216, 0 } },
    { 0, { 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0 } },
    { 0, { 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0 } },
};
