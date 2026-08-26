#pragma once

#include "kyoshin/cf/voice/cvsys/CVS_THREAD.hpp"

// C-linkage imports from sibling voice TUs (retail symbol names - keep
// linkage/signatures verbatim).
extern "C" {
    int  func_802A3E88(CVS_THREAD* self);
    void func_802A3BEC(CVS_THREAD* self, CCharVoice* voicePtr);
    int  func_802A3C44(CVS_THREAD* self, CCharVoice* voicePtr, int voiceId);
    int  func_802A3D54(CCharVoice* voicePtr, int voiceId, int arg);
    CVoiceHandle* func_802A330C(int size, int align);

    // Ring-buffer allocator from CCharVoiceMan.
    u8* func_802A34E4(int size);

    // Base-class constructor for CVS_THREAD (retail mangled
    // __ct__cf_CVS_THREAD).
    void __ct__cf_CVS_THREAD(void* self);
}

// Circular voice-node list returned by the voice manager func_800B6BA4().
// Each node's field_8 points at the CCharVoice embedded in the handle
// (i.e. handle + 0x3E9C). Same layout family as CVS_THREAD_BATTLE_END_SP.hpp.
struct CVoiceListNode {
    CVoiceListNode* field_0;  // 0x00: next node
    CVoiceListNode* field_4;  // 0x04: previous node
    CCharVoice* field_8;      // 0x08: &handle->voice
};

struct CVoiceManager {
    void* field_0;              // 0x00
    CVoiceListNode* field_4;    // 0x04: circular-list sentinel
};

// C++-linkage voice-manager query (retail mangled name func_800B6BA4__Fv).
CVoiceManager* func_800B6BA4();

// Phantom vtable view over a CVoiceHandle vtable (vtable pointer at +0x00) so
// the idle-check slot at byte offset 0x2BC (index 175) can be dispatched as a
// real r12-chained virtual call matching retail. Only cast + call a slot, so
// no vtable is emitted in this TU.
struct CVoiceVTV {
#define CVTV_N(n) virtual void s##n();
    CVTV_N(0) CVTV_N(1) CVTV_N(2) CVTV_N(3) CVTV_N(4) CVTV_N(5) CVTV_N(6) CVTV_N(7) CVTV_N(8) CVTV_N(9)
    CVTV_N(10) CVTV_N(11) CVTV_N(12) CVTV_N(13) CVTV_N(14) CVTV_N(15) CVTV_N(16) CVTV_N(17) CVTV_N(18) CVTV_N(19)
    CVTV_N(20) CVTV_N(21) CVTV_N(22) CVTV_N(23) CVTV_N(24) CVTV_N(25) CVTV_N(26) CVTV_N(27) CVTV_N(28) CVTV_N(29)
    CVTV_N(30) CVTV_N(31) CVTV_N(32) CVTV_N(33) CVTV_N(34) CVTV_N(35) CVTV_N(36) CVTV_N(37) CVTV_N(38) CVTV_N(39)
    CVTV_N(40) CVTV_N(41) CVTV_N(42) CVTV_N(43) CVTV_N(44) CVTV_N(45) CVTV_N(46) CVTV_N(47) CVTV_N(48) CVTV_N(49)
    CVTV_N(50) CVTV_N(51) CVTV_N(52) CVTV_N(53) CVTV_N(54) CVTV_N(55) CVTV_N(56) CVTV_N(57) CVTV_N(58) CVTV_N(59)
    CVTV_N(60) CVTV_N(61) CVTV_N(62) CVTV_N(63) CVTV_N(64) CVTV_N(65) CVTV_N(66) CVTV_N(67) CVTV_N(68) CVTV_N(69)
    CVTV_N(70) CVTV_N(71)
    virtual float s72();  // slot 72 = vtable offset 0x128: HP-ratio numerator
    virtual float s73();  // slot 73 = vtable offset 0x12C: HP-ratio denominator
    CVTV_N(74) CVTV_N(75) CVTV_N(76) CVTV_N(77) CVTV_N(78) CVTV_N(79)
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
    virtual int idle();
};

// Tail view of a CVoiceHandle covering the voice sub-state index at +0x3F28
// (same layout family as CScenarioLogOwner::field_0x3F28; the base header's
// CVoiceHandle reaches +0x3F60 only as opaque padding).
struct CVoiceHandleState {
    u8 _00[0x3F28];
    u16 field_0x3F28;  // 0x3F28: sub-state index (8 -> base 3)
};

// Init-state triple for this thread variant.
extern u32 lbl_eu_80539C04[3];

// Init-state triple restored by the advance/play function func_802A7508.
extern u32 lbl_eu_80539C10[3];

// Init-state triple copied by the factory __ct__802A7254.
extern u32 lbl_eu_80539BF8[3];

// CVS_THREAD_HP vtable (stored at offset 0x1C by the factory).
extern u32 lbl_eu_80539C1C[7];

// HP-ratio thresholds used by the factory to pick the HP-change voice ID.
extern f32 lbl_eu_80668CA0;
extern f32 lbl_eu_80668CA4;

// Randomly pick one idle voice handle from the voice-manager circular list,
// excluding `exclude` (NULL when none available). Retail symbol is the plain
// unmangled name, so keep C linkage.
extern "C" CVoiceHandle* func_802A7998(CVoiceHandle* exclude);



/**
 * CVS_THREAD_HP - voice thread for HP-change audio events.
 *
 * Inherits from CVS_THREAD and returns a smaller thread-local buffer
 * (210 bytes) compared to the BUF variant (260 bytes).
 */
class CVS_THREAD_HP : public CVS_THREAD {
public:
    // Size of the thread-local voice buffer in bytes.
    static const int BUFFER_SIZE = 0xD2;

    CVoiceHandle* field_0x20;  // 0x20: voice handle pointer (slot 1)
    int field_0x24;            // 0x24: voice ID for HP-change playback

    int blank1() override;
};
