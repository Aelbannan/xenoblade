// Voice thread state machines for the battle-main voice system.
#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/CfMapItemManager.hpp"
#include "kyoshin/cf/voice/cvsys/CVS_THREAD.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "monolib/math/Random.hpp"

// C-linkage imports from sibling modules (no compatible declaring header in
// scope; mirrors CVS_THREAD_CHAIN.hpp).  Declared here instead of via
// code_802B8A3C.hpp so the base-ctor import carries its self argument
// (retail passes r3=self).
extern "C" {
    int  func_802A3E88(CVS_THREAD* self);
    int  func_802A3C44(CVS_THREAD* self, CCharVoice* voicePtr, int voiceId);
    int  func_802A3D54(CCharVoice* voicePtr, int voiceId, int arg);
    void __ct__cf_CVS_THREAD(void* self);
    int  func_802A77E8(CVoiceHandle* handle);
    void* func_802A34E4(int size);
    // Runtime rethrow (NMWException.h): noreturn so MWCC elides the
    // __end__catch epilogue of the catch-all handler.
    __declspec(noreturn) void __throw(char* throwtype, u32 location,
                                      u32 dtor);
}

// ── Shared views over the voice-owner object ─────────────────────────────

// View over the voice-owner object covering the manager flags at +0x3F00
// (bit 2 gates battle-main playback) and the sub-state id at +0x3F28.
struct BattleMainOwnerView {
    u8 _pad[0x3F00];
    u32 field_0x3F00;                 // 0x3F00
    u8 _pad2[0x3F28 - 0x3F04];
    u16 field_0x3F28;                 // 0x3F28: id matched against the table
    u8 _pad4[0x3F60 - 0x3F2A];
    UnkTarget* unkTarget;             // 0x3F60: ->0x08 ->0x18 state chain
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

// Sibling voice-id selector implemented below; the one-argument view is
// what retail func_802AF43C calls it through.

int func_802AF02C(BattleMainOwnerView* owner);
// Retail exports this selector as an unmangled symbol, so it keeps C linkage
// (callers' bl relocs target the plain name).
extern "C" int func_802AF13C(BattleMainOwnerView* owner, int arg);

// Import without a declaring header (compile-only scaffolding).
extern "C" CVoiceHandle* func_802A7998(CVoiceHandle* handle);

// Word-wise overlay over the base thread's 12-byte handler ptmf (offsets
// 0x00-0x0B): each variant's init tables write three words that together
// form the member-function pointer.
struct BmStateTriple {
    u32 unk0;  // +0x00
    u32 unk4;  // +0x04
    u32 unk8;  // +0x08
};

// Init-state triples and this subclass's vtable.
extern u32 lbl_eu_8053AA58[3];
extern u32 lbl_eu_8053AA64[3];
extern u32 lbl_eu_8053AA70[3];
extern u32 lbl_eu_8053AA7C[7];

// Float thresholds for func_802AF13C, plus the int->float conversion
// magic (0x4330000080000000): planting it under its retail name makes
// MWCC's literal pool unify the builtin cast's pooled magic onto this
// label (ocUnit::turn pattern).
const double lbl_eu_80668EB8 = 4503601774854144.0;
extern float lbl_eu_80668EA8;
extern float lbl_eu_80668EAC;
extern float lbl_eu_80668EB0;
extern float lbl_eu_80668EB4;

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
};

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
struct BmHandleSubView {
    u8 _pad[4];
    BmSubVTV* field_0x04;    // +0x04: sub-object carrying the result slot
};

int func_802AED0C(BattleMainOwnerView* owner);
int func_802AEF80(BattleMainOwnerView* owner);
// C-linkage so the retail unmangled symbol func_802AF2DC is referenced.
extern "C" int func_802AF2DC(BattleMainOwnerView* owner);
int func_802AF388(BattleMainOwnerView* owner);
int func_802AF3DC(CVoiceHandle* self);
int func_802AF43C(CVoiceHandle* self);
int func_802AF49C(CVoiceHandle* self);
int func_802AF4FC(CVoiceHandle* self, int param);
int func_802AF56C(CVoiceHandle* self);
int func_802AF9D0(CVoiceHandle* self, int param, int size);

