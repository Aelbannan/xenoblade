#pragma once

#include "kyoshin/cf/voice/cvsys/CVS_THREAD.hpp"

namespace cf {

/**
 * CVS_THREAD_CHAIN - voice thread chain node.
 *
 * Inherits from CVS_THREAD and adds a link pointer (field_0x20) for chaining
 * multiple voice threads together (e.g. multi-part battle voices).
 * Object size: 0x24 (36 bytes).
 */
class CVS_THREAD_CHAIN : public CVS_THREAD {
public:
    CVoiceHandle* field_0x20;  // 0x20: voice handle slot

    int func_802A5A0C() { return 1; }
};

} // namespace cf

// C-linkage imports used by this TU's thread helpers / factory. Kept here
// (not in the .cpp) because they are genuine retail C/ABI symbols imported
// from sibling voice modules.
extern "C" {
    int           func_802A3E88(CVS_THREAD* self);
    void          func_802A3BEC(CVS_THREAD* self, CCharVoice* voicePtr);
    int           func_802A3C44(CVS_THREAD* self, CCharVoice* voicePtr, int voiceId);
    void          func_802A3E28(CVS_THREAD* self);
    int           func_802A3D54(CCharVoice* voicePtr, int voiceId, int arg);
    CVoiceHandle* func_802A330C(int size, int align);
    u8*           func_802A34E4(int size);
    int           func_800BE8F4(CCharVoice* voicePtr);
    void          __ct__cf_CVS_THREAD();
    // Runtime rethrow (NMWException.h): declared noreturn so MWCC elides the
    // __end__catch epilogue of a catch-all handler that ends with `bl __throw`
    // (retail catch-all handlers end at the rethrow).
    __declspec(noreturn) void __throw(char* throwtype, void* location,
                                      void* dtor);
}

// Globals: init-state triple and this subclass's vtable.
extern u32 lbl_eu_80539A30[3];
extern u32 lbl_eu_80539A3C[7];

// Named view over the CVS_THREAD init-state triple (bytes 0x00-0x08), which
// the factory copies wholesale from lbl_eu_80539A30. Same layout as the first
// three CVS_THREAD words; only used to avoid anonymous-field access here.
struct CVS_THREAD_CHAIN_INIT {
    u32* word0;  // 0x00
    u32 word1;   // 0x04
    u32 word2;   // 0x08
};

