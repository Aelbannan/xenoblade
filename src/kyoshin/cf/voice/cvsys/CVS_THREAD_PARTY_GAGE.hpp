#pragma once

#include "kyoshin/cf/voice/cvsys/CVS_THREAD.hpp"

/**
 * CVS_THREAD_PARTY_GAGE - voice thread for party gauge change events.
 *
 * Inherits from CVS_THREAD and allocates a 0x8C (140) byte buffer
 * for party gauge voice processing.
 */
class CVS_THREAD_PARTY_GAGE : public CVS_THREAD {
public:
    // Size of the thread-local voice buffer in bytes.
    static const int BUFFER_SIZE = 0x8C;

    // Voice handle for the party member character data (offset 0x20).
    CVoiceHandle* partyMember;

    // Voice handle for associated gauge data (offset 0x24).
    CVoiceHandle* gaugeData;

    // Gauge change threshold level (0=low, 1=mid, 2=high) (offset 0x28).
    u32 thresholdLevel;
};

// Normalized retail symbol names imported from sibling voice modules
// (unmangled C-ABI symbol names, declared like in every other cvsys header).
extern "C" {
    int           func_802A3E88(CVS_THREAD* self);
    void          func_802A3BEC(CVS_THREAD* self, CCharVoice* voicePtr);
    int           func_802A3C44(CVS_THREAD* self, CCharVoice* voicePtr, int voiceId);
    int           func_802A3D54(CCharVoice* voicePtr, int voiceId, int arg);
    CVoiceHandle* func_802A330C(int size, int align);
    u8*           func_802A34E4(int size);
    int           func_802A790C(CVoiceHandle* exclude);
    CVoiceHandle* func_802A7998(CVoiceHandle* exclude);
    int           func_802A7DF8(CVoiceHandle* handle);
    int           func_802A77E8(CVoiceHandle* handle);
    int           func_802A7850(int iter);
    int           func_802A7B90(CVoiceHandle* handle1, CVoiceHandle* handle2);
    int           func_802A7CC4(CVoiceHandle* handle1, CVoiceHandle* handle2);
    void          __ct__cf_CVS_THREAD();
    // Runtime rethrow (NMWException.h): declared noreturn so MWCC elides the
    // __end__catch epilogue of a catch-all handler that ends with `bl __throw`
    // (retail catch-all handlers end at the rethrow).
    __declspec(noreturn) void __throw(char* throwtype, void* location,
                                      void* dtor);
}

// Init state triples and this subclass's vtable.
extern u32 lbl_eu_80539C48[3];
extern u32 lbl_eu_80539C54[3];
extern u32 lbl_eu_80539C60[3];
extern u32 lbl_eu_80539C6C[7];
extern u32 lbl_eu_80662D18[2];

// Phantom vtable view over a CVS_THREAD vtable pointer (stored at +0x1C) so
// slot-2 (offset 0x08) dispatch is emitted as a real r12-chained virtual
// call. CVS_THREAD's vtable is retail data (never constructed from C++), so
// we only cast + call a slot and no vtable is emitted in this TU.
struct CVSThreadSlot2VTV {
    virtual void slot0();
    virtual void slot1();
    virtual int  slot2();
};

// Phantom vtable view over a CVoiceHandle vtable (vtable pointer at +0x00) so
// the idle-check slot at byte offset 0x2BC (index 175) can be dispatched as a
// real r12-chained virtual call matching retail.  Only cast + call a slot, so
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
    virtual int idle();
};
