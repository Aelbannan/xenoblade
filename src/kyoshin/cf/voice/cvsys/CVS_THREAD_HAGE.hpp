#pragma once

#include <types.h>
#include "kyoshin/cf/voice/cvsys/CVS_THREAD.hpp"
#include "kyoshin/cf/voice/CCharVoice.hpp"

// Forward declarations for the C-linkage imports below.
class CVS_THREAD_HAGE;

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" {
    int func_802A3E88(CVS_THREAD* self);
    void func_802A3BEC(CVS_THREAD* self, CCharVoice* voicePtr);
    int func_802A3C44(CVS_THREAD* self, CCharVoice* voicePtr, int voiceId);
    int func_802A77E8(CVoiceHandle* handle);
    int func_802A7850(int iter);
    int func_802A7B90(CVoiceHandle* handle1, CVoiceHandle* handle2);
    CVS_THREAD_HAGE* func_802A330C(int size, int align);
    u8* func_802A34E4(int size);
    void __ct__cf_CVS_THREAD();

    // Runtime rethrow (NMWException.h): declared noreturn so MWCC elides the
    // __end__catch epilogue of a catch-all handler that ends with `bl __throw`
    // (retail catch-all handlers end at the rethrow).
    __declspec(noreturn) void __throw(char* throwtype, void* location,
                                      void* dtor);
}

// Phantom vtable view over CVoiceHandle so the idle-check slot at byte
// offset 0x2BC is dispatched as a true r12-chained virtual call
// (`lwz r12,0(rS); lwz r12,0x2BC(r12); mtctr; bctrl`) matching retail.
// Never instantiated (only cast + slot call), so no vtable is emitted.
// MWCC places declared slot P at vtable offset (P+2)*4, so declared slot
// 173 lands at 0x2BC (is-active check).
struct CVoiceHandleVt {
#define CVTH_PAD(n) virtual void v##n();
    CVTH_PAD(0) CVTH_PAD(1) CVTH_PAD(2) CVTH_PAD(3) CVTH_PAD(4) CVTH_PAD(5)
    CVTH_PAD(6) CVTH_PAD(7) CVTH_PAD(8) CVTH_PAD(9) CVTH_PAD(10) CVTH_PAD(11)
    CVTH_PAD(12) CVTH_PAD(13) CVTH_PAD(14) CVTH_PAD(15) CVTH_PAD(16) CVTH_PAD(17)
    CVTH_PAD(18) CVTH_PAD(19) CVTH_PAD(20) CVTH_PAD(21) CVTH_PAD(22) CVTH_PAD(23)
    CVTH_PAD(24) CVTH_PAD(25) CVTH_PAD(26) CVTH_PAD(27) CVTH_PAD(28) CVTH_PAD(29)
    CVTH_PAD(30) CVTH_PAD(31) CVTH_PAD(32) CVTH_PAD(33) CVTH_PAD(34) CVTH_PAD(35)
    CVTH_PAD(36) CVTH_PAD(37) CVTH_PAD(38) CVTH_PAD(39) CVTH_PAD(40) CVTH_PAD(41)
    CVTH_PAD(42) CVTH_PAD(43) CVTH_PAD(44) CVTH_PAD(45) CVTH_PAD(46) CVTH_PAD(47)
    CVTH_PAD(48) CVTH_PAD(49) CVTH_PAD(50) CVTH_PAD(51) CVTH_PAD(52) CVTH_PAD(53)
    CVTH_PAD(54) CVTH_PAD(55) CVTH_PAD(56) CVTH_PAD(57) CVTH_PAD(58) CVTH_PAD(59)
    CVTH_PAD(60) CVTH_PAD(61) CVTH_PAD(62) CVTH_PAD(63) CVTH_PAD(64) CVTH_PAD(65)
    CVTH_PAD(66) CVTH_PAD(67) CVTH_PAD(68) CVTH_PAD(69) CVTH_PAD(70) CVTH_PAD(71)
    CVTH_PAD(72) CVTH_PAD(73) CVTH_PAD(74) CVTH_PAD(75) CVTH_PAD(76) CVTH_PAD(77)
    CVTH_PAD(78) CVTH_PAD(79) CVTH_PAD(80) CVTH_PAD(81) CVTH_PAD(82) CVTH_PAD(83)
    CVTH_PAD(84) CVTH_PAD(85) CVTH_PAD(86) CVTH_PAD(87) CVTH_PAD(88) CVTH_PAD(89)
    CVTH_PAD(90) CVTH_PAD(91) CVTH_PAD(92) CVTH_PAD(93) CVTH_PAD(94) CVTH_PAD(95)
    CVTH_PAD(96) CVTH_PAD(97) CVTH_PAD(98) CVTH_PAD(99) CVTH_PAD(100) CVTH_PAD(101)
    CVTH_PAD(102) CVTH_PAD(103) CVTH_PAD(104) CVTH_PAD(105) CVTH_PAD(106) CVTH_PAD(107)
    CVTH_PAD(108) CVTH_PAD(109) CVTH_PAD(110) CVTH_PAD(111) CVTH_PAD(112) CVTH_PAD(113)
    CVTH_PAD(114) CVTH_PAD(115) CVTH_PAD(116) CVTH_PAD(117) CVTH_PAD(118) CVTH_PAD(119)
    CVTH_PAD(120) CVTH_PAD(121) CVTH_PAD(122) CVTH_PAD(123) CVTH_PAD(124) CVTH_PAD(125)
    CVTH_PAD(126) CVTH_PAD(127) CVTH_PAD(128) CVTH_PAD(129) CVTH_PAD(130) CVTH_PAD(131)
    CVTH_PAD(132) CVTH_PAD(133) CVTH_PAD(134) CVTH_PAD(135) CVTH_PAD(136) CVTH_PAD(137)
    CVTH_PAD(138) CVTH_PAD(139) CVTH_PAD(140) CVTH_PAD(141) CVTH_PAD(142) CVTH_PAD(143)
    CVTH_PAD(144) CVTH_PAD(145) CVTH_PAD(146) CVTH_PAD(147) CVTH_PAD(148) CVTH_PAD(149)
    CVTH_PAD(150) CVTH_PAD(151) CVTH_PAD(152) CVTH_PAD(153) CVTH_PAD(154) CVTH_PAD(155)
    CVTH_PAD(156) CVTH_PAD(157) CVTH_PAD(158) CVTH_PAD(159) CVTH_PAD(160) CVTH_PAD(161)
    CVTH_PAD(162) CVTH_PAD(163) CVTH_PAD(164) CVTH_PAD(165) CVTH_PAD(166) CVTH_PAD(167)
    CVTH_PAD(168) CVTH_PAD(169) CVTH_PAD(170) CVTH_PAD(171) CVTH_PAD(172)
#undef CVTH_PAD
    virtual int isVoiceActive();      // slot 173 -> vtable offset 0x2BC
};