// Phantom vtable view over a CVoiceHandle vtable (vtable pointer at +0x00) so
// the is-active slot at byte offset 0x2BC (index 175) can be dispatched as a
// real r12-chained virtual call matching retail. Only cast + call a slot, so
// no vtable is emitted in this TU.
struct CVoiceChainVTV {
#define CHAIN_VTV_N(n) virtual void s##n();
    CHAIN_VTV_N(0) CHAIN_VTV_N(1) CHAIN_VTV_N(2) CHAIN_VTV_N(3) CHAIN_VTV_N(4) CHAIN_VTV_N(5) CHAIN_VTV_N(6) CHAIN_VTV_N(7) CHAIN_VTV_N(8) CHAIN_VTV_N(9)
    CHAIN_VTV_N(10) CHAIN_VTV_N(11) CHAIN_VTV_N(12) CHAIN_VTV_N(13) CHAIN_VTV_N(14) CHAIN_VTV_N(15) CHAIN_VTV_N(16) CHAIN_VTV_N(17) CHAIN_VTV_N(18) CHAIN_VTV_N(19)
    CHAIN_VTV_N(20) CHAIN_VTV_N(21) CHAIN_VTV_N(22) CHAIN_VTV_N(23) CHAIN_VTV_N(24) CHAIN_VTV_N(25) CHAIN_VTV_N(26) CHAIN_VTV_N(27) CHAIN_VTV_N(28) CHAIN_VTV_N(29)
    CHAIN_VTV_N(30) CHAIN_VTV_N(31) CHAIN_VTV_N(32) CHAIN_VTV_N(33) CHAIN_VTV_N(34) CHAIN_VTV_N(35) CHAIN_VTV_N(36) CHAIN_VTV_N(37) CHAIN_VTV_N(38) CHAIN_VTV_N(39)
    CHAIN_VTV_N(40) CHAIN_VTV_N(41) CHAIN_VTV_N(42) CHAIN_VTV_N(43) CHAIN_VTV_N(44) CHAIN_VTV_N(45) CHAIN_VTV_N(46) CHAIN_VTV_N(47) CHAIN_VTV_N(48) CHAIN_VTV_N(49)
    CHAIN_VTV_N(50) CHAIN_VTV_N(51) CHAIN_VTV_N(52) CHAIN_VTV_N(53) CHAIN_VTV_N(54) CHAIN_VTV_N(55) CHAIN_VTV_N(56) CHAIN_VTV_N(57) CHAIN_VTV_N(58) CHAIN_VTV_N(59)
    CHAIN_VTV_N(60) CHAIN_VTV_N(61) CHAIN_VTV_N(62) CHAIN_VTV_N(63) CHAIN_VTV_N(64) CHAIN_VTV_N(65) CHAIN_VTV_N(66) CHAIN_VTV_N(67) CHAIN_VTV_N(68) CHAIN_VTV_N(69)
    CHAIN_VTV_N(70) CHAIN_VTV_N(71) CHAIN_VTV_N(72) CHAIN_VTV_N(73) CHAIN_VTV_N(74) CHAIN_VTV_N(75) CHAIN_VTV_N(76) CHAIN_VTV_N(77) CHAIN_VTV_N(78) CHAIN_VTV_N(79)
    CHAIN_VTV_N(80) CHAIN_VTV_N(81) CHAIN_VTV_N(82) CHAIN_VTV_N(83) CHAIN_VTV_N(84) CHAIN_VTV_N(85) CHAIN_VTV_N(86) CHAIN_VTV_N(87) CHAIN_VTV_N(88) CHAIN_VTV_N(89)
    CHAIN_VTV_N(90) CHAIN_VTV_N(91) CHAIN_VTV_N(92) CHAIN_VTV_N(93) CHAIN_VTV_N(94) CHAIN_VTV_N(95) CHAIN_VTV_N(96) CHAIN_VTV_N(97) CHAIN_VTV_N(98) CHAIN_VTV_N(99)
    CHAIN_VTV_N(100) CHAIN_VTV_N(101) CHAIN_VTV_N(102) CHAIN_VTV_N(103) CHAIN_VTV_N(104) CHAIN_VTV_N(105) CHAIN_VTV_N(106) CHAIN_VTV_N(107) CHAIN_VTV_N(108) CHAIN_VTV_N(109)
    CHAIN_VTV_N(110) CHAIN_VTV_N(111) CHAIN_VTV_N(112) CHAIN_VTV_N(113) CHAIN_VTV_N(114) CHAIN_VTV_N(115) CHAIN_VTV_N(116) CHAIN_VTV_N(117) CHAIN_VTV_N(118) CHAIN_VTV_N(119)
    CHAIN_VTV_N(120) CHAIN_VTV_N(121) CHAIN_VTV_N(122) CHAIN_VTV_N(123) CHAIN_VTV_N(124) CHAIN_VTV_N(125) CHAIN_VTV_N(126) CHAIN_VTV_N(127) CHAIN_VTV_N(128) CHAIN_VTV_N(129)
    CHAIN_VTV_N(130) CHAIN_VTV_N(131) CHAIN_VTV_N(132) CHAIN_VTV_N(133) CHAIN_VTV_N(134) CHAIN_VTV_N(135) CHAIN_VTV_N(136) CHAIN_VTV_N(137) CHAIN_VTV_N(138) CHAIN_VTV_N(139)
    CHAIN_VTV_N(140) CHAIN_VTV_N(141) CHAIN_VTV_N(142) CHAIN_VTV_N(143) CHAIN_VTV_N(144) CHAIN_VTV_N(145) CHAIN_VTV_N(146) CHAIN_VTV_N(147) CHAIN_VTV_N(148) CHAIN_VTV_N(149)
    CHAIN_VTV_N(150) CHAIN_VTV_N(151) CHAIN_VTV_N(152) CHAIN_VTV_N(153) CHAIN_VTV_N(154) CHAIN_VTV_N(155) CHAIN_VTV_N(156) CHAIN_VTV_N(157) CHAIN_VTV_N(158) CHAIN_VTV_N(159)
    CHAIN_VTV_N(160) CHAIN_VTV_N(161) CHAIN_VTV_N(162) CHAIN_VTV_N(163) CHAIN_VTV_N(164) CHAIN_VTV_N(165) CHAIN_VTV_N(166) CHAIN_VTV_N(167) CHAIN_VTV_N(168) CHAIN_VTV_N(169)
    CHAIN_VTV_N(170) CHAIN_VTV_N(171) CHAIN_VTV_N(172)
#undef CHAIN_VTV_N
    virtual int isActive();  // slot 173 @ 0x2BC: handle is-active check
};