// Scan the 60-stride list at lbl_eu_8053A4B8 for the entry whose id matches
// the owner's +0x3F28 sub-state, then draw a random voice from pool A.
// Goto shapes pin MWCC's branch layout: two-way exit at 'top' (bne/b) and a
// pre-tested walk loop whose counter is tested before each pool halfword.
int func_802AED0C(BattleMainOwnerView* owner) {
    VoiceIdListEntry* e = lbl_eu_8053A4B8;
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
// Defined BEFORE func_802AEF80 so MWCC cannot see the callee body and
// inline it (retail keeps the bl).
int func_802AF49C(CVoiceHandle* self) {
    if (!(self->field_0x3F00 & 4))
        return 0;
    CCharVoice* voicePtr = (CCharVoice*)self;
    if (self != NULL) {
        voicePtr = &self->voice;
    }
    // Block-scope extern declaration hides the in-TU body from MWCC's
    // -O4 inliner so the call stays a bl (matches retail).
    extern int func_802AEF80(BattleMainOwnerView* owner);
    int voiceId = func_802AEF80((BattleMainOwnerView*)self);
    func_802A3D54(voicePtr, voiceId, 0xA);
    return 0;
}

// __declspec(noinline): retail keeps this selector outlined (callers emit
// bl); without it MWCC's -O4 inliner copies the body into func_802AF49C.
// Same goto-shaped body as func_802AED0C (pool B at +0x0C); inlining the
// walk here instead of via the helper keeps MWCC's register colours equal
// to retail.
__declspec(noinline) int func_802AEF80(BattleMainOwnerView* owner) {
    VoiceIdListEntry* e = lbl_eu_8053A4B8;
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

// Same scan as func_802AED0C, drawing from pool C.
// __declspec(noinline): retail keeps this selector outlined; without it
// MWCC's -O4 inliner copies the body into func_802AF56C.
// __declspec(noinline): retail keeps this selector outlined; without it
// MWCC's -O4 inliner copies the body into func_802AF56C.
// Same goto-shaped body as func_802AED0C (pool C at +0x38).
__declspec(noinline) int func_802AF2DC(BattleMainOwnerView* owner) {
    VoiceIdListEntry* e = lbl_eu_8053A4B8;
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

// ── us-802b1a9c (func_802AF02C) ─────────────────────────────────────────────
// Scan the 60-stride list at lbl_eu_8053A4B8 for the entry matching the
// owner's +0x3F28 sub-state, validate the 0x3F60 -> 0x08 -> 0x18 chain
// (value must lie in [0x10,0x18]), fetch the equipment-data pool selected
// by the work object's byte at +0x77, and draw a random halfword from the
// leading run of positive entries (capped at 2).
// Same-TU inline search helper (btm_bda_to_acl pattern): the inlined
// `return p` lowers to retail's branch-over-branch `bne next; b found`
// split, which every direct loop form folds into a single beq.
static __inline VoiceIdListEntry* bmFindEntry(BattleMainOwnerView* owner) {
    VoiceIdListEntry* p = lbl_eu_8053A4B8;
    int cur;
    while ((cur = p->id) != 0) {
        if (cur == (s32)owner->field_0x3F28)
            return p;
        p++;
    }
    return NULL;
}

int func_802AF02C(BattleMainOwnerView* owner) {
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
    return *(s16*)((char*)p + ml::math::mtRand(n) * 2);
}

// Bit-2 gate on +0x3F00, then scan the 60-stride list at lbl_eu_8053A4B8 for
// an entry matching the +0x3F28 id; returns whether a match was found.
int func_802AF388(BattleMainOwnerView* self) {
    if (!(self->field_0x3F00 & 4))
        return 0;
    // Scan until the 0 terminator; on a match keep the entry pointer, else
    // clear it so the final boolean reports whether anything matched.
    // The +0x3F28 halfword is reloaded every iteration (retail lhz inside
    // the loop), so the match test sits in the loop body.
    const VoiceIdListEntry* p = lbl_eu_8053A4B8;
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
// Declared above with one argument on purpose: retail calls it passing
// only r3 (the full three-parameter definition appears further below).
int func_802AF43C(CVoiceHandle* self) {
    // Retail calls the three-parameter selector passing only r3; mirror that
    // with a narrow local declaration so no r4/r5 setup is emitted.
    extern int func_802AEDB8(BattleMainOwnerView* owner);
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

// ── us-802b1828 (func_802AEDB8) ─────────────────────────────────────────────
// Full retail signature (thread state, voice handle, sub-state id).
// Scans the 60-stride id table for the thread's +0x3F28 sub-state; on a hit,
// draws a random voice id from the pool selected by the handle's battle
// state. When the handle gate (flag bit 1, state 1, work state 0x10-0x17)
// passes it probes pool +0x36 (cap 1) and only accepts a positive draw,
// otherwise it falls through to the id-based pools (+0xA cap 1 for nonzero
// id, +6 cap 2 for zero id).
extern "C" int func_802AEDB8(BattleMainOwnerView* thread,
                             BattleMainOwnerView* handle, int id) {
    // Locate the table row whose id equals the thread's +0x3F28 sub-state.
    // Declaration order (table, id copy, handle copy) drives MWCC's prologue
    // scheduling; the shared `cur` keeps the row word in one register for
    // both the terminator test and the match compare.
    VoiceIdListEntry* e = lbl_eu_8053A4B8;
    int cur;
    goto check;
top:
    if (cur == thread->field_0x3F28)
        goto found;
    e++;
check:
    cur = e->id;
    if (cur != 0)
        goto top;
    e = NULL;
found:
    if (e == NULL)
        return -1;

    // Gate: flag bit 1, battle state 1, work state within [0x10,0x17].
    int gated = 0;
    if (handle->field_0x3F00 & 2) {
        if (func_802A77E8((CVoiceHandle*)handle) == 1) {
            UnkTargetInner* w = handle->unkTarget->field_0x08;
            gated = (w->field_0x18 >= 0x10 && w->field_0x18 <= 0x17);
        }
    }

    if (gated != 0) {
        // Walk the leading positive run of pool +0x36 (one step max).
        char* c = (char*)e;
        int i = 0;
        while (i < 1 && *(s16*)(c + 0x36) > 0) {
            c += 2;
            i++;
        }
        s16 pick =
            *(s16*)((char*)e + ml::math::mtRand(i < 1 ? 1 : i) * 2 + 0x36);
        // Only accept a positive draw; otherwise fall through to the pools.
        if (pick > 0)
            return pick;
    }

    if (id != 0) {
        char* c = (char*)e;
        int i = 0;
        while (i < 1 && *(s16*)(c + 0xA) > 0) {
            c += 2;
            i++;
        }
        return *(s16*)((char*)e + ml::math::mtRand(i < 1 ? 1 : i) * 2 + 0xA);
    }

    char* c = (char*)e;
    int i = 0;
    while (i < 2 && *(s16*)(c + 6) > 0) {
        c += 2;
        i++;
    }
    return *(s16*)((char*)e + ml::math::mtRand(i < 1 ? 1 : i) * 2 + 6);
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
    // Retail forwards the request parameter into the selector unchanged.
    int voiceId = func_802AF13C((BattleMainOwnerView*)self, param);
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

// ── us-802b203c (__ct__802AF5CC) ────────────────────────────────────────────
// Factory/constructor for the battle-main voice thread. Registers against
// the player's voice handle (embedded voice at +0x3E9C), allocates the
// scratch handle (0x10E) plus the 0x24-byte thread, constructs it with the
// battle-main vtable and the caller's id, then seeds the init-state triple.
// Retail returns the stack-frame record address.
void* __ct__802AF5CC(int arg) {
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
        ((BmHandleSubView*)handle)->field_0x04->getResult()->word0;
    if (func_80174C98(handle, &captured, 0x803) == 0)
        return 0;

    // Idle check via the handle's vtable slot at 0x2BC.
    if (((CVoiceVTV*)handle)->idle() != 0)
        return 0;

    // Scratch allocation (discarded) then the thread object itself.
    if (func_802A330C(0x10e, 1) == NULL)
        return 0;
    CVS_THREAD_BATTLE_MAIN* self =
        (CVS_THREAD_BATTLE_MAIN*)(u32)func_802A34E4(0x24);
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
    self->unk0 = *(const VoiceCb*)lbl_eu_8053AA58;

    return self;
}

// ── us-802b2194 (func_802AF724) ─────────────────────────────────────────────
// Periodic battle voice refresh: restore the init-state triple, and while
// the player's voice handle reports idle, play a random line (range picked
// by the thread's flag byte) through the embedded voice.
void func_802AF724(CVS_THREAD_BATTLE_MAIN* self) {
    // Restore the base state triple via the lwzu/spread load-with-update
    // pattern (v0 declared first so the lwzu destination colours low).
    BmStateTriple* st = (BmStateTriple*)&self->unk0;
    u32 v0;
    const u32* src = lbl_eu_8053AA64;
    v0 = *src++;
    st->unk4 = *src++;
    st->unk0 = v0;
    st->unk8 = *src;

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
            if (func_802A3C44((CVS_THREAD*)self, v, ml::math::mtRand(2) + 0x51B) == 0)
                ((CVS_THREAD*)self)->func_802A3B50();
        } else {
            CCharVoice* v = (CCharVoice*)handle;
            if (handle != NULL) {
                v = &handle->voice;
            }
            if (func_802A3C44((CVS_THREAD*)self, v, ml::math::mtRand(2) + 0x519) == 0)
                ((CVS_THREAD*)self)->func_802A3B50();
        }
    }
}

// ── us-802b22b4 (func_802AF844) ─────────────────────────────────────────────
// Same refresh shape as func_802AF724 but gated on the thread-blocked check
// and an extra handle remap through func_802A7998 before the idle probe.
void func_802AF844(CVS_THREAD_BATTLE_MAIN* self) {
    if (func_802A3E88((CVS_THREAD*)self) != 0)
        return;

    BmStateTriple* st = (BmStateTriple*)&self->unk0;
    u32 v0;
    const u32* src = lbl_eu_8053AA70;
    v0 = *src++;
    st->unk4 = *src++;
    st->unk0 = v0;
    st->unk8 = *src;

    CCharVoice* voice = (CCharVoice*)cf::CfGameManager::getPlayer(0);
    CVoiceHandle* handle = (CVoiceHandle*)voice;
    if (voice != NULL) {
        handle = (CVoiceHandle*)((u8*)voice - 0x3E9C);
    }
    // Remap the handle, requiring both the input and the result non-null.
    CVoiceHandle* mapped = func_802A7998(handle);
    if (handle != NULL && mapped != NULL && ((CVoiceVTV*)mapped)->idle() == 0) {
        if (self->field_0x20 != 0) {
            CCharVoice* v = (CCharVoice*)mapped;
            if (mapped != NULL) {
                v = &mapped->voice;
            }
            if (func_802A3C44((CVS_THREAD*)self, v, ml::math::mtRand(2) + 0x523) == 0)
                ((CVS_THREAD*)self)->func_802A3B50();
        } else {
            CCharVoice* v = (CCharVoice*)mapped;
            if (mapped != NULL) {
                v = &mapped->voice;
            }
            if (func_802A3C44((CVS_THREAD*)self, v, ml::math::mtRand(2) + 0x521) == 0)
                ((CVS_THREAD*)self)->func_802A3B50();
        }
    }
}

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
// ── us-802b1bac (func_802AF13C) ─────────────────────────────────────────────
// Scan the table for the owner's sub-state like func_802AF02C, then compute
// the ratio of the live pair of float virtuals (slots 0x12C/0x128) over an
// argument-biased denominator, fetch a third float virtual (slot 0x130),
// bucket the ratio against four descending float thresholds (each must also
// exceed the third virtual's value), and return the halfword pooled at
// entry+0x2E+bucket*2.
// Same-TU inline search helper (bmFindEntry pattern): the inlined `return p`
// lowers to retail's branch-over-branch `bne next; b found` split.
static __inline VoiceIdListEntry* bmFindEntryBM(BattleMainOwnerView* owner) {
    VoiceIdListEntry* p = lbl_eu_8053A4B8;
    int cur;
    while ((cur = p->id) != 0) {
        if (cur == (s32)owner->field_0x3F28)
            return p;
        p++;
    }
    return NULL;
}

extern "C" int func_802AF13C(BattleMainOwnerView* owner, int arg) {
    VoiceIdListEntry* p = bmFindEntryBM(owner);
    if (p == NULL)
        return -1;

    BmOwnerFloatVTV* vtv = (BmOwnerFloatVTV*)owner;
    float num = vtv->get12C();
    float den = vtv->get128();
    // Builtin int->float cast: MWCC lowers this to the 0x4330 trick whose
    // final single-rounded fsubs both removes the bias and rounds; the
    // pooled magic constant aliases the shared blob lbl_eu_80668EB8.
    float ratio = ((float)arg + den) / num;
    // Third virtual is fetched after the ratio; each threshold must also
    // exceed this value for its bucket to win.
    float c = vtv->get130();

    int idx;
    // Guard shape: test1 is a plain inverted `ratio < T` branch (bge);
    // test2 must stay as negated `T <= c` - folding it to `T > c` loses
    // the cror eq,lt,eq fusion MWCC emits for the le-test.
    if (ratio < lbl_eu_80668EA8 && lbl_eu_80668EA8 <= c)
        idx = 3;
    else if (ratio < lbl_eu_80668EAC && lbl_eu_80668EAC <= c)
        idx = 2;
    else if (ratio < lbl_eu_80668EB0 && lbl_eu_80668EB0 <= c)
        idx = 1;
    else if (ratio < lbl_eu_80668EB4 && lbl_eu_80668EB4 <= c)
        idx = 0;
    else
        idx = -1;

    // Commuted second bound blocks MWCC's unsigned range-check fusion.
    if (idx < 0)
        return -1;
    if (4 <= idx)
        return -1;
    return *(s16*)((char*)p + idx * 2 + 0x2E);
}

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
