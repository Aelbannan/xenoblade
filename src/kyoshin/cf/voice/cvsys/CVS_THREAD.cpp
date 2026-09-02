#include "kyoshin/cf/voice/cvsys/CVS_THREAD.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/CfGameManagerData.hpp"          // lbl_eu_80663E14
#include "libs/monolib/src/scn/CScn_8049603C.hpp"     // func_8049603C (single owner decl)
#include "monolib/math/Random.hpp"

// Phantom vtable view over a CVoiceHandle vtable (vptr at +0x00) so the
// idle-check slot lands at byte offset 0x2BC (declared slot 173; MWCC
// places declared slot P at vtable offset (P+2)*4). Cast+call only, so no
// vtable is emitted for this view.

// Result of the target-state virtual at vtable offset 0x2A4: +0x04 is an
// actor id fed to findObjectById, +0x50 is a work object carrying a u16
// state word at +0x48 checked by the battle-state helpers.
struct UnkVoiceWork {
    u8 _00[0x48];
    u16 field_48;
};
struct CVoiceVf167Res {
    u8 _00[4];
    u32 field_04;
    u8 _08[0x48];
    UnkVoiceWork* field_50;
};

// C++-mangled imports (MWCC emits findObjectById__Fi /
// getCfObjectPc__FPQ22cf12CfObjectMove, matching the retail symbols).
namespace cf { class CfObjectMove; }
void* findObjectById(int id);
void* getCfObjectPc(cf::CfObjectMove* objMove);
// Battle-state helpers (retail unmangled C-ABI symbols).
extern "C" int func_80146148(int value);
extern "C" int func_80146300(u32 id, u32 flag);

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
    CVTV_N(160) CVTV_N(161) CVTV_N(162) CVTV_N(163) CVTV_N(164) CVTV_N(165) CVTV_N(166)
    virtual CVoiceVf167Res* voiceTarget(); // slot 167 -> vtable offset 0x2A4
    CVTV_N(168) CVTV_N(169) CVTV_N(170) CVTV_N(171) CVTV_N(172)
#undef CVTV_N
    virtual int idle();               // slot 173 -> vtable offset 0x2BC
};

// ── Local phantom views for func_802A3D54 ─────────────────────────────────

// Voice object flag word at +0x64 (bit 2 selects the alternate sdata2
// constant for the sound request).
struct CVoiceSndFlagView {
    u8 _00[0x64];
    u32 flag64;
};

// Scene camera-view block returned by func_8049603C (float at +0xC is the
// remaining display-time fraction subtracted from the base volume).
struct CVoiceSndCamView {
    u8 _00[0xC];
    f32 field_C;
};

namespace cf {
// TU-local phantom view of cf::CfObject: emits the retail-mangled reloc
// requestVoice__Q22cf8CfObjectFiUlff. object/CfObject.hpp declares a
// void-returning form shared with other TUs; the mangling encodes neither
// the return type, and this call site passes the callee's r3 straight out.
class CfObject {
public:
    int requestVoice(int a, unsigned long b, float c, float d);
};
} // namespace cf

// ── Extern symbols ────────────────────────────────────────────────────────