// Init data tables for slot states (3 u32s each).
extern "C" u32 lbl_eu_80539BA8[3];
extern "C" u32 lbl_eu_80539BB4[3];
extern "C" u32 lbl_eu_80539BC0[3];

// Vtable for CVS_THREAD_HAGE (0x1C = 28 bytes = 7 entries).
extern "C" u32 lbl_eu_80539BCC[7];

/**
 * CVS_THREAD_HAGE - voice thread variant for "Hage" (bald/bare) audio.
 *
 * Inherits from CVS_THREAD and returns a smaller thread-local buffer
 * (240 bytes) compared to the BUF variant (260 bytes).
 */
struct CVS_THREAD_HAGE_VTable;

// Init-state triple view over lbl_eu_80539BA8 (word 0 is stored into the
// object's pointer-sized state0 field, hence the pointer type).
struct HAGEInitData {
    u32* w0;   // 0x00
    u32 w1;    // 0x04
    u32 w2;    // 0x08
};

// Raw layout of the CVS_THREAD_HAGE object exposing the implicit vtable
// pointer at 0x1C (owned by the CVS_THREAD base) so the factory can
// override it with the HAGE vtable, plus the two voice slots.
struct CVS_THREAD_HAGE_raw {
    u32* state0;                // 0x00: init-state word 1
    u32 state1;                 // 0x04: init-state word 2
    u32 state2;                 // 0x08: init-state word 3
    u32 field_0x0C;             // 0x0C
    u32 field_0x10;             // 0x10
    u32 field_0x14;             // 0x14
    u32 field_0x18;             // 0x18
    const CVS_THREAD_HAGE_VTable* vtable;  // 0x1C
    CVoiceHandle* slot1;        // 0x20: voice handle pointer (slot 1)
    CVoiceHandle* slot2;        // 0x24: voice handle pointer (slot 2)
};

class CVS_THREAD_HAGE : public CVS_THREAD {
public:
    // Size of the thread-local voice buffer in bytes.
    static const int BUFFER_SIZE = 0xF0;

    CVoiceHandle* field_0x20;  // 0x20: voice handle pointer (slot 1)
    CVoiceHandle* field_0x24;  // 0x24: voice handle pointer (slot 2)

    int blank1() override;
};

// CVoiceHandle is defined once in the shared base header CVS_THREAD.hpp.